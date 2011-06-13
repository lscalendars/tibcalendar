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
#include "mercc.dat"
#include "mercd.dat"

double MerHL, MerHB, MerHR; // Mercury, heliocentric long, lat, radius.
double MerHX, MerHY, MerHZ; // Mercury, heliocentric x, y, z.

extern double TwoPi; // = 6.283185307179586476;

double modfeh ( double, double );

void clc_merlon ( double );
void clc_merrct ( double );

// MERCURY RECTANGULAR COORDINATES:
// Data in MERCC.DAT

void clc_merrct ( double t )
  {
    // External data: double MerHX, MerHY, MerHZ;
    // Mars, heliocentric x, y, z.

    double X, Y, Z; // Rectangular coordinates.
    double X0, X1, X2, X3, X4, X5;
    double Y0, Y1, Y2, Y3, Y4, Y5;
    double Z0, Z1, Z2, Z3, Z4, Z5;
    double D, Alpha, t2, t3, t4, t5;
    double DeltaL, DeltaN, A1, A2;
    int i;

    t2 = t * t;
    t3 = t * t2;
    t4 = t2 * t2;
    t5 = t2 * t3;

    X0 = 0.0;
    for ( i = 1852; i >= 0; --i )
//  for ( i = 0; i < 1853; ++i )
      {
        Alpha = MeX0P[i][1] + MeX0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeX0P[i][0] * cos ( Alpha );
        X0 = X0 + D;
      }

    X1 = 0.0;
    for ( i = 1022; i >= 0; --i )
//  for ( i = 0; i < 1023; ++i )
      {
        Alpha = MeX1P[i][1] + MeX1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeX1P[i][0] * cos ( Alpha );
        X1 = X1 + D;
      }

    X2 = 0.0;
    for ( i = 412; i >= 0; --i )
//  for ( i = 0; i < 413; ++i )
      {
        Alpha = MeX2P[i][1] + MeX2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeX2P[i][0] * cos ( Alpha );
        X2 = X2 + D;
      }

    X3 = 0.0;
    for ( i = 134; i >= 0; --i )
//  for ( i = 0; i < 135; ++i )
      {
        Alpha = MeX3P[i][1] + MeX3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeX3P[i][0] * cos ( Alpha );
        X3 = X3 + D;
      }

    X4 = 0.0;
    for ( i = 41; i >= 0; --i )
//  for ( i = 0; i < 42; ++i )
      {
        Alpha = MeX4P[i][1] + MeX4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeX4P[i][0] * cos ( Alpha );
        X4 = X4 + D;
      }

    X5 = 0.0;
    for ( i = 15; i >= 0; --i )
//  for ( i = 0; i < 16; ++i )
      {
        Alpha = MeX5P[i][1] + MeX5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeX5P[i][0] * cos ( Alpha );
        X5 = X5 + D;
      }

    X = X0 + X1 * t + X2 * t2 + X3 * t3 + X4 * t4 + X5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    MerHX = X;

// NEXT - Y - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Y0 = 0.0;
    for ( i = 1852; i >= 0; --i )
//  for ( i = 0; i < 1853; ++i )
      {
        Alpha = MeY0P[i][1] + MeY0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeY0P[i][0] * cos ( Alpha );
        Y0 = Y0 + D;
      }

    Y1 = 0.0;
    for ( i = 1022; i >= 0; --i )
//  for ( i = 0; i < 1023; ++i )
      {
        Alpha = MeY1P[i][1] + MeY1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeY1P[i][0] * cos ( Alpha );
        Y1 = Y1 + D;
      }

    Y2 = 0.0;
    for ( i = 412; i >= 0; --i )
//  for ( i = 0; i < 413; ++i )
      {
        Alpha = MeY2P[i][1] + MeY2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeY2P[i][0] * cos ( Alpha );
        Y2 = Y2 + D;
      }

    Y3 = 0.0;
    for ( i = 134; i >= 0; --i )
//  for ( i = 0; i < 135; ++i )
      {
        Alpha = MeY3P[i][1] + MeY3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeY3P[i][0] * cos ( Alpha );
        Y3 = Y3 + D;
      }

    Y4 = 0.0;
    for ( i = 41; i >= 0; --i )
//  for ( i = 0; i < 42; ++i )
      {
        Alpha = MeY4P[i][1] + MeY4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeY4P[i][0] * cos ( Alpha );
        Y4 = Y4 + D;
      }

    Y5 = 0.0;
    for ( i = 15; i >= 0; --i )
//  for ( i = 0; i < 16; ++i )
      {
        Alpha = MeY5P[i][1] + MeY5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeY5P[i][0] * cos ( Alpha );
        Y5 = Y5 + D;
      }

    Y = Y0 + Y1 * t + Y2 * t2 + Y3 * t3 + Y4 * t4 + Y5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    MerHY = Y;

// NEXT - Z - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Z0 = 0.0;
    for ( i = 597; i >= 0; --i )
//  for ( i = 0; i < 598; ++i )
      {
        Alpha = MeZ0P[i][1] + MeZ0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeZ0P[i][0] * cos ( Alpha );
        Z0 = Z0 + D;
      }

    Z1 = 0.0;
    for ( i = 359; i >= 0; --i )
//  for ( i = 0; i < 360; ++i )
      {
        Alpha = MeZ1P[i][1] + MeZ1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeZ1P[i][0] * cos ( Alpha );
        Z1 = Z1 + D;
      }

    Z2 = 0.0;
    for ( i = 166; i >= 0; --i )
//  for ( i = 0; i < 167; ++i )
      {
        Alpha = MeZ2P[i][1] + MeZ2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeZ2P[i][0] * cos ( Alpha );
        Z2 = Z2 + D;
      }

    Z3 = 0.0;
    for ( i = 46; i >= 0; --i )
//  for ( i = 0; i < 47; ++i )
      {
        Alpha = MeZ3P[i][1] + MeZ3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeZ3P[i][0] * cos ( Alpha );
        Z3 = Z3 + D;
      }

    Z4 = 0.0;
    for ( i = 11; i >= 0; --i )
//  for ( i = 0; i < 12; ++i )
      {
        Alpha = MeZ4P[i][1] + MeZ4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeZ4P[i][0] * cos ( Alpha );
        Z4 = Z4 + D;
      }

    Z5 = 0.0;
    for ( i = 6; i >= 0; --i )
//  for ( i = 0; i < 7; ++i )
      {
        Alpha = MeZ5P[i][1] + MeZ5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeZ5P[i][0] * cos ( Alpha );
        Z5 = Z5 + D;
      }

    Z = Z0 + Z1 * t + Z2 * t2 + Z3 * t3 + Z4 * t4 + Z5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    MerHZ = Z;

// Aberration:

//  X = 3.10 + 62830.14 * U;
//  X = modfeh ( X, TwoPi );
//  DeltaL = ( -997.0 + 17.0 * cos ( X ) ) * 1.0e-7;

// Now, Nutation:

// A1 = 2.18 - 3375.70 * U + 0.36 * U2;
// A1 = modfeh ( A1, TwoPi );
// A2 = 3.51 + 125666.39 * U + 0.10 * U2;
// A2 = modfeh ( A2, TwoPi );
// DeltaN = ( -834.0 * sin ( A1 ) - 64.0 * sin ( A2 ) ) * 1.0e-7;

// printf ( "Correction for Aberration: %.6f, Nutation: %.6f\n",
//          DeltaL, DeltaN );

//  L = ( L + SL * 1.0e-7 + DeltaL + DeltaN ) / rad;
//  if ( L < 0.0 )
//    L = L + 360.0;

  } // END - clc_merlon ()

