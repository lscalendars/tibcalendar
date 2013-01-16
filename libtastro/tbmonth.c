/*********************************************************************************
License for Tibastro - Tibetan astronomical and astrological calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

Permission is hereby granted, free of charge, to any person  obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so, subject
to the following conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
************************************************************************************/

#include <stdio.h> // for printf, currently used for errors, but should be changed...
#include "tbday.h"
#include "tbmonth.h"
#include "utils.h"
#include "jd.h"			// for jd_to_wd
#include "system.h" // for epoch
#include "tbstructures.h"
#include "astrology.h" // when we want a next day or month, we need to update the astrology linked to it if any

/* Function giving informations on the next month of a given tib_month */
// it supposes that month->type and month->asked_month is well filled
void
tib_month_next (tib_month* month, astro_system *asys)
{
  long int adj_mth;
  if ( !month->zero_month_flag )   // if zeromthfg == 1, we just call adj_zla for a second time with the same values, else:
      {
      next_zla_dag (month->true_month);
            // when we've reached the last month, month->asked_month should return to 1
      if (month->month == 12 && month->type != FIRST_OF_DOUBLE)
        {
        // a bit subtle: if it's the last month of year:
        //   - if asked_month = 12, then it should be 1
        //   - if asked_month = 13, this means that the adjusted month will be asked_month -1
        //     and thus if we call adj_zla with asked_month = 1, the result will be 12 and thus an
        //     an infinite loop, so it should be 2
        month->asked_month = month->asked_month - 11; 
          }
      else
        {
        month->asked_month = (month->asked_month + 1);
        if (month->asked_month == 15)
          month->asked_month = 1;
        }
      }
  adj_mth = adj_zla (month->asked_month, month->true_month, &(month->zero_month_flag), asys);
  get_month_data (asys, month->true_month[0], month->rilcha, month->nyidru, month->gzadru);
  if (month->type == SECOND_OF_DOUBLE)
      month->type = NORMAL;
  if (month->type == FIRST_OF_DOUBLE)
      month->type = SECOND_OF_DOUBLE;
  if (adj_mth < 0L)
    {
      month->type = FIRST_OF_DOUBLE;
      month->month = -adj_mth;
    }
  else
     month->month = adj_mth;
  if (month->month == 1 && month->type != SECOND_OF_DOUBLE)  // new year
    {
    month->year->year = month->year->year + 1;
    if (month->year->astro_data)
      get_year_astro_data(month->year);
    }
 // finally, we update the astrology linked to it, if any
 if (month->astro_data)
   get_month_astro_data(month, asys);
}

/* Function to calculate true month, "zla ba rnam par dag pa" from a tibetan date
 * inputs: 
 *  - epch is the epoch
 *  - y is the tibetan year
 *  - m is the tibetan month
 * output is a table of two int representing the true month: 
 *  - first is the integer part
 *  - second is the remainder mod. 65
 *  - returned in the argument zla_dagp
 * reference : KTC p. 15
 * operation is first getting M, then M*1;2 + 0;55 (65)
 */
void
zla_dag (const epoch *epch, long int y, long int m, long int zd[2])
{
  long int yr, a, b;
  yr = y - epch->year;
  a = 12 * yr + m - 3;		// intermediate value (M)
  b = 2 * a + epch->eyr_a;
  zd[1] = b % 65;
  zd[0] = a + b / 65;
}

//TODO: test
inline void
next_zla_dag (long int zd[2])
{
  //equivalent to add_lst_2(zd[2], {1,2}, 65);
  long int r = zd[1] + 2;
  zd[1] = r % 65;
  zd[0] = zd[0] + 1 + r/65;
}

/* In the following functions, some variable represent always the same things:
 *  - ty: the almost tibetan year (numbered as western years), with the
 *    particularity that all years are considered to have 12 month, and thus
 *    can be different from the true tibetan year for a few month if preceding
 *    year has 13 lunar months. This is useful for calculations but will
 *    need adjustments before being printed to the user
 *  - tm: the count of month from the beginning of ty (first month: tm=0).
 *    tm and ty are intermediate values.
 *  - adj_mth: the adjusted month as appearing, with the following convention:
 *      - if tm is the first month of a doubled month, adj_mth = -tm
 *      - else adj_mth is the month appearing in the calendar (can be different
 *        from tm because of doubled month)
 *  - zd[2]: the true month (zd[0]), with the remainder mod 65, called
 *    intercalation index in KTC (zd[1])
 *    In the case of zd[1] index becoming 0 or 1, we modify zd[0] for
 *    the first month with zd[1] of 0 or 1: zd[0] becomes zd[0]-1, but
 *    gets back to normal for the second month with zd[1] 0 or 1.
 *  - zeromthflg: see comments of phugpa_adj_zla
 */

