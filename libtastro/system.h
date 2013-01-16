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

#ifndef SYSTEM_H
#define SYSTEM_H

// first the different systems
#define PHUGPA 1
#define TSURPHU 2
#define SHERAB_LING 3
#define ERROR_CORRECTION 4
#define MODERN_KALACAKRA 5

// the epoch structure for traditional systems
typedef struct epoch
{
    long int year; // the western calendar year
    long int eyr_a;  // Intercalation index
    long int ril_a;
    long int ril_b;
    long int spz_c;
    long int spz_b;
    long int spz_f;
    long int spz_j;
    long int rahupart; // 230 fraction for Rahu cycle
    long int dragkadd; // This is added for "drag po'i rkang 'dzin".
    long int meradd; // Figures to be added for planets for the
    long int venadd; // nyin zhag dal ba. These are days of cycle.
    long int maradd;
    long int jupadd;
    long int satadd;
    long int zlasho; // index of the intercalary months, see KTC p.51. we always consider that the second index is the first + 1, and this long int is the first
    long int gzad[6]; // mean weekday at new moon at the epoch: see KTC p.18
    long int nyid[6]; // mean sun of the epoch's month: see KTC p.21
    
} epoch;

// the system structure
typedef struct astro_system
{
    unsigned char type; // PHUGPA, TSURPHU, etc.
    long int sun_f;
    long int nyi_drup_const[5];
     long int nyi_long_const[6]; // constant to calculate lunar day solar longitude, "nyi ma'i longs spyod", see KTC p. 23
    epoch *epoch;
} astro_system;

astro_system *get_astro_system(unsigned char type);
astro_system *new_system();

#endif
