#include"ui.h"
#include<stdio.h>
#include "translation.h"
#include "tbstructures.h"
#include "jd.h"

void ui_print_lst(long int l[5], unsigned char length)
 {
   unsigned char i;
   printf("%ld;", l[0]);
   for (i=1;i<length-1;i++)
     {
       printf("%ld,", l[i]);
     }
   printf("%ld", l[length-1]);
 }

void init_ui()
 {
   return;
 }
 
void end_ui()
 {
   return;
 }


void print_tib_year(tib_year *ty) {
  printf("Royal year %ld\n", ty->year+127);
  if (ty->astro_data)
  {
  printf("year %d of rabjung %d: ", ty->astro_data->yor, ty->astro_data->rabjung);
  printf("%s %s %s, %d\n", get_gender_str(ty->astro_data->gender), get_element_5_str(ty->astro_data->element), get_animal_str(ty->astro_data->animal), ty->astro_data->sme_ba);
  }
}

void print_tib_month_astro_data(tib_month_astro_data *tma) {
  printf("%s %s %s\n", get_gender_str(tma->gender), get_element_5_str(tma->element), get_animal_str(tma->animal));
  }

void print_tib_month(tib_month *tm) {
  printf("    Month data:\n");
  switch(tm->type)
    {
      case NORMAL:
        printf("month: %ld - true month: %ld;%ld\n", tm->month, tm->true_month[0], tm->true_month[1]);
        break;
      case FIRST_OF_DOUBLE:
        printf("month: %ld (first of a double month) - true month: %ld;%ld\n", tm->month, tm->true_month[0], tm->true_month[1]); 
        break;
      default: // second of double
        printf("month: %ld (second of a double month) - true month: %ld;%ld\n", tm->month, tm->true_month[0], tm->true_month[1]); 
        break;
    }
  printf("starting (julian) day: %ld\n", tm->start_gd);
  printf("anomaly: %ld;%ld\n", tm->rilcha[0], tm->rilcha[1]);
  printf("mean solar longitude: "); ui_print_lst(tm->nyidru,5); printf("\n");
  printf("mean weekday: "); ui_print_lst(tm->gzadru,5); printf("\n");
  if (tm->astro_data)
     print_tib_month_astro_data(tm->astro_data);
  }


void print_tib_planet_data(tib_planet_data *pd) {
  printf("    Planeteray data for the day:\n");
  printf("mars longitude: "); ui_print_lst(pd->marmurdag,5); printf("\n");
  printf("jupiter longitude: "); ui_print_lst(pd->jupmurdag,5); printf("\n");
  printf("saturn longitude: "); ui_print_lst(pd->satmurdag,5); printf("\n");
  printf("mercury longitude: "); ui_print_lst(pd->mermurdag,5); printf("\n");
  printf("venus longitude: "); ui_print_lst(pd->venmurdag,5); printf("\n");
  printf("rahu longitude: "); ui_print_lst(pd->rahudong,5); printf("\n");
  }

// month_type is month->type
void print_tib_day_astro_data(tib_day_astro_data *tda, unsigned char month_type) {
  unsigned char tmp=0;
  printf("sideral day: %d;%d,%d (%s: %s)\n", tda->sideral_day[0], tda->sideral_day[1], tda->sideral_day[2], get_zodiac_str(tda->sideral_day[0]), get_zodiac_western_str(tda->sideral_day[0]));
  printf("lunar mansion at daybreak: %ld, ", tda->moonlong_db[0]);
  printf("yoga: %s, karana: %s\n", get_yoga_str(tda->yoga[0]), get_karana_str(tda->karana));
  // now anniversaries:
  switch (tda->anniversary)
   {
     case ANN_DEM_MIR:
       printf("From 1st to 15th, Demonstration of Miracles.\n");
       break;
     case ANN_REV_KALACAKRA:
       printf("Revelation of the Kalacakra Tantra.\n");
       break;
     case ANN_BIRTH:
       printf("Birth of the Buddha.\n");
       break;
     case ANN_ENL_PARI:
       printf("Enlightenment and Parinirvana of the Buddha.\n");
       break;
     case ANN_TURN_WHEEL:
       printf("Turning of the Wheel of the Dharma.\n");
       break;
     case ANN_ENTRY_WOMB:
       printf("The Buddha's entry into the womb of his mother.\n");
       break;
     case ANN_DESCENT_REALM:
       printf("Descent of the Buddha from the realm of the gods.\n");
       break;
     default:
       break;
   }
   // we set tmp to the total number of Earth lords to print
   // remember that tda->nn can be 2
   if (tda->nn)
     tmp = 1;
   tmp = tmp + tda->yk + tda->zph + tda-> kbz + tda->kth;
   // a 2c optimization...
   if (tmp == 0)
     return;
   printf("Earth-lords: ");
      if (tda->yk)
        {
        printf("yan kwong");
        tmp = tmp -1;
        if (tmp)
          printf(", ");
        }
      if (tda->zph)
        {
        printf("zin phung");
        tmp = tmp -1;
        if (tmp)
          printf(", ");
        }
      if (tda->kbz)
        {
        printf("klu bzlog");
        tmp = tmp -1;
        if (tmp)
          printf(", ");
        }
      if (tda->kth)
        {
        printf("klu thebs");
        tmp = tmp -1;
        if (tmp)
          printf(", ");
        }
      if (tda->nn == 1)
        printf("nyi nag");
      if (tda->nn == 2)
        printf("nyi nag ngan pa dgu 'dzom");
      printf("\n");
  }

void print_tib_day(tib_day *td)
{
  int gday, gmonth, gyear, gdow;
  printf("    Day data:\n");
  printf("general day: %ld ", td->gd);
  jd_to_wd(td->gd, &gday, &gmonth, &gyear, &gdow);
  printf("(western %.02d/%.02d/%d)\n", gday, gmonth, gyear);
  printf("lunar day: %ld", td->tt);
  switch(td->ommited)
    {
      case OMMITED: // TODO: improve, currently it does not make any sense
        printf(" - day is ommited");
        break;
      case PREVIOUS_OMMITED:
        printf(" - previous lunar day is ommited");
        break;
      case NEXT_OMMITED:
        printf(" - next lunar day is ommited");
        break;
      default:
        break;
    }
  switch(td->duplicated)
    {
      case FIRST_OF_DUPLICATED:
        printf(" - first of a duplicated lunar day");
        break;
      case SECOND_OF_DUPLICATED:
        printf(" - second of a duplicated lunar day");
        break;
      default:
        break;
    }
  printf("\n");
  printf("mean solar longitude: "); ui_print_lst(td->nyibar,5); printf("\n");
  printf("true solar longitude: "); ui_print_lst(td->nyidag,5); printf("\n");
  printf("true weekday: "); ui_print_lst(td->gzadag,5); printf(" (%s)\n", get_weekday_str(td->gzadag[0]));
  if (td->astro_data)
    print_tib_day_astro_data(td->astro_data, td->month->type); // TODO : check for td->month == NULL!
  if (td->planet_data)
    print_tib_planet_data(td->planet_data);
}

void print_tib_day_r(tib_day *td)
{
if (td->month && td->month->year)
  print_tib_year(td->month->year);
if (td->month)
  print_tib_month(td->month);
print_tib_day(td);
}
