/*
License for Tibastro - Tibetan almanach calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TBDAY_H
#define TBDAY_H

#include "system.h" // for epoch  
#include "tbstructures.h"

void nyi_dag_and_gza_dag ( long int nyibar[6], long int tsebar[6], long int rilcha[2], long int tt, long int nyidag[6], long int gzadag[6], long int sun_f) ;
long int spi_zag (epoch *epch, long int cur_mth, long int tt, long int sd);
int get_tt_data(astro_system *asys, long int cur_mth, long int gzadru[6], long int nyidru[6], long int rilcha[6], long int tt, long int nyidag[6], long int gzadag[6], long int nyibar[6]);
void get_td_from_jd (long int jd, tib_day *td, astro_system *sys);
tib_day*get_tib_day_from_tib_date (int y, int m, int d, astro_system *asys, unsigned char m_f_o_s, unsigned char d_f_o_s);
void tib_day_next (tib_day *td, astro_system *asys);

#endif
