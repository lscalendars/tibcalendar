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
