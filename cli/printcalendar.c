/*
License for Tibastro - Tibetan almanach calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "tbstructures.h"
#include "system.h"
#include "stdio.h"
#include "translation.h"
#include "astrology.h"
#include "jd.h"
#include "printcalendar.h"

/*
 * The idea of this file is to print a calendar which is exactly the same as the calendars on Kalacakra.org
 * in order to compare and make large tests.
 */

void
print_calendar (FILE * f, int y, astro_system * asys)
{
  tib_day *td;
  long int prev_month = 0;	// to record last month's number
  unsigned char prev_month_type = NORMAL;	// to record last month's type
  td = get_tib_day_from_tib_date (y, 1, 1, asys, FIRST, FIRST);
  get_day_astro_data (td, asys, 0);	// to initialize the astrological data
  print_year_info (f, td->month->year);

  while (td->month->year->year == y)
    {
      // if we changed month (it also prints the first month
      if (prev_month != td->month->month
	  || td->month->type != prev_month_type)
	{
	  print_month_info (f, td->month);
	  prev_month = td->month->month;
	  prev_month_type = td->month->type;
	}
      print_day_info (f, td, asys);
      // automaticall updated td->month, td->year, etc.
      tib_day_next (td, asys);	// will change td->month->year->year
    }
}

void
print_year_info (FILE * f, tib_year * year)
{
  if (!year->astro_data)
    get_year_astro_data (year);
  fprintf (f, "\nNew Year: %ld, %s-%s-%s\n", year->year,
	   get_element_5_str (year->astro_data->element),
	   get_gender_str (year->astro_data->gender),
	   get_animal_str (year->astro_data->animal));
}

void
print_month_info (FILE * f, tib_month * month)
{
  fprintf (f, "\nTibetan Lunar Month: %ld", month->month);
  switch (month->type)
    {
    case FIRST_OF_DOUBLE:
      fprintf (f, " (Intercalary)");
      break;
    case SECOND_OF_DOUBLE:	// TODO: there was a reference to epch !=2, see why
      fprintf (f, " (Delayed)");
      break;
    default:
      break;
    }
  fprintf (f, " - %s-%s-%s\n\n",
	   get_element_5_str (month->astro_data->element),
	   get_gender_str (month->astro_data->gender),
	   get_animal_str (month->astro_data->animal));
}

static char *dayoweek[7] =
  { "Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri" };

static char *byedT[11] = { "gdab pa", "byis pa", "rigs can", "til rdung",
  "khyim skyes", "tshong ba", "vishti", "mi sdug pa",
  "bkra shis", "rkang bzhi", "klu"
};

static char *yogaT[27] =
  { "rnam sel", "mdza' bo", "tshe dang ldan pa", "skal bzang",
  "dge byed", "shin tu 'grams", "las bzang", "'dzin byed",
  "zug rngu", "'grams", "'phel", "brtan pa", "yongs bsnun",
  "dga' ba", "rdo rje", "dngos grub", "phan tshun",
  "mchog can", "yongs 'joms", "zhi ba", "grub pa",
  "bsgrub bya", "dge ba", "dkar po", "tshangs pa",
  "dbang po", "'khon 'dzin"
};

static char *lunmanT[27] =
  { "tha skar", "bra nye", "smin drug", "snar ma", "mgo",
  "lag", "nabs so", "rgyal", "skag", "mchu", "gre", "dbo",
  "me bzhi", "nag pa", "sa ri", "sa ga", "lha mtshams",
  "snron", "snrubs", "chu stod", "chu smad", "gro zhin",
  "mon gre", "mon gru", "khrums stod", "khrums smad",
  "nam gru"
};

static char *Clunman[28] =
  { "Jiao", "Kang", "Di", "Fang", "Xin", "Wei", "Ji", "Dou",
  "Niu", "Nu", "Xu", "Wei", "Shi", "Bi", "Kui", "Lou",
  "Wei", "Mao", "Bi", "Zui", "Can", "Jing", "Gui", "Liu",
  "Xing", "Zhang", "Yi", "Zhen"
};

static char *wmonths[12] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
  "Sep", "Oct", "Nov", "Dec"
};

char *cycparT[8] =
  { "zon", "li", "khon", "dwa", "khen", "kham", "gin", "zin" };

