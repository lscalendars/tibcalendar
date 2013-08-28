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

#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include "sunmon.h"
#include "jd.h"

// TODO: use #defines
static double rad = 1.745329251994329576923691e-2; // degrees to radians, from GAL
static double cpi= 3.141592653589793238462643; // From GAL
static double TwoPi = 6.283185307179586476925287;
static double pis2;
static double rad2deg = 57.29577951308232087679815; // = 360/2Pi
static double rad2as = 206264.8062470963551564734; // from GAL
static double deg2rad = 1.745329251994329576923691e-2; // degrees to radians, from GAL, pi/180
static double as2rad = 4.848136811095359935899141e-6;

double EarHL, EarHB, EarHR; // Earth, heliocentric long, lat, radius.
double EarHX, EarHY, EarHZ; // Earth, heliocentric x, y, z.
double MonL;

double mol, sol;
double meansol, sun_minus_mnode, moon_minus_sun;

int datadone = 0;

double w[4][5];
double zeta[5]; 
double eart[5];
double peri[5];
double del[5][5];
double p[9][5];
// Moon constants:
double nu, g, e, np, ep, alpha, am, dtasm, xa; // I added xa - not in Fortran code??
// Corrections to the constants Nu, Gamma, E, n', e':
double delnu, delg, dele, delnp, delep;
double bp[5][2] = { {0.311079095, -0.103837907},
                   {-0.4482398e-2, 0.6682870e-3},
                   {-0.110248500e-2, -0.129807200e-2},
                   {0.1056062e-2, -0.1780280e-3},
                   {+0.50928e-4, -0.37342e-4} };
double Dprec = -0.29965; // Chap2002, p.3: minutes of arc per century
double a405 = 384747.9613701725;
double aelp = 384747.980674318;

double getmeanSun ( double juld )
  {
    double T, TM, TM2, TM3, TM4, TM5, X, delt, SunL;

// From sun_mon:
    T = (juld - 2451545.0);
    delt = get_deltaT (juld) / 86400.0; // This converts seconds to fraction of a day
    T = (T + delt) / 36525.0;  // This is now in Julian centuries.

    TM = T / 10.0; // Measured in Julian millenia
    TM2 = TM * TM;
    TM3 = TM * TM2;
    TM4 = TM2 * TM2;
    TM5 = TM * TM4;

// Mean Sun, Astronomical Algorithms, p.183: Should compare with p.212

    X = 280.4664567 + 360007.6982779 * TM;
    X = modfeh ( X, 360.0 );
    SunL = X + 0.03032028 * TM2
             + TM3 / 49931.0
             - TM4 / 15300.0
             - TM5 / 2000000.0;
    SunL = modfeh ( SunL, 360.0 );
    if ( SunL < 0.0 )
      SunL = SunL + 360.0;
    return ( SunL );
  } // END - getmeanSun

double getmeanSunmeannewMoon ( double juld )
  {
    double T, T2, T3, T4, X, delt, SunL;
    double D; // mean elongation of the Moon
    int more = 1;

    do {
         T = (juld - 2451545.0);
         delt = get_deltaT (juld) / 86400.0; // This converts seconds to fraction of a day
         T = (T + delt) / 36525.0;  // This is now in Julian centuries.
         T2 = T * T;
         T3 = T * T2;
         T4 = T2 * T2;

// Mean Moon elongation, Astronomical Algorithms, p.338: 

          X = 297.8501921 + 445267.1114034 * T;
          X = modfeh ( X, 360.0 );
          D = X - 0.0018819 * T2
             + T3 / 545868.0
             - T4 / 113065000.0;
          D = modfeh ( D, 360.0 );
          if ( D > 180.0 )
            D = D - 360.0;
          else if ( D < -180.0 )  
            D = D + 360.0;
          if ( D < 0.00001 && D > -0.00001 ) // order of thousandths of a second!
            more = 0;
          else
            // was 29.5 in EH's code, this value comes from Wikipedia
            juld = juld - D * ( 29.530589 / 360.0 ); 
        } while ( more );
    SunL = getmeanSun ( juld );
    return ( SunL );
  } // END - getmeanSunmeannewMoon

