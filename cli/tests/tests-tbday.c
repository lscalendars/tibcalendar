#include"tests-tbday.h"
#include"tbday.h"
#include"../ui.h"
#include"utils.h"
#include"system.h"
#include <stdlib.h>

/* global julian day test routine */

int
check_tibday ()
{
  if (check_tibday_phugpa_individuals () == 0)
    {
      return 0;
    }
  return 1;
}

int
check_tibday_phugpa_individuals ()
{
  int res;
  tib_day *td1 = new_tib_day ();
  tib_day *td2 = new_tib_day ();
  astro_system *sys = get_astro_system(PHUGPA);
  res = check_tibday_phugpa_individual (1, 2000487L, td1, td2, sys);
  if (!res)
    {
      free_tib_day (td1);
      free_tib_day (td2);
      return 0;
    }
  res = check_tibday_phugpa_individual (2, 2455738L, td1, td2, sys);
  if (!res)
    {
      free_tib_day (td1);
      free_tib_day (td2);
      return 0;
    }
  free_tib_day (td1);
  free_tib_day (td2);
  free(sys);
  return 1;
}

// i: the number of the result tibetan day
// jd: the julian day it corresponds to
// td1 and td2: two tibetan days to play with
int
check_tibday_phugpa_individual (int i, long int jd, tib_day * td1,
				tib_day * td2, astro_system *sys)
{
  get_tibday_const (i, td1);
  get_td_from_jd (jd, td2, sys);
  return (compare_tib_day (td2, td1));
}

// Function returning 0 if different and 1 if equal

int
compare_tib_day (tib_day * td1, tib_day * td2)
{
  if (!compare_tib_month (td1->month, td2->month))
    {
      printf ("error: found month data:\n");
      print_tib_month (td1->month);
      printf ("when it should be:\n");
      print_tib_month (td2->month);
      return 0;
    }
  if (td1->gd != td2->gd || td1->tt != td2->tt || td1->ommited != td2->ommited
      || td1->duplicated != td2->duplicated
      || !compare_lst (td1->nyibar, td2->nyibar, 5)
      || !compare_lst (td1->nyidag, td2->nyidag, 5)
      || !compare_lst (td1->gzadag, td2->gzadag, 5))
    {
      printf ("error: found day data:\n");
      print_tib_day (td1);
      printf ("when it should be:\n");
      print_tib_day (td2);
      return 0;
    }
  return 1;
}

int
compare_tib_month (tib_month * tm1, tib_month * tm2)
{
  if (tm1->year->year != tm2->year->year || tm1->month != tm2->month
      || tm1->month_type != tm2->month_type
      || tm1->start_gd != tm2->start_gd)
    return 0;
  if (!compare_lst (tm1->rilcha, tm2->rilcha, 2)
      || !compare_lst (tm1->true_month, tm2->true_month, 2)
      || !compare_lst (tm1->gzadru, tm2->gzadru, 5)
      || !compare_lst (tm1->nyidru, tm2->nyidru, 5))
    return 0;
  return 1;
}

// function comparing a list

int
compare_lst (long int *l1, long int *l2, int length)
{
  unsigned char i = 0;
  while (i < length)
    {
      if (l1[i] != l2[i])
	return 0;
      i++;
    }
  return 1;
}

// a function to get the constant tib_days to compare
void
get_tibday_const (int i, tib_day * td)
{
  switch (i)
    {
    case 1:
      // 13 january 765: duplicated day followed by ommited day  
      td->month->year->year = 765; // TODO: fix it!
      td->month->month = 1;
      td->month->month_type = FIRST_OF_DOUBLE;
      aux_set_lst_2 (td->month->true_month, 21829, 48);
      td->month->zero_month_flag = 0;
      td->month->start_gd = 2000471;
      aux_set_lst_2 (td->month->rilcha, 17, 92);
      aux_set_lst_5 (td->month->gzadru, 5, 46, 36, 4, 340);
      aux_set_lst_5 (td->month->nyidru, 20, 53, 17, 0, 6);
      td->gd = 2000488;
      td->tt = 17;
      td->ommited = NEXT_OMMITED;
      td->duplicated = FIRST_OF_DUPLICATED;
      aux_set_lst_5 (td->nyibar, 22, 7, 30, 0, 0);
      aux_set_lst_5 (td->nyidag, 22, 12, 30, 0, 0);
      aux_set_lst_5 (td->gzadag, 2, 0, 23, 1, 232);
      break;
    case 2:
      // 2455738: second of a duplicated day
      td->month->year->year = 2011; // TODO: fix it!
      td->month->month = 4;
      aux_set_lst_2 (td->month->true_month, 37245, 60);
      td->month->zero_month_flag = 0;
      td->month->start_gd = 2455712;
      aux_set_lst_2 (td->month->rilcha, 4, 10);
      aux_set_lst_5 (td->month->gzadru, 4, 18, 21, 0, 558);
      aux_set_lst_5 (td->month->nyidru, 2, 29, 6, 1, 41);
      td->gd = 2455738;
      td->tt = 24;
      td->ommited = NORMAL;
      td->duplicated = SECOND_OF_DUPLICATED;
      aux_set_lst_5 (td->nyibar, 4, 13, 52, 5, 1);
      aux_set_lst_5 (td->nyidag, 4, 20, 21, 2, 64);
      aux_set_lst_5 (td->gzadag, 0, 2, 43, 3, 390);
      break;
    default:
      break;
    }
}

void
aux_set_lst_5 (long int lst[5], long int n0, long int n1, long int n2,
	       long int n3, long int n4)
{
  lst[0] = n0;
  lst[1] = n1;
  lst[2] = n2;
  lst[3] = n3;
  lst[4] = n4;
}

void
aux_set_lst_2 (long int lst[2], long int n0, long int n1)
{
  lst[0] = n0;
  lst[1] = n1;
}
