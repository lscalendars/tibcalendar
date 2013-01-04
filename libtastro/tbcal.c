#include <stdio.h>
#include "tbcal.h"
#include "tbday.h"
#include "system.h"
#include "jd.h" // ?

void print_tib_cal(FILE *f, int year, astro_system *asys)
 {
 tib_month *month;
 long int zd[2];
 long int zeromthfg;
 long int adj_mth;
 /*    Finding the month   */
 // the idea here is to get the data for 1 month of the year, directly in Tibetan date, not passing
 // through julian date
// so first we get the true month for this month:
zla_dag (asys->epoch, (long int) year, 1, zd);
// we adjust the month:
adj_mth = adj_zla (1, zd, &zeromthfg, asys);
// now zd is set, we check if we are at the end of a 
 // loop on months to check if a new year happened
 month = next_month(month);
 // loop on days to check when the new year happened
 // print year data
 // print month data
 // print day data on a loop on days, with a check for a change of month (and year)
 }