// Find delta T, returns number of seconds
// Method from: http://eclipse.gsfc.nasa.gov/SEcat5/deltatpoly.html
double get_deltaT ( double JDate )
  {
    double y;
    double u, u2, u3, u4, u5, u6;
    double dT;
    double t, t2, t3, t4, t5, t6, t7;

    int wy, wm, wd, wdow;

    // deltaT could be a little more precise if we substract jd_factor, but it seems
    // totally unnecessary

    jd_to_wd ((long int) floor(JDate + 0.5), &wd, &wm, &wy, &wdow);
    
    y = (double) wy + ((double) ( wm - 1 ) )/ 12.0 + ((double) (wd - 1 ))/ 360.0;

    if ( y < -500.0 )
      {
        u = (y - 1820.0 )/100.0;
        dT = -20.0 + 32.0 * u * u;
      }
    else if ( y >= -500.0 && y < 500.0 )
      {
        u = y / 100.0;
        u2 = u * u;
        u3 = u2 * u;
        u4 = u2 * u2;
        u5 = u2 * u3;
        u6 = u3 * u3;
        dT = 10583.6 - 1014.41 * u + 33.78311 * u2 - 5.952053 * u3
              - 0.1798452 * u4 + 0.022174192 * u5 + 0.0090316521 * u6;
      }
    else if ( y >= 500.0 && y < 1600.0 )
      {
        u = ( y - 1000.0 ) / 100.0;
        u2 = u * u;
        u3 = u2 * u;
        u4 = u2 * u2;
        u5 = u2 * u3;
        u6 = u3 * u3;
        dT = 1574.2 - 556.01 * u + 71.23472 * u2 + 0.319781 * u3
             - 0.8503463 * u4 - 0.005050998 * u5 + 0.0083572073 * u6;
      }
    else if ( y >= 1600.0 && y < 1700.0 )
      {
        t = y - 1600.0;
        t2 = t * t;
        t3 = t * t2;
        dT = 120.0 - 0.9808 * t - 0.01532 * t2 + t3 / 7129.0;
      }
    else if ( y >= 1700.0 && y < 1800.0 )
      {
        t = y - 1700.0;
        t2 = t * t;
        t3 = t * t2;
        t4 = t2 * t2;        
        dT = 8.83 + 0.1603 * t - 0.0059285 * t2 + 0.00013336 * t3 - t4 / 1174000.0;
      }
    else if ( y >= 1800.0 && y < 1860.0 )
      {
        t = y - 1800.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        t5 = t2 * t3;
        t6 = t3 * t3;
        t7 = t3 * t4;
        dT = 13.72 - 0.332447 * t + 0.0068612 * t2 + 0.0041116 * t3 - 0.00037436
             * t4 + 0.0000121272 * t5 - 0.0000001699 * t6 + 0.000000000875 * t7;
      }
    else if ( y >= 1860.0 && y < 1900.0 )
      {
        t = y - 1860.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        t5 = t2 * t3;
        dT = 7.62 + 0.5737 * t - 0.251754 * t2 + 0.01680668 * t3
                - 0.0004473624 * t4 + t5 / 233174.0;
      }
    else if ( y >= 1900.0 && y < 1920.0 )
      {
        t = y - 1900.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        dT = -2.79 + 1.494119 * t - 0.0598939 * t2 + 0.0061966 * t3 - 0.000197 * t4;
      }
    else if ( y >= 1920.0 && y < 1941.0 )
      {
        t = y - 1920.0;
        t2 = t * t;
        t3 = t2 * t;
        dT = 21.20 + 0.84493 * t - 0.076100 * t2 + 0.0020936 * t3;
      }
    else if ( y >= 1941.0 && y < 1961.0 )
      {
        t = y - 1950.0;
        t2 = t * t;
        t3 = t2 * t;
        dT = 29.07 + 0.407 * t - t2 / 233.0 + t3 / 2547.0;
      }
    else if ( y >= 1961.0 && y < 1986.0 )
      {
        t = y - 1975.0;
        t2 = t * t;
        t3 = t2 * t;
        dT = 45.45 + 1.067 * t - t2 / 260.0 - t3 / 718.0;
      }
    else if ( y >= 1986.0 && y < 2005.0 )
      {
        t = y - 2000.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        t5 = t2 * t3;
        dT = 63.86 + 0.3345 * t - 0.060374 * t2 + 0.0017275 * t3 +
             0.000651814 * t4 + 0.00002373599 * t5;
      }
    else if ( y >= 2005.0 && y < 2050.0 )
      {
        t = y - 2000.0;
        t2 = t * t;
        dT = 62.92 + 0.32217 * t + 0.005589 * t2;
      }
    else if ( y >= 2050.0 && y < 2150.0 )
      {
        dT = -20.0 + 32.0 * ((y-1820.0)/100.0) * ((y-1820.0)/100.0)
             - 0.5628 * (2150.0 - y);
      }
    else if ( y >= 2150.0 )
      {
        u = ( y - 1820.0 ) / 100.0;
        dT = -20 + 32 * u * u;
      }
    return (dT);
  } // END - get_deltaT

