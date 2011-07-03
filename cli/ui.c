#include"ui.h"
#include<stdio.h>
#include<jd.h> // for MONDAY, etc.

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

void print_weekday(long int weekday)
{
  switch(weekday)
    {
      case MONDAY:
        printf("monday");
        break;
      case TUESDAY:
        printf("tuesday");
        break;
      case WEDNESDAY:
        printf("wednesday");
        break;
      case THURSDAY:
        printf("thursday");
        break;
      case FRIDAY:
        printf("friday");
        break;
      case SATURDAY:
        printf("saturday");
        break;
      default:
        printf("sunday");
        break;
    }
}

void print_tib_month(tib_month *tm) {
  printf("year: %ld\n", tm->year);
  printf("month: %ld (%ld) - %ld;%ld\n", tm->month, tm->adjusted_month, tm->true_month[0], tm->true_month[1]); 
  printf("starting day: %ld\n", tm->start_gd);
  printf("anomaly: %ld;%ld\n", tm->rilcha[0], tm->rilcha[1]);
  printf("mean solar longitude: "); ui_print_lst(tm->nyidru,5); printf("\n");
  printf("mean weekday: "); ui_print_lst(tm->gzadru,5); printf("\n");
  }


void print_tib_planets(tib_day *td) {
  
  printf("mars longitude: "); ui_print_lst(td->marmurdag,5); printf("\n");
  printf("jupiter longitude: "); ui_print_lst(td->jupmurdag,5); printf("\n");
  printf("saturn longitude: "); ui_print_lst(td->satmurdag,5); printf("\n");
  printf("mercury longitude: "); ui_print_lst(td->mermurdag,5); printf("\n");
  printf("venus longitude: "); ui_print_lst(td->venmurdag,5); printf("\n");
  printf("rahu longitude: "); ui_print_lst(td->rahudong,5); printf("\n");
  }

void print_tib_day(tib_day *td)
{
  printf("    Month data:\n");
  print_tib_month(td->month);
  printf("    Day data:\n");
  printf("general day: %ld\n", td->gd);
  printf("lunar day: %ld", td->tt);
  switch(td->ommited)
    {
      case PREVIOUS_OMMITED:
        printf(" - previous day is ommited\n");
        break;
      case NEXT_OMMITED:
        printf(" - next day is ommited\n");
        break;
      default:
        break;
    }
  switch(td->duplicated)
    {
      case FIRST_OF_DUPLICATED:
        printf(" - first of a duplicated day\n");
        break;
      case SECOND_OF_DUPLICATED:
        printf(" - second of a duplicated day\n");
        break;
      default:
        printf("\n");
        break;
    }
  printf("mean solar longitude: "); ui_print_lst(td->nyibar,5); printf("\n");
  printf("true solar longitude: "); ui_print_lst(td->nyidag,5); printf("\n");
  printf("true weekday: "); ui_print_lst(td->gzadag,5); printf(" ("); print_weekday(td->gzadag[0]); printf(")\n");
  printf("    Planet data:\n");
  print_tib_planets(td);
}
