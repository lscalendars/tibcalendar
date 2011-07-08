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