// Calculate solar and lunar longitudes.
// In main routine, all times are in UT.
// Here, they have to be converted to ET, TT, or whatever is the current fad.
void sun_mon ( double jd, double *sol, double *mol)
  {
    double delt, T;
    double Tau, Tau_T, DeltaD;
    double EarHL;

    T = ( jd - 2451545.0 ); // J2000.0 epoch
    delt = get_deltaT ( jd ) / 86400.0;  // converts from seconds to fraction of a day
    T = ( T + delt ) / 36525.0;   // This is now in Julian centuries.

// First, do Sun:

    // this gives the earth heliocentric rectangular coordinates
    clc_earrct ( T / 10.0 ); // Needs Julian millenia

// Correct for light-time and aberration together: (Meeus 225)

    // DeltaD is the distance from Earth to Sun
    DeltaD = sqrt ( EarHX * EarHX + EarHY * EarHY + EarHZ * EarHZ );
    // 0.0057755183 is the time (in days) taken by light to travel 1 Astronomical
    // Unit (1 AU = 149.597.871 km)
    // Tau is the time light takes to travel from Sun to Earth, 
    Tau = DeltaD * 0.0057755183;
    // Tau_T is the same time expressed in julian years
    Tau_T = Tau / 36525.0;
    // T - Tau_T is the time at which the event we see on Earth happened (due to light speed)
    // EarHL is the Earth longitude as seen from the sun, expressed in rad
    EarHL = clc_earlon ( (T-Tau_T) / 10.0 ); // expressed in millenia too
    // EarHL * 360 / TwoPi is EarHL in degrees (EarHLd)
    // EarHLd + 180 = EarHLd -180 = the longitude of the Sun (a little drawing might help)
    *sol = EarHL * 360.0 / TwoPi + 180.0;
    if ( *sol > 360.0 )
      *sol = *sol - 360.0;

// Now do Moon:

    *mol = clc_monlon ( T );  // Result into MonL; needs Julian centuries.
    if ( *mol > 360.0 )
      *mol = *mol - 360.0;
  } // END - sun_mon ()