// MERCURY SPHERICAL COORDINATES:
// Data in MERCD.DAT

void clc_merlon ( double t )
  {
    // External data: double MerHL, MerHB, MerHR;
    // Mercury heliocentric long, lat, radius.

    double L, B, R; // Longitude and radius vector.
    double SL;
    double L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4, B5;
    double R0, R1, R2, R3, R4, R5;
    double D, Alpha, t2, t3, t4, t5;
    double DeltaL, DeltaN, A1, A2;
    int i;

    t2 = t * t;
    t3 = t * t2;
    t4 = t2 * t2;
    t5 = t2 * t3;

// FIRST - HELIOCENTRIC LONGITUDE:

    L0 = 4.40250710144;  // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 1379; i >= 1; --i )  // AND THESE
//  for ( i = 1; i < 1380; ++i )   // AND THESE
      {
        Alpha = MeL0P[i][1] + MeL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }

    L0 = L0 + SL;

    L1 = 26088.14706222746;       // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 838; i >= 1; --i )
//  for ( i = 1; i < 839; ++i )
      {
        Alpha = MeL1P[i][1] + MeL1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeL1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L1 = L1 + SL;

    SL = 0.0;
    for ( i = 394; i >= 0; --i )
//  for ( i = 0; i < 395; ++i )
      {
        Alpha = MeL2P[i][1] + MeL2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeL2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L2 = SL;

    SL = 0.0;
    for ( i = 152; i >= 0; --i )
//  for ( i = 0; i < 153; ++i )
      {
        Alpha = MeL3P[i][1] + MeL3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeL3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L3 = SL;

    SL = 0.0;
    for ( i = 27; i >= 0; --i )
//  for ( i = 0; i < 28; ++i )
      {
        Alpha = MeL4P[i][1] + MeL4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeL4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L4 = SL;

    SL = 0.0;
    for ( i = 12; i >= 0; --i )
//  for ( i = 0; i < 13; ++i )
      {
        Alpha = MeL5P[i][1] + MeL5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeL5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L5 = SL;

    L = L0 + L1 * t + L2 * t2 + L3 * t3 + L4 * t4 + L5 * t5;

    L = modfeh ( L, TwoPi );
    if ( L < 0.0 )
      L = L + TwoPi;

    MerHL = L;

// NEXT - HELIOCENTRIC LATITUDE:

    SL = 0.0;
    for ( i = 817; i >= 0; --i )
//  for ( i = 0; i < 818; ++i )
      {
        Alpha = MeB0P[i][1] + MeB0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeB0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B0 = SL;

    SL = 0.0;
    for ( i = 493; i >= 0; --i )
//  for ( i = 0; i < 494; ++i )
      {
        Alpha = MeB1P[i][1] + MeB1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeB1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B1 = SL;

    SL = 0.0;
    for ( i = 229; i >= 0; --i )
//  for ( i = 0; i < 230; ++i )
      {
        Alpha = MeB2P[i][1] + MeB2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeB2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B2 = SL;

    SL = 0.0;
    for ( i = 52; i >= 0; --i )
//  for ( i = 0; i < 53; ++i )
      {
        Alpha = MeB3P[i][1] + MeB3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeB3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B3 = SL;

    SL = 0.0;
    for ( i = 14; i >= 0; --i )
//  for ( i = 0; i < 15; ++i )
      {
        Alpha = MeB4P[i][1] + MeB4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeB4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B4 = SL;

    SL = 0.0;
    for ( i = 9; i >= 0; --i )
//  for ( i = 0; i < 10; ++i )
      {
        Alpha = MeB5P[i][1] + MeB5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeB5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B5 = SL;

    B = B0 + B1 * t + B2 * t2 + B3 * t3 + B4 * t4 + B5 * t5;
//  B = modfeh ( B, TwoPi );
    MerHB = B;

// NEXT - HELIOCENTRIC RADIUS VECTOR:

    R0 = 0.39528271652;            // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 1214; i >= 1; --i )
//  for ( i = 1; i < 1215; ++i )
      {
        Alpha = MeR0P[i][1] + MeR0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeR0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R0 = R0 + SL;

    SL = 0.0;
    for ( i = 710; i >= 0; --i )
//  for ( i = 0; i < 711; ++i )
      {
        Alpha = MeR1P[i][1] + MeR1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeR1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R1 = SL;

    SL = 0.0;
    for ( i = 325; i >= 0; --i )
//  for ( i = 0; i < 326; ++i )
      {
        Alpha = MeR2P[i][1] + MeR2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeR2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R2 = SL;

    SL = 0.0;
    for ( i = 118; i >= 0; --i )
//  for ( i = 0; i < 119; ++i )
      {
        Alpha = MeR3P[i][1] + MeR3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeR3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R3 = SL;

    SL = 0.0;
    for ( i = 17; i >= 0; --i )
//  for ( i = 0; i < 18; ++i )
      {
        Alpha = MeR4P[i][1] + MeR4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeR4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R4 = SL;

    SL = 0.0;
    for ( i = 9; i >= 0; --i )
//  for ( i = 0; i < 10; ++i )
      {
        Alpha = MeR5P[i][1] + MeR5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MeR5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R5 = SL;
    R = R0 + R1 * t + R2 * t2 + R3 * t3 + R4 * t4 + R5 * t5;
    MerHR = R;
  } // - clc_merlon ()
      