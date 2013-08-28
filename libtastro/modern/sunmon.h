/*********************************************************************************
Licence for Kcal - Kalacakra calendar software

Copyright (c) 2011-2013 Edward Henning

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

#ifndef _SUNMON_H_
#define _SUNMON_H_

#ifdef __cplusplus
extern "C" {
#endif

double clc_earlon ( double );
void clc_earrct ( double );
double clc_monlon ( double );
void clc_data ( void );
double modfeh ( double, double );
double dms ( double, double, double );
double getmeanSunmeannewMoon ( double juld );
double getmeanSun ( double juld );
double get_deltaT ( double JDate );
void sun_mon ( double jd, double *sun, double *moon);
double com3f60 ( int, int, int );

extern double cmpb[2645];
extern double cper[33256];
extern double EaX0P[1007][3];
extern double EaX1P[600][3];
extern double EaX2P[248][3];
extern double EaX3P[46][3];
extern double EaX4P[20][3];
extern double EaX5P[7][3];
extern double EaY0P[1007][3];
extern double EaY1P[600][3];
extern double EaY2P[248][3];
extern double EaY3P[46][3];
extern double EaY4P[20][3];
extern double EaY5P[7][3];
extern double EaZ0P[178][3];
extern double EaZ1P[97][3];
extern double EaZ2P[47][3];
extern double EaZ3P[11][3];
extern double EaZ4P[5][3];
extern double EaL0P[559][3];
extern double EaL1P[341][3];
extern double EaL2P[142][3];
extern double EaL3P[22][3];
extern double EaL4P[11][3];
extern double EaL5P[5][3];
extern double EaB0P[184][3];
extern double EaB1P[99][3];
extern double EaB2P[49][3];
extern double EaB3P[11][3];
extern double EaB4P[5][3];
extern double EaR0P[526][3];
extern double EaR1P[292][3];
extern double EaR2P[139][3];
extern double EaR3P[27][3];
extern double EaR4P[10][3];
extern double EaR5P[3][3];
extern double fmpb[2645][5];
extern double fper[33256][5];
extern double SvP[218][6];
extern double SivP[244][3];
extern double SiivP[200][3];
extern double SiiivP[200][3];

#ifdef __cplusplus
}
#endif

#endif
