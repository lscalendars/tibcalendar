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

//   //get_tpdata(2455740L, td); // 27/6/2011
  //get_tpdata(2455738L, td); // 27/6/2011
  //get_tpdata(2000487L, td); // 13/01/765, duplicated day followed by ommited day
// 13 january 765 : duplicated day followed by ommited day  
#include "tbday.h"

int check_tibday_phugpa_individuals();
int compare_tib_day(tib_day *td1, tib_day *td2);
int compare_tib_month(tib_month *tm1, tib_month *tm2);
int compare_lst(long int *l1, long int *l2, int length);
void get_tibday_const(int i, tib_day *td);
int check_tibday();
void aux_set_lst_5(long int lst[5], long int n0, long int n1, long int n2, long int n3, long int n4);
void aux_set_lst_2(long int lst[2], long int n0, long int n1);
int check_tibday_phugpa_individual(int i, long int jd, tib_day *td1, tib_day *tb2, astro_system *sys);
