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

#ifndef TBPLANETS_H
#define TBPLANETS_H
#include "tbday.h"

#define INNER_PLANET 1
#define OUTER_PLANET 0



tib_planet_data * get_planets_data (long int jd, long int tt, long int zd0, long int nyibar[6], astro_system *sys);
void get_geo_l(long int plandaldag[6], long int plandalbar[6], long int dragkang[6], long int planmurdag[6], long int planfrac, long int *planbye1, long int *planbye2, long int *plandom1, long int *plandom2, unsigned char plantquo1[15], unsigned char plantquo2[15], unsigned char type, long int rkang_frac);
void get_true_slow_l (long int pdaldag[6], long int pdalbar[6], long int pfac[6], long int pbye[6], long int pdom[6], long int frac);
void get_mean_slow_l (long int lst[5], long int dat, long int cyc, long int frac);
void get_rahu_l (epoch *epch, long int m, long int tt, long int rahudong[5]);

#endif
