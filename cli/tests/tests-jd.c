#include"tests-jd.h"
#include<jd.h>
#include<stdlib.h>
#include<stdio.h>

/* global julian day test routine */

int check_jd()
  {
  if (check_jd_to_wd_conv() == 0)
    {
      return 0;
    }
  if (check_jd_to_wd_individual() == 0)
    {
      return 0;
    }
  return 1;
  }


/* routine checking julian day to gregorian day conversion
 * by doing julian day to gregorian day and then back gregorian day to julian
 * day. */

int check_jd_to_wd_conv ()
{
  long int jd, jd2;
  int day, month, year, dow;
  for (jd=1; jd<200000; jd=jd+167)
    {
      jd_to_wd (jd, &day, &month, &year, &dow);
      jd2 = wd_to_jd(day, month, year);
      if (jd != jd2)
        {
          printf ( "error: jd %ld get converted to %d/%d/%d, which gets converted to jd %ld.\n", jd, day, month, year, jd2);
          return 0;
        }
    }
  return 1;
}

/* routine checking julian day to gregorian day conversion
 * by testing individual dates. */

int check_jd_to_wd_individual ()
{
  if (check_jd_to_wd_individual_one(2299160L, 1582, 10, 4, THURSDAY) == 0) return 0; // end of julian calendar
  if (check_jd_to_wd_individual_one(2299161L, 1582, 10, 15, FRIDAY) == 0) return 0; // beginning of gregorian calendar
  if (check_jd_to_wd_individual_one(2455729L, 2011, 06, 16, THURSDAY) == 0) return 0; // 16 june 2011
  if (check_jd_to_wd_individual_one(0, -4712, 1, 1, MONDAY) == 0) return 0; // JD 0
  if (check_jd_to_wd_individual_one(588466L, -3101, 02, 18, FRIDAY) == 0) return 0; // kali yuga beginning
  return 1;
}

/* checking one individual date by converting a julian day to western day
 * and then the other way. Also checking day of week. */

int check_jd_to_wd_individual_one (long int jd, int wy, int wm, int wd, int wdow)
{
  int year, month, day, dow;
  long int jd2;
  jd_to_wd(jd, &day, &month, &year, &dow);
  if (year != wy || month != wm || day != wd || dow != wdow)
    {
      printf("error: jd %ld gets converted to %d/%d/%d (dow: %d), but should be %d/%d/%d (dow: %d).\n", jd, day, month, year, dow, wd, wm, wy, wdow);
      return 0;
    }
  jd2 = wd_to_jd(wd, wm, wy);
  if (jd != jd2)
        {
          printf ( "error: %d/%d/%d gets converted to jd %ld, but should be %d.\n", wd, wm, wy, jd2, jd);
          return 0;
        }
  return 1;
}