void
print_day_info (FILE * f, tib_day * td, astro_system * asys)
{
  int dow, wday, wmonth, wyear;	// in order to compute the western date
  long int gzadag0;
  // First special case: the case of ommited days:
  if (td->ommited == OMMITED)	// This is for omitted lunar day
    {
      fprintf (f, "%ld. Omitted: %s %s %d\n",
	       td->tt,
	       get_animal_str (td->astro_data->l_animal),
	       cycparT[td->astro_data->l_trigram], td->astro_data->l_sme_ba);
      return;
    }
  // In the case of the first of a duplicated day, gzadag0 is td->gzadag[0]-1, and we have
  // to print gzadag0;60,0 instead of td->gzadag[0];td->gzadag[1],td->gzadag[2]
  if (td->duplicated == FIRST_OF_DUPLICATED)
    gzadag0 = (td->gzadag[0] + 6) % 7;
  else
    gzadag0 = td->gzadag[0];
  jd_to_wd (td->gd, &wday, &wmonth, &wyear, &dow);
  // Line 1
  fprintf (f, "%ld: %s. %s. %s-%s; %d %s %d\n",
	   td->tt,
	   dayoweek[gzadag0],
	   lunmanT[td->astro_data->moonlong_db[0]],
	   get_element_4_str (get_dow_element (dow)),
	   get_element_4_str (get_lunar_mansion_element
			      (td->astro_data->moonlong_db[0])), wday,
	   wmonths[wmonth - 1], wyear);
  // Line 2, not printed in case of first of a duplicated day
  if (td->duplicated != FIRST_OF_DUPLICATED)
    fprintf (f, "\x20\x20%s, %s, %s, %s %d\n",
	     yogaT[td->astro_data->yoga[0]],
	     byedT[td->astro_data->karana],
	     get_animal_str (td->astro_data->l_animal),
	     cycparT[td->astro_data->l_trigram], td->astro_data->l_sme_ba);
  // Line 3
  if (td->duplicated == FIRST_OF_DUPLICATED)
    {
      // in this case, we should print gzadag[0]-1;60,0 instead of gzadag
      //TODO: understand why 60...
      fprintf(f, "\x20\x20%ld;60,0",  gzadag0);
    }
  else
      fprintf (f, "\x20\x20%ld;%ld,%ld",td->gzadag[0], td->gzadag[1], td->gzadag[2]);
  fprintf (f, " %ld;%ld,%ld %ld;%ld,%ld %ld;%ld,%ld",
	   td->astro_data->moonlong_db[0], td->astro_data->moonlong_db[1],
	   td->astro_data->moonlong_db[2], td->nyidag[0], td->nyidag[1],
	   td->nyidag[2], td->astro_data->yoga[0], td->astro_data->yoga[1],
	   td->astro_data->yoga[2]);
  if (asys->type != TSURPHU)
    fprintf (f, " %d;%d,%d",
	     td->astro_data->sideral_day[0], td->astro_data->sideral_day[1],
	     td->astro_data->sideral_day[2]);
  fprintf (f, "\n");
  // Line 4
  fprintf (f, "\x20\x20Solar: %s-%s. %s",
	   get_element_5_str (td->astro_data->s_element),
	   get_animal_str (td->astro_data->s_animal),
	   Clunman[td->astro_data->c_lunar_mansion]);
  if (asys->type != TSURPHU)
    fprintf (f, " %d", td->astro_data->s_sme_ba);
  fprintf (f, "\n");
  // Line 5
  /* uncomment if you want sadag
     if (asys->type == TSURPHU)
     {
     print_sadag(f, td->astro_data);
     } */
  switch (td->astro_data->anniversary)
    {
    case ANN_DEM_MIR:
      fprintf (f, "  From 1st to 15th, Demonstration of Miracles.\n");
      break;
    case ANN_REV_KALACAKRA:
      fprintf (f, "  Revelation of the Kalacakra Tantra.\n");
      break;
    case ANN_BIRTH:
      fprintf (f, "  Birth of the Buddha.\n");
      break;
    case ANN_ENL_PARI:
      fprintf (f, "  Enlightenment and Parinirvana of the Buddha.\n");
      break;
    case ANN_TURN_WHEEL:
      fprintf (f, "  Turning of the Wheel of the Dharma.\n");
      break;
    case ANN_ENTRY_WOMB:
      fprintf (f, "  The Buddha's entry into the womb of his mother.\n");
      break;
    case ANN_DESCENT_REALM:
      fprintf (f, "  Descent of the Buddha from the realm of the gods.\n");
      break;
    default:
      break;
    }
}

void
print_sadag (FILE * f, tib_day_astro_data * tda)
{
  unsigned char tmp = 0;
  // we set tmp to the total number of Earth lords to print
  // remember that tda->nn can be 2
  if (tda->nn)
    tmp = 1;
  tmp = tmp + tda->yk + tda->zph + tda->kbz + tda->kth;
  // a 2c optimization...
  if (tmp == 0)
    return;
  fprintf (f, "\x20\x20\x20\x20");
  if (tda->yk)
    {
      fprintf (f, "yan kwong");
      tmp = tmp - 1;
      if (tmp)
	printf (", ");
    }
  if (tda->zph)
    {
      fprintf (f, "zin phung");
      tmp = tmp - 1;
      if (tmp)
	printf (", ");
    }
  if (tda->kbz)
    {
      fprintf (f, "klu bzlog");
      tmp = tmp - 1;
      if (tmp)
	printf (", ");
    }
  if (tda->kth)
    {
      fprintf (f, "klu thebs");
      tmp = tmp - 1;
      if (tmp)
	printf (", ");
    }
  if (tda->nn == 1)
    fprintf (f, "nyi nag");
  if (tda->nn == 2)
    fprintf (f, "nyi nag ngan pa dgu 'dzom");
  fprintf (f, "\n");
}
