#ifndef TBDAY_H
#define TBDAY_H

#include "system.h" // for epoch  
#include "tbstructures.h"

void nyi_dag_and_gza_dag ( long int nyibar[6], long int tsebar[6], long int rilcha[2], long int tt, long int nyidag[6], long int gzadag[6]) ;
long int spi_zag (epoch *epch, long int cur_mth, long int tt, long int sd);
int get_tt_data(astro_system *asys, long int cur_mth, long int gzadru[6], long int nyidru[6], long int rilcha[6], long int tt, long int nyidag[6], long int gzadag[6], long int nyibar[6]);
void get_td_from_jd (long int jd, tib_day *td, astro_system *sys);
tib_day*get_tib_day_from_tib_date (int y, int m, int d, astro_system *asys, unsigned char m_f_o_s, unsigned char d_f_o_s);
void tib_day_next (tib_day *td, astro_system *asys);

#endif