// Earth longitude, expressed in rad
double clc_earlon ( double t ) // t is Julian millenia, ET.
  {
    double SL;
    double L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3, R4, R5;
    double D, Alpha, t2, t3, t4, t5;
    int i;
    double EarHL/*, EarHB*/;

    t2 = t * t;
    t3 = t * t2;
    t4 = t2 * t2;
    t5 = t2 * t3;

// First, longitude:

    L0 = 1.75347045673;         // Different for each planet
    SL = 0.0;
    for ( i = 558; i >= 1; --i )
      {
        Alpha = EaL0P[i][1] + EaL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L0 = L0 + SL;

    L1 = 6283.31966747491;      // Different for each planet
    SL = 0.0;
    for ( i = 340; i >= 1; --i )
      {
        Alpha = EaL1P[i][1] + EaL1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L1 = L1 + SL;

    L2 = 0.00052918870;
    SL = 0.0;
    for ( i = 141; i >= 1; --i )
      {
        Alpha = EaL2P[i][1] + EaL2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L2 = L2 + SL;

    SL = 0.0;
    for ( i = 21; i >= 0; --i )
      {
        Alpha = EaL3P[i][1] + EaL3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L3 = SL;

    SL = 0.0;
    for ( i = 10; i >= 0; --i )
      {
        Alpha = EaL4P[i][1] + EaL4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L4 = SL;

    SL = 0.0;
    for ( i = 4; i >= 0; --i )
      {
        Alpha = EaL5P[i][1] + EaL5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L5 = SL;

    EarHL = L0 + L1 * t + L2 * t2 + L3 * t3 + L4 * t4 + L5 * t5;

    EarHL = modfeh ( EarHL, TwoPi );
    if ( EarHL < 0.0 )
      EarHL = EarHL + TwoPi;

/*

// Now, latitude:

    SL = 0.0;
    for ( i = 183; i >= 0; --i )
      {
        Alpha = EaB0P[i][1] + EaB0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaB0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B0 = SL;

    SL = 0.0;
    for ( i = 98; i >= 0; --i )
      {
        Alpha = EaB1P[i][1] + EaB1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaB1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B1 = SL;

    SL = 0.0;
    for ( i = 48; i >= 0; --i )
      {
        Alpha = EaB2P[i][1] + EaB2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaB2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B2 = SL;

    SL = 0.0;
    for ( i = 10; i >= 0; --i )
      {
        Alpha = EaB3P[i][1] + EaB3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaB3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B3 = SL;

    SL = 0.0;
    for ( i = 4; i >= 0; --i )
      {
        Alpha = EaB4P[i][1] + EaB4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaB4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B4 = SL;

    EarHB = B0 + B1 * t + B2 * t2 + B3 * t3 + B4 * t4;

// Next, heliocentric radius vector:

    R0 = 1.00013988799;         // Different for each planet
    SL = 0.0;
    for ( i = 525; i >= 1; --i ) // index 0 is R0, above
      {
        Alpha = EaR0P[i][1] + EaR0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaR0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R0 = R0 + SL;

    SL = 0.0;
    for ( i = 291; i >= 0; --i )
      {
        Alpha = EaR1P[i][1] + EaR1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaR1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R1 = SL;

    SL = 0.0;
    for ( i = 138; i >= 0; --i )
      {
        Alpha = EaR2P[i][1] + EaR2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaR2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R2 = SL;

    SL = 0.0;
    for ( i = 26; i >= 0; --i )
      {
        Alpha = EaR3P[i][1] + EaR3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaR3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R3 = SL;

    SL = 0.0;
    for ( i = 9; i >= 0; --i )
      {
        Alpha = EaR4P[i][1] + EaR4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaR4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R4 = SL;

    SL = 0.0;
    for ( i = 2; i >= 0; --i )
      {
        Alpha = EaR5P[i][1] + EaR5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaR5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R5 = SL;
    EarHR = R0 + R1 * t + R2 * t2 + R3 * t3 + R4 * t4 + R5 * t5;
    
    */
    
    return EarHL;
  } // END - clc_earlon ()

// Earth rectangular coordinates:
void clc_earrct ( double t )  // t is Julian millenia
  {
    double X0, X1, X2, X3, X4, X5;
    double Y0, Y1, Y2, Y3, Y4, Y5;
    double Z0, Z1, Z2, Z3, Z4;
    double D, Alpha, t2, t3, t4, t5;
    int i;

    t2 = t * t;
    t3 = t * t2;
    t4 = t2 * t2;
    t5 = t2 * t3;

    X0 = 0.0;
    for ( i = 1006; i >= 0; --i )
      {
        Alpha = EaX0P[i][1] + EaX0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaX0P[i][0] * cos ( Alpha );
        X0 = X0 + D;
      }

    X1 = 0.0;
    for ( i = 599; i >= 0; --i )
      {
        Alpha = EaX1P[i][1] + EaX1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaX1P[i][0] * cos ( Alpha );
        X1 = X1 + D;
      }

    X2 = 0.0;
    for ( i = 247; i >= 0; --i )
      {
        Alpha = EaX2P[i][1] + EaX2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaX2P[i][0] * cos ( Alpha );
        X2 = X2 + D;
      }

    X3 = 0.0;
    for ( i = 45; i >= 0; --i )
      {
        Alpha = EaX3P[i][1] + EaX3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaX3P[i][0] * cos ( Alpha );
        X3 = X3 + D;
      }

    X4 = 0.0;
    for ( i = 19; i >= 0; --i )
      {
        Alpha = EaX4P[i][1] + EaX4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaX4P[i][0] * cos ( Alpha );
        X4 = X4 + D;
      }

    X5 = 0.0;
    for ( i = 6; i >= 0; --i )
      {
        Alpha = EaX5P[i][1] + EaX5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaX5P[i][0] * cos ( Alpha );
        X5 = X5 + D;
      }

    EarHX = X0 + X1 * t + X2 * t2 + X3 * t3 + X4 * t4 + X5 * t5;

// Next - Y;

    Y0 = 0.0;
    for ( i = 1006; i >= 0; --i )
      {
        Alpha = EaY0P[i][1] + EaY0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaY0P[i][0] * cos ( Alpha );
        Y0 = Y0 + D;
      }

    Y1 = 0.0;
    for ( i = 599; i >= 0; --i )
      {
        Alpha = EaY1P[i][1] + EaY1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaY1P[i][0] * cos ( Alpha );
        Y1 = Y1 + D;
      }

    Y2 = 0.0;
    for ( i = 247; i >= 0; --i )
      {
        Alpha = EaY2P[i][1] + EaY2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaY2P[i][0] * cos ( Alpha );
        Y2 = Y2 + D;
      }

    Y3 = 0.0;
    for ( i = 45; i >= 0; --i )
      {
        Alpha = EaY3P[i][1] + EaY3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaY3P[i][0] * cos ( Alpha );
        Y3 = Y3 + D;
      }

    Y4 = 0.0;
    for ( i = 19; i >= 0; --i )
      {
        Alpha = EaY4P[i][1] + EaY4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaY4P[i][0] * cos ( Alpha );
        Y4 = Y4 + D;
      }

    Y5 = 0.0;
    for ( i = 6; i >= 0; --i )
      {
        Alpha = EaY5P[i][1] + EaY5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaY5P[i][0] * cos ( Alpha );
        Y5 = Y5 + D;
      }

    EarHY = Y0 + Y1 * t + Y2 * t2 + Y3 * t3 + Y4 * t4 + Y5 * t5;

// Next - Z:

    Z0 = 0.0;
    for ( i = 177; i >= 0; --i )
      {
        Alpha = EaZ0P[i][1] + EaZ0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaZ0P[i][0] * cos ( Alpha );
        Z0 = Z0 + D;
      }

    Z1 = 0.0;
    for ( i = 96; i >= 0; --i )
      {
        Alpha = EaZ1P[i][1] + EaZ1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaZ1P[i][0] * cos ( Alpha );
        Z1 = Z1 + D;
      }

    Z2 = 0.0;
    for ( i = 46; i >= 0; --i )
      {
        Alpha = EaZ2P[i][1] + EaZ2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaZ2P[i][0] * cos ( Alpha );
        Z2 = Z2 + D;
      }

    Z3 = 0.0;
    for ( i = 10; i >= 0; --i )
      {
        Alpha = EaZ3P[i][1] + EaZ3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaZ3P[i][0] * cos ( Alpha );
        Z3 = Z3 + D;
      }

    Z4 = 0.0;
    for ( i = 4; i >= 0; --i )
      {
        Alpha = EaZ4P[i][1] + EaZ4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaZ4P[i][0] * cos ( Alpha );
        Z4 = Z4 + D;
      }

    EarHZ = Z0 + Z1 * t + Z2 * t2 + Z3 * t3 + Z4 * t4;
  } // END - clc_earrct ()

// this is x % y for double
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

// Moon longitude
double clc_monlon ( double T)
  {
    double t[5], v[6], x, y, xp, yp;
    int n, k, iv;
    
    double mon;

    if ( !datadone )
      clc_data ();

    t[0] = 1.0;
    t[1] = T;
    t[2] = t[1] * t[1];
    t[3] = t[2] * t[1];
    t[4] = t[3] * t[1];

    // TODO: do it at initialization
    for ( iv = 0; iv < 3; iv++ )
      {  
        v[iv] = 0.0;
        v[iv+3] = 0.0;
      }  

// Periodic series
// iv = 1
    for ( n = 0; n < 1023; n++ ) 
      {  
        x = cmpb[n] ;
        y = fmpb[n][0] ;
        yp = 0.0 ;
        for ( k = 1; k <= 4; k++ ) 
          {
            y = y + fmpb[n][k] * t[k] ;
            yp = yp + k * fmpb[n][k] * t[k-1] /* (k-1) */ ;
          }
        v[0] += x * sin ( y ) ;
        v[3] += x * yp * cos ( y ) ;
      }
// iv = 2
    for ( n = 1023; n < 1941; n++ ) 
      {  
        x = cmpb[n] ;
        y = fmpb[n][0] ;
        yp = 0.0 ;
        for ( k = 1; k <= 4; k++ ) 
          {
            y = y + fmpb[n][k] * t[k] ;
            yp = yp + k * fmpb[n][k] * t[k-1] /* (k-1) */ ;
          }
        v[1] += x * sin ( y ) ;
        v[4] += x * yp * cos ( y ) ;
      }
// iv = 2    
    for ( n = 1941; n < 2645; n++ ) 
      {  
        x = cmpb[n] ;
        y = fmpb[n][0] ;
        yp = 0.0 ;
        for ( k = 1; k <= 4; k++ ) 
          {
            y = y + fmpb[n][k] * t[k] ;
            yp = yp + k * fmpb[n][k] * t[k-1] /* (k-1) */ ;
          }
        v[2] += x * sin ( y ) ;
        v[5] += x * yp * cos ( y ) ;
      }

// Poisson series, Perturbations, first block, iv = 0 

    for ( n = 0; n < 11314; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[0] += x * t[0] * sin ( y ) ;
        v[3] += xp * sin ( y ) + x * t[0] * yp * cos ( y ) ;
      }
    for ( n = 11314; n < 12513; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = x * t[0] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[0] += x * t[1] * sin ( y ) ;
        v[3] += xp * sin ( y ) + x * t[1] * yp * cos ( y ) ;
      }
    for ( n = 12513; n < 12732; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = 2 * x * t[1] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[0] += x * t[2] * sin ( y ) ;
        v[3] += xp * sin ( y ) + x * t[2] * yp * cos ( y ) ;
      }
    for ( n = 12732; n < 12734; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = 3 * x * t[2] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[0] += x * t[3] * sin ( y ) ;
        v[3] += xp * sin ( y ) + x * t[3] * yp * cos ( y ) ;
      }

// SECOND BLOCK, iv = 1

    for ( n = 12734; n < 19196; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[1] += x * t[0] * sin ( y ) ;
        v[4] += xp * sin ( y ) + x * t[0] * yp * cos ( y ) ;
      }
    for ( n = 19196; n < 19712; n++ )
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = x * t[0] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[1] += x * t[1] * sin ( y ) ;
        v[4] += xp * sin ( y ) + x * t[1] * yp * cos ( y ) ;
      }
    for ( n = 19712; n < 19764; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = 2 * x * t[1] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[1] += x * t[2] * sin ( y ) ;
        v[4] += xp * sin ( y ) + x * t[2] * yp * cos ( y ) ;
      }

// THIRD BLOCK, iv = 2

    for ( n = 19764; n < 31879; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[2] += x * t[0] * sin ( y ) ;
        v[5] += xp * sin ( y ) + x * t[0] * yp * cos ( y ) ;
      }
    for ( n = 31879; n < 33044; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = x * t[0] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[2] += x * t[1] * sin ( y ) ;
        v[5] += xp * sin ( y ) + x * t[1] * yp * cos ( y ) ;
      }
    for ( n = 33044; n < 33254; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = 2 * x * t[1] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[2] += x * t[2] * sin ( y ) ;
        v[5] += xp * sin ( y ) + x * t[2] * yp * cos ( y ) ;
      }
    for ( n = 33254; n < 33256; n++ ) 
      {
        x = cper[n] ;
        y = fper[n][0] ;
        xp = 0.0 ;
        yp = 0.0 ;
        xp = 3 * x * t[2] ;
        for ( k = 1; k <= 4; k++ ) 
          { 
            y = y + fper[n][k] * t[k] ;
            yp = yp + k * fper[n][k] * t[k-1] ;
          }
        v[2] += x * t[3] * sin ( y ) ;
        v[5] += xp * sin ( y ) + x * t[3] * yp * cos ( y ) ;
      }

    v[0] = v[0] * as2rad + w[1][0] + w[1][1] * t[1] + w[1][2] * t[2] + w[1][3] * t[3] + w[1][4] * t[4];
    v[1] = v[1] * as2rad; 
    v[2] = v[2] * a405 / aelp ;
    v[3] = v[3] * as2rad + w[1][1] + 2.0 * w[1][2] * t[1] + 3.0 * w[1][3] * t[2] + 4.0 * w[1][4] * t[3];
    v[4] = v[4] * as2rad;

//    printf ( "V = %.16f %.16f %.16f %.16f %.16f %.16f\n", v[0], v[1], v[2], v[3], v[4], v[5] );

    mon = v[0] * rad2as / 3600.0;

//    printf ( "Longitude = %.16f\n", MonL );

    mon = mon + ( 5029.096 * t[1] + 1.1120 * t[2] + 0.000077 * t[3] - 0.00002353 * t[4] ) / 3600.0
                - ( 0.29965 * t[1] ) / 3600.0;
    
    mon = modfeh ( mon, 360.0 );
   
//    printf ( "Longitude = %.16f\n", MonL );

    if ( mon < 0.0 )
      mon = mon + 360.0;
      
    return (mon);
    
//    printf ( "Longitude = %.16f\n", MonL );
  
//    return ( MonL );    
  } // END - clc_monlon ()

void clc_data ( void )
  {
    int i, j, k;
    double Dw1_0, Dw2_0, Dw3_0, Deart_0, Dperi, Dw1_1, Dgam;
    double De, Deart_1, Dep, Dw2_1, Dw3_1, Dw1_2;
    double x2, x3, y2, y3, d21, d22, d23, d24, d25;   
    double d31, d32, d33, d34, d35, Cw2_1, Cw3_1;  
    int icor = 0; // Set for fit to LLR. For DE405, set to = 1

    datadone = 1;
    pis2 = cpi / 2.0;
    am =  0.074801329;
    alpha = 0.002571881;
    dtasm  = (2.0*alpha)/(3.0*am);
    xa = (2.0*alpha)/3.0;

//--- Corrections to constants: Chap2002, p.4.
      
    k = icor;
    if (k != 1)
      k=0;
    if ( k == 0 ) 
      {
//     Values of the corrections to the constants fitted to LLR.
//     Fit 13-05-02 (2 iterations) except Phi and eps w2_1 et w3_1      
        Dw1_0   = -0.10525;
        Dw2_0   =  0.16826;
        Dw3_0   = -0.10760;
        Deart_0 = -0.04012;
        Dperi   = -0.04854;
        Dw1_1   = -0.32311;
        Dgam    =  0.00069;
        De      = +0.00005;
        Deart_1 =  0.01442;
        Dep     =  0.00226;
        Dw2_1   =  0.08017;
        Dw3_1   = -0.04317;
        Dw1_2   = -0.03794;
      }
    else
      {     
//     Values of the corrections to the constants fitted to DE405
//     over the time interval (1950-2060)
        Dw1_0   = -0.07008;
        Dw2_0   =  0.20794;
        Dw3_0   = -0.07215;
        Deart_0 = -0.00033;
        Dperi   = -0.00749;
        Dw1_1   = -0.35106;
        Dgam    =  0.00085;
        De      = -0.00006;
        Deart_1 =  0.00732;
        Dep     =  0.00224;
        Dw2_1   =  0.08017;
        Dw3_1   = -0.04317;
        Dw1_2   = -0.03743;
      }

// Fundamental arguments (Moon and EMB) Chap2002, p.3, top:
   
    w[1][0] = dms ( 218.0, 18.0, 59.95571+Dw1_0);     // ELP
    w[1][1] = (1732559343.73604 + Dw1_1)/rad2as;      // ELP
    w[1][2] = (-6.8084 + Dw1_2)/rad2as;               // DE405
    w[1][3] = 0.66040e-2 / rad2as;                    // ELP
    w[1][4] = -0.31690e-4 / rad2as;                   // ELP

    w[2][0] = dms ( 83.0 ,21.0 , 11.67475 + Dw2_0 );   // ELP
    w[2][1] = ( 14643420.3171 + Dw2_1 ) / rad2as;        // DE405
    w[2][2] = ( -38.2631 ) / rad2as;                     // DE405
    w[2][3] = -0.45047e-1 / rad2as;                      // ELP
    w[2][4] = 0.21301e-3 / rad2as;                       // ELP
      
    w[3][0] = dms ( 125.0, 2.0, 40.39816 + Dw3_0 );   // ELP
    w[3][1] = ( -6967919.5383 + Dw3_1 ) / rad2as;        // DE405
    w[3][2] = ( 6.3590 ) / rad2as;                       // DE405
    w[3][3] = 0.76250e-2 / rad2as;                       // ELP
    w[3][4] = -0.35860e-4 / rad2as;                      // ELP

    eart[0] = dms ( 100.0, 27.0 ,59.13885 + Deart_0 ); // VSOP2000
    eart[1] = ( 129597742.29300 + Deart_1 ) / rad2as;   // VSOP2000
    eart[2] = -0.020200 / rad2as;                       // ELP
    eart[3] = 0.9e-5 / rad2as;                          // ELP
    eart[4] = 0.15000e-6 / rad2as;                      // ELP

    peri[0] = dms ( 102.0,56.0,14.45766 + Dperi);    // VSOP2000
    peri[1] = 1161.24342 / rad2as;                      // VSOP2000
    peri[2] = 0.529265 / rad2as;                        // VSOP2000
    peri[3] = -0.11814e-3 / rad2as;                     // VSOP2000
    peri[4] = 0.11379e-4 / rad2as;                      // VSOP2000

// --- Corrections to the secular terms of Moon angles -- Chap2002, p.6, top 
// arc seconds per century

    if ( icor == 1 )
      {
        w[1][3] = w[1][3] -0.00018865 / rad2as;
        w[1][4] = w[1][4] -0.00001024 / rad2as;
        w[2][2] = w[2][2] +0.00470602 / rad2as;
        w[2][3] = w[2][3] -0.00025213 / rad2as;
        w[3][2] = w[3][2] -0.00261070 / rad2as;
        w[3][3] = w[3][3] -0.00010712 / rad2as;
      }      

// --- Corrections to the mean motions of the Moon angles W2 and W3 
//     infered from the modifications of the constants
     
    x2 = w[2][1]/w[1][1];
    x3 = w[3][1]/w[1][1];
    y2 = am*bp[0][0]+xa*bp[4][0];
    y3 = am*bp[0][1]+xa*bp[4][1]; 

    d21 = x2-y2; 
    d22 = w[1][1]*bp[1][0];  
    d23 = w[1][1]*bp[2][0];
    d24 = w[1][1]*bp[3][0];
    d25 = y2/am;  

// --- Corrections to the mean motions of the Moon angles W2 and W3 
//     infered from the modifications of the constants 

    d31 = x3-y3;
    d32 = w[1][1]*bp[1][1];
    d33 = w[1][1]*bp[2][1];
    d34 = w[1][1]*bp[3][1];
    d35 = y3/am;

    Cw2_1 = d21*Dw1_1+d25*Deart_1+d22*Dgam+d23*De+d24*Dep;
    Cw3_1 = d31*Dw1_1+d35*Deart_1+d32*Dgam+d33*De+d34*Dep;

    w[2][1] = w[2][1]+Cw2_1/rad2as;
    w[3][1] = w[3][1]+Cw3_1/rad2as;

// Arguments of Delaunay 

    for ( i = 0; i < 5; ++i)
      {
        del[1][i] = w[1][i]-eart[i];  // D 
        del[2][i] = w[1][i]-w[3][i];  // F 
        del[3][i] = w[1][i]-w[2][i];  // l 
        del[4][i] = eart[i]-peri[i];  // l'
      }
    del[1][0] = del[1][0] + cpi;

// Planetary arguments (mean longitudes and mean motions) Chap2002, p.3

    p[1][0] = dms( 252.0, 15.0,  3.216919 );  // VSOP2000
    p[2][0] = dms( 181.0, 58.0, 44.758419 );  // VSOP2000
    p[3][0] = dms( 100.0, 27.0, 59.138850 );  // VSOP2000
    p[4][0] = dms( 355.0, 26.0,  3.642778 );  // VSOP2000
    p[5][0] = dms(  34.0, 21.0,  5.379392 );  // VSOP2000
    p[6][0] = dms(  50.0,  4.0, 38.902495 );  // VSOP2000
    p[7][0] = dms( 314.0,  3.0,  4.354234 );  // VSOP2000
    p[8][0] = dms( 304.0, 20.0, 56.808371 );  // VSOP2000

    p[1][1] = 538101628.66888 / rad2as; // VSOP2000
    p[2][1] = 210664136.45777 / rad2as; // VSOP2000
    p[3][1] = 129597742.29300 / rad2as; // VSOP2000
    p[4][1] =  68905077.65936 / rad2as; // VSOP2000
    p[5][1] =  10925660.57335 / rad2as; // VSOP2000
    p[6][1] =   4399609.33632 / rad2as; // VSOP2000
    p[7][1] =   1542482.57845 / rad2as; // VSOP2000
    p[8][1] =    786547.89700 / rad2as; // VSOP2000

    for ( i = 1; i <= 8; ++i ) // do i=1,8
      for ( j = 2; j <= 4; ++ j ) // do j=2,4
       p[i][j]=0.0;

// Zeta : Mean longitude W1 + Rate of the precession. Chap2002, p.3. 

    zeta[0] = w[1][0];
    zeta[1] = w[1][1] + ( 5029.0966 + Dprec ) / rad2as;
    zeta[2] = w[1][2];
    zeta[3] = w[1][3];
    zeta[4] = w[1][4];
     
// Corrections to the parameters: Nu, E, Gamma, n' et e'. Chap2002, p.4.

    delnu = (0.55604 + Dw1_1 )/rad2as/w[1][1];    // ELP
    dele  = (0.01789 + De )/rad2as;               // ELP
    delg  = (-0.08066 + Dgam )/rad2as;            // ELP
    delnp = (-0.06424 + Deart_1 )/rad2as/w[1][1]; // ELP 
    delep = (-0.12879 + Dep )/rad2as;             // ELP      
      
  } // END - clc_data ()

// DMS(ideg,imin,sec)=(ideg+imin/60.d0+sec/3600.d0)*deg
double dms ( double d, double m, double s )
  {
    double x;
    x = d + m/60.0 + s/3600.0;
    return ( deg2rad * modfeh ( x, 360.0 ) );
  }
  