/* Function to adjust month number, if needed
 * inputs:
 *  - epch: the epoch, especially the two remainders for the intercalary month (cf. KTC p. 51)
 *  - zd, tm, zeromthfg: explained above
 * returns adj_mth, as explained above
 * it also modifies zd[0] and zeromthfg so that it is well set for next use (might pose problems sometimes)
 * taking the example of KTC54:
 
 4th month:   zd=842;64 -> adj_mth = 3
 5th month:   zd=844;1   -> adj_mth = 5
 6th month:   zd=845;3   -> adj_mth = 6
 
 but 843;1 will be used for the missing 4th month
 
 the expected sequence of calls for this function for this example is thus:
 call 1: tm=4   zd=842;64  zeromonthfg=0 -> adj_mth = 3
 call 2: tm=5   zd=844;1    zeromonthfg=0 -> adj_mth = 4, zeromthfg=1 and zd=843;1
 call 3: tm=5   zd=843;1    zeromonthfg=1 -> adj_mth = 5, zeromthfg=0 and zd = 844;1
 call 4: tm=6   zd=845;3    zeromonthfg=0 -> adj_mth = 6 

tm here is precisely month->expected month
 
 * another example of expected sequence of calls and results (with double month)
  tm  zd[0]   zd[1]    zeromthfg (before-after)   modified zd[0]  adj_mth    type of month
  8      23        45                     0-0                             23                       8                 normal
  9      24        47                     0-0                             24                       9                 normal
  10    25        49                     0-0                             25                       -10               first of double
  11    26        51                     0-0                             26                       10               second of double
  12    27        52                     0-0                             27                       11               normal
  13    28        54                     0-0                             28                       12               normal
  1      29        56                     0-0                             29                       1               normal
  ...   ...         ...                        ...                              ...                        ...               ...
  5     32        62                      0-0                             32                       4                normal
  6     33        64                      0-0                             33                       5                normal
  7     35        1                        0-1                             34                       6                normal
  7     34        1                        1-0                             35                       7                normal
  8     35        3                        0-0                             36                       8                normal
 */
long int
phugpa_adj_zla (long int tm, long int zd[2], epoch *epch,
		unsigned char *zeromthfg)
{
  long int adj_mth;
  if (zd[1] == epch->zlasho || zd[1] == epch->zlasho + 1)
    {
      // for month taking the name of the following month, the convention is to
      // return -tm. the month->type must be set to FIRST_OF_DOUBLE
      return -tm;
    }
  else
    {
      if (zd[1] > epch->zlasho + 1)
	{
	  adj_mth = tm - 1;
	}
      else
	{
	  // here we modify zd[0] for the first month with a zd[1] of 0 or 1
	  if (zd[1] == 0 || zd[1] == 1)
	    {
	      if (!*zeromthfg)
		{
		  *zeromthfg = 1;
		  adj_mth = tm - 1;
		  zd[0] = zd[0] - 1;
		}
	      else
		{
		  *zeromthfg = 0;
		  adj_mth = tm;
		  zd[0] = zd[0] + 1;
		}
	    }
	  else			// Arrive here if 1 < zladag[1] < zlasho
	      adj_mth = tm;
	}
    }
	  if (adj_mth == 0)
	      adj_mth = 12;
  return adj_mth;
}

/* Idem for Tsurphu system.
 */
long int
tsurphu_adj_zla (long int tm, long int zd[2],
		unsigned char *zeromthfg)
{
      if ( zd[1] == 0 || zd[1] == 1 )
      {
         if (!*zeromthfg)
          {
           *zeromthfg = 1;
            zd[0] = zd[0] - 1;
            return -tm;
          }
        else
          {
            *zeromthfg = 0;
            zd[0] = zd[0] + 1;
          }
      }
  return tm;
}

/* Chapeau function to call the above two (TODO: should be moved elsewhere later maybe) */
// do not forget this can modify zd[0]
long int adj_zla (long int tm, long int zd[2],
		unsigned char *zeromthfg, astro_system *sys)
{
	    if (sys->type == PHUGPA || sys->type == SHERAB_LING)
        return phugpa_adj_zla (tm, zd, sys->epoch, zeromthfg);
      else if (sys->type == TSURPHU)
        return tsurphu_adj_zla (tm, zd, zeromthfg);
      else
        printf ("error: function adj_sla called with unknown system.");
        return 0;
}

