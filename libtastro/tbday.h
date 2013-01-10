#ifndef TBDAY_H
#define TBDAY_H

#include "system.h" // for epoch  
#include "tbstructures.h"

void zla_dag (const epoch *epch, long int y, long int m, long int zd[2]);
void next_zla_dag (long int zd[2]);
void nyi_dag_and_gza_dag ( long int nyibar[6], long int tsebar[6], long int rilcha[2], long int tt, long int nyidag[6], long int gzadag[6]) ;
long int spi_zag (epoch *epch, long int cur_mth, long int tt, long int sd);
int get_tt_data(epoch *epch, long int cur_mth, long int gzadru[6], long int nyidru[6], long int rilcha[6], long int tt, long int nyidag[6], long int gzadag[6], long int nyibar[6]);
void get_td_from_jd (long int jd, tib_day *td, astro_system *sys);
void get_month_data (epoch *epch, long int zd0, long int rilcha[2], long int nyidru[6], long int gzadru[6]);
long int adj_zla (long int tm, long int zd[2], 	unsigned char *zeromthfg, astro_system *sys);
tib_day*get_tib_day_from_tib_date (int y, int m, int d, astro_system *asys, unsigned char m_f_o_s, unsigned char d_f_o_s);
void tib_month_next (tib_month* month, astro_system *asys);
void tib_day_next (tib_day *td, astro_system *asys);

#endif
