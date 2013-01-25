#ifndef TBMONTH_H
#define TBMONTH_H

#include "system.h" // for epoch  
#include "tbstructures.h"

void zla_dag (const epoch *epch, long int y, long int m, long int zd[2]);
void next_zla_dag (long int zd[2]);
void get_month_data (astro_system *asys, long int zd0, long int rilcha[2], long int nyidru[6], long int gzadru[6]);
long int adj_zla (long int tm, long int zd[2], 	unsigned char *zeromthfg, astro_system *sys);
void tib_month_next (tib_month* month, astro_system *asys);
void find_month_and_year (long int jd, astro_system *sys, tib_month *month);

#endif