/*
 * Function taking the julian day and returning the true year and month. To
 * do so it computes the julian day for the new moon of the consecutive month
 * until we find a new moon that is after the julian day we want, and we go back
 * one month earlier.
 * 
 * The first approximation for the month and year is 3 month before the western
 * date. It is not traditional, but there is no traditional way of looking for
 * daily data for a julian day... It doesn't change anything for the computation
 * though.
 * 
 * inputs:
 *  - jd: the julian day for which we want the date
 *  - epch: the epoch
 * outputs:
 *  - it fills the month fields with everything needed
 */
// TODO; modify it to use tib_month_next
void
find_month_and_year (long int jd, astro_system *sys, tib_month *month)
{
  long int tm, ty, adj_mth, gd, hld_tm; 
  int wd, wm, wy, wdow; 
  // a flag to take the zeroth month into account twice
  unsigned char zeromthfg = 0;
  unsigned char done = 0;
  long int zd[2] = { 0, 0 };	// anomaly
  long int nyibar[6] = { 0, 0, 0, 0, 0, 0 };	// mean solar longitude
  long int nyidag[6] = { 0, 0, 0, 0, 0, 0 };	// true solar longitude
  long int gzadag[6] = { 0, 0, 0, 0, 0, 0 };	// true weekday
  
  jd_to_wd (jd, &wd, &wm, &wy, &wdow);
  tm = (long int) wm - 3;
  ty = (long int) wy;
  if (tm < 1)
    {
      ty = ty - 1;
      tm = tm + 12;
    }
  
  while (!done)
    {
      if (!zeromthfg)		// We need to use the same data, twice.
	{
	  zla_dag (sys->epoch, ty, tm, zd);
	  month->asked_month = tm;
	}

      adj_mth = adj_zla (tm, zd, &zeromthfg, sys);	// do not forget this can modify zd[0]

      // here the lunar day is 0 thus we don't need the corrections that
      // apply to other lunar days (KTC 23).
      get_month_data (sys, zd[0], month->rilcha, month->nyidru, month->gzadru);
      gd = get_tt_data (sys, zd[0], month->gzadru, month->nyidru, month->rilcha, 0, nyidag, gzadag, nyibar);

      // Now, have we gone far enough? We first find the true new Moon date 
      // immediately after or equal to our target date (only for coming back
      // to the month before afterwards)
      if (gd >= jd)
	done = 1;
      else
	{
	  month->year->year = ty;
    hld_tm = tm;
	  month->month = adj_mth;
	  month->true_month[0] = zd[0];
	  month->true_month[1] = zd[1];
	  month->start_gd = gd;
	  if (!zeromthfg)
	    tm = tm + 1;
	  if (tm > 12)
	    {
	      tm = tm - 12;
	      ty = ty + 1;
	    }
	}
    }
  // month->year,month, etc. contains data about the month before
  // adjusting the year so that it is really the tibetan year
  if (month->month == 12 && hld_tm == 1)
    month->year->year = month->year->year - 1;
  if ( month->month < 0L ) // Intercalary
    {
      month->type = FIRST_OF_DOUBLE;
      month->month = -month->month;
    }
  // for the Phugpa system, delayed month are month with zd[1] equal to 50 or 51, for tsurphu it's 0 or 1
  else if ((sys->type == PHUGPA && (month->true_month[1] == sys->epoch->zlasho+2 || month->true_month[1] == sys->epoch->zlasho+3 ) ) || (sys->type == TSURPHU && (month->true_month[1] == 0 || month->true_month[1] == 1 ) ))
    month->type = SECOND_OF_DOUBLE;
  // we need to get the rilcha, nyidru and gzadru for the month
  get_month_data (sys, month->true_month[0], month->rilcha, month->nyidru, month->gzadru);
}

/* Function to calculate month mean Sun, "nyi ma'i dhru ba" at new moon
 *  - zd0 is the true month main part
 *  - nyidru, rilcha and gzadru are the result
 * See KTC p. 21 for details
 */
void
get_month_data (astro_system *asys, long int zd0, long int rilcha[2], long int nyidru[6], long int gzadru[6])
{
  long int a, b;
  static long int gza_drup_const[5] = { 1, 31, 50, 0, 480 } ;
  // first we compute rilcha
  b = zd0 + asys->epoch->ril_b;
  a = 2 * zd0 + asys->epoch->ril_a + b / 126;
  rilcha[1] = b % 126;
  rilcha[0] = a % 28;
  //printf("getmonthdatarilcha0 : %ld\nrilcha1 : %ld\n", rilcha[0], rilcha[1]);
  // then nyidru
  mul_lst (nyidru, asys->nyi_drup_const, zd0, 27, asys->sun_f);
  add_lst (nyidru, nyidru, asys->epoch->nyid, 27, asys->sun_f);
  // then gzadru
  mul_lst (gzadru, gza_drup_const, zd0, 7, 707);
  add_lst (gzadru, gzadru, asys->epoch->gzad, 7, 707);
}
