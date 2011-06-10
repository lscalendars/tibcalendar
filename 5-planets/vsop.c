/*********************************************************************************
License for Kcal - Kalacakra calendar software

Copyright (c) 2011 Edward Henning

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

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "jd.h"

long int gregd2jul ( long int, long int, long int );
void jul2date ( long int, struct tmst * );
double get_deltaT ( double );

void UTET ( int, int, int, int, int, int, double *, double * );

// Global variables

double T, T2, T3, T4, U, U2, U3, U4, U5, U6, U7;
double rad;
double TwoPi = 6.283185307179586476;
          //   3.14159265358979323846
double SunL, SunR; // Solar longitude and radius vector.

extern double MerHL, MerHB, MerHR; // Mercury, heliocentric long, lat, radius.
extern double MerHX, MerHY, MerHZ; // Mercury, heliocentric x, y, z.

extern double VenHL, VenHB, VenHR; // Venus, heliocentric long, lat, radius.
extern double VenHX, VenHY, VenHZ; // Venus, heliocentric x, y, z.

extern double EarHL, EarHB, EarHR; // Earth, heliocentric long, lat, radius.
extern double EarHX, EarHY, EarHZ; // Earth, heliocentric x, y, z.

extern double MarHL, MarHB, MarHR; // Mars, heliocentric long, lat, radius.
extern double MarHX, MarHY, MarHZ; // Mars, heliocentric x, y, z.

extern double JupHL, JupHB, JupHR; // Jupiter, heliocentric long, lat, radius.
extern double JupHX, JupHY, JupHZ; // Jupiter, heliocentric x, y, z.

extern double SatHL, SatHB, SatHR; // Saturn, heliocentric long, lat, radius.
extern double SatHX, SatHY, SatHZ; // Saturn, heliocentric x, y, z.

extern double MerGL, MerGB, MerGR; // Mercury, geocentric long, lat, radius.
double VenGL, VenGB, VenGR; // Venus, geocentric long, lat, radius.
//double MarGL, MarGB, MarGR; // Mars, geocentric long, lat, radius. - not used

double set_time_data ( double );
void dis_test ( void );
void make_geocent ( double, double, double, double *, double *, double * );
void clc_merlon ( double );
void clc_merrct ( double );
void clc_venlon ( double );
void clc_venrct ( double );
void clc_earlon ( double );
void clc_earrct ( double );
void clc_marlon ( double );
void clc_marrct ( double );
void clc_juplon ( double );
void clc_juprct ( double );
void clc_satlon ( double );
void clc_satrct ( double );
void run_heliotests ( void );
void run_geotest ( void );
double modfeh ( double, double );

int main(void)
{
  int i;

  rad = 3.141592653589793 / 180.0;

  run_heliotests (); // Main helicentric test routine
//run_geotest ();

//for ( i = 0; i < 4000; ++i )
// {
//    UTET ( i, 12, 19, 12, 0, 0, &JD, &ET );
//    set_time_data ( JD );
//    test_lon ();
//    printf ( "Maxdif = %.10f\n\n", maxdif );
// }
  } // main ()

double set_time_data ( double JD )
  {
    T = ( JD - 2451545.0 ) / 36525.0;
    printf ( "Julian date = %.4f, T = %.11f\n", JD, T );
    T2 = T * T;
    T3 = T * T2;
    T4 = T2 * T2;
    U = ( JD - 2451545.0 ) / 3652500.0;
    U2 = U * U;
    U3 = U2 * U;
    U4 = U2 * U2;
    U5 = U3 * U2;
    U6 = U3 * U3;
    U7 = U4 * U3;
  } // set_time_data

double modfeh ( double x, double y )
  {
    double N;
    if ( x > 0.0 )
      {
        if ( x < y )
          return ( x );
        N = floor ( x / y );
        return ( x - N * y );
      }
    if ( -x < y )
      return ( x );
    N = ceil ( x / y );
    return ( x - N * y );
  } // END - modfeh ()

void run_geotest ( void )
  {
    double JD, ET;
    double VenGX, VenGY, VenGZ;
    double VenGLam, VenGBet, VenGR, DeltaD, Tau, Tau_T;

    UTET ( 1992, 12, 20, 0, 0, 0, &JD, &ET ); // ALL OK

    set_time_data ( JD );  // MUST BE FIRST!!

    printf ( "T = %f\n", T );

    clc_venrct ( T / 10.0 );
    clc_earrct ( T / 10.0 );

    VenGX = VenHX - EarHX;
    VenGY = VenHY - EarHY;
    VenGZ = VenHZ - EarHZ;

// CORRECT FOR LIGHT-TIME AND ABERRATION TOGETHER: (Meeus 225)

    DeltaD = sqrt ( VenGX * VenGX + VenGY * VenGY + VenGZ * VenGZ );

    printf ( "DeltaD = %.6f\n", DeltaD );

    Tau = DeltaD * 0.0057755183;

    printf ( "Tau = %.7f days\n", Tau );

    Tau_T = Tau / 36525.0;

    printf ( "Tau_T = %.7f Julian centuries\n", Tau_T );

    clc_venrct ( (T-Tau_T) / 10.0 );
    clc_earrct ( (T-Tau_T) / 10.0 );

    VenGX = VenHX - EarHX;
    VenGY = VenHY - EarHY;
    VenGZ = VenHZ - EarHZ;

    printf ( "Venus, geometric X = %.6f, Y = %.6f, Z = %.6f\n",
             VenGX, VenGY, VenGZ );

    VenGLam = atan2 ( VenGY, VenGX ) * 360.0 / TwoPi;
    if ( VenGLam < 0.0 )
      VenGLam = VenGLam + 360.0;
    VenGBet = atan2 ( VenGZ, sqrt ( VenGX * VenGX + VenGY * VenGY ) )
             * 360.0 / TwoPi;
    VenGR = sqrt ( VenGX * VenGX + VenGY * VenGY + VenGZ * VenGZ );

    printf ( "Lambda = %.6f, Beta = %.6f, R = %.6f\n",
             VenGLam, VenGBet, VenGR );

// SO FAR, CORRECTED FOR LIGHT-TIME AND ABERRATION.

//    clc_earlon ( T / 10.0 );
//    HL = EarHL; HB = EarHB; HR = EarHR;
//    HX = EarHX; HY = EarHY; HZ = EarHZ;

//extern double VenHL, VenHB, VenHR; // Venus, heliocentric long, lat, radius.
//extern double VenHX, VenHY, VenHZ; // Venus, heliocentric x, y, z.

//extern double EarHL, EarHB, EarHR; // Earth, heliocentric long, lat, radius.
//extern double EarHX, EarHY, EarHZ; // Earth, heliocentric x, y, z.
//void clc_venlon ( double );
//void clc_earlon ( double );
//void clc_earrct ( double );

  }
