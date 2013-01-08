/*********************************************************************************
License for TCG - Tibetan Calendar software for "grub rtsis"

Copyright (c) 2009-2011 Edward Henning

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

// this file contains a simple command line interface to the tibastro library

#include<stdio.h>
#include<stdlib.h>
#include"tests/tests-jd.h"
#include"tests/tests-tbday.h"
#include"ui.h"
#include"tibastro.h"
#include "system.h"
#include "astrology.h"
#include<jd.h>
#include<tbday.h>
#include<tbcal.h>



int
main ()
{
  // TODO: command-line analysis
  tib_day *td;
  astro_system *sys;
  td = new_tib_day();
  init_ui();
  //run_tests();
  //get_tpdata(2449719L); // 1/1/1995
  //get_tpdata(2449719L, td);
  //print_tib_day(td);
  sys = get_astro_system(PHUGPA);
  //get_day_data(2449749L, td, sys);
  //print_tib_day(td);
  free(td);
  td = get_new_year(1935, sys);
  get_day_astro_data(td, sys);
  print_tib_day_r(td);
  free(sys);
  //main_menu();
  end_ui();
  return 0;
}

void main_menu()
{
  char c;
  ui_print("\nWelcome in tibastro:\n");
  ui_print("  1. get tibetan calendar data for a date\n");
  ui_print("  2. run tests\n");
  ui_print("  3. exit\n");
  c = 0;
  while (c < '1' || c>'3')
    {
      c = ui_getchar();
      switch(c)
        {
          case '1':
            get_tibdata_for_date();
            break;
          case '2':
            run_tests();
            break;
          case '3':
            return;
            break;
          default:
            break;
        }
    }
  return;
}

void run_tests()
{
  int res;
  res = check_jd();
  if (res == 0)
    {
      ui_print("Tests for julian to western date conversion: FAILED!\n\n");
    }
  else
    {
      ui_print("Tests for julian to western date conversion: OK!\n\n");
    }
  res = check_tibday();
  if (res == 0)
    {
      ui_print("Tests for julian to tibetan date conversion: FAILED!\n\n");
    }
  else
    {
      ui_print("Tests for julian to tibetan date conversion: OK!\n\n");
    }
  main_menu();
  return;
}

void get_tibdata_for_date()
{
  astro_system *sys;
  sys = get_astro_system(PHUGPA);
  long int day, month, year;
  long int jd;
  tib_day *td;
  ui_print("Enter western date (dd mm yyyy): ");
  scanf ("%ld %ld %ld", &day, &month, &year);
  ui_print("\n");
  jd = wd_to_jd(day, month, year);
  ui_print("jd: %ld\n", jd);
  td = new_tib_day();
  get_td_from_jd(jd, td, sys);
  print_tib_day_r(td);
  free(td);
  free(sys);
  main_menu();
  return;
}


