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
#include "marsc.dat"
#include "marsd.dat"

double MarHL, MarHB, MarHR; // Mars, heliocentric long, lat, radius.
double MarHX, MarHY, MarHZ; // Mars, heliocentric x, y, z.

extern double TwoPi; // = 6.283185307179586476;

void clc_marlon ( double );
void clc_marrct ( double );
double modfeh ( double, double );

// MARS RECTANGULAR COORDINATES:
// Data in MARSC.DAT

void clc_marrct ( double t )
  {
    // External data: double MarHX, MarHY, MarHZ;
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
    for ( i = 1906; i >= 0; --i )
//  for ( i = 0; i < 1907; ++i )
      {
        Alpha = MaX0P[i][1] + MaX0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaX0P[i][0] * cos ( Alpha );
        X0 = X0 + D;
      }

    X1 = 0.0;
    for ( i = 1022; i >= 0; --i )
//  for ( i = 0; i < 1023; ++i )
      {
        Alpha = MaX1P[i][1] + MaX1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaX1P[i][0] * cos ( Alpha );
        X1 = X1 + D;
      }

    X2 = 0.0;
    for ( i = 510; i >= 0; --i )
//  for ( i = 0; i < 511; ++i )
      {
        Alpha = MaX2P[i][1] + MaX2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaX2P[i][0] * cos ( Alpha );
        X2 = X2 + D;
      }

    X3 = 0.0;
    for ( i = 191; i >= 0; --i )
//  for ( i = 0; i < 192; ++i )
      {
        Alpha = MaX3P[i][1] + MaX3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaX3P[i][0] * cos ( Alpha );
        X3 = X3 + D;
      }

    X4 = 0.0;
    for ( i = 80; i >= 0; --i )
//  for ( i = 0; i < 81; ++i )
      {
        Alpha = MaX4P[i][1] + MaX4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaX4P[i][0] * cos ( Alpha );
        X4 = X4 + D;
      }

    X5 = 0.0;
    for ( i = 25; i >= 0; --i )
//  for ( i = 0; i < 26; ++i )
      {
        Alpha = MaX5P[i][1] + MaX5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaX5P[i][0] * cos ( Alpha );
        X5 = X5 + D;
      }

    X = X0 + X1 * t + X2 * t2 + X3 * t3 + X4 * t4 + X5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    MarHX = X;

// NEXT - Y - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Y0 = 0.0;
    for ( i = 1906; i >= 0; --i )
//  for ( i = 0; i < 1907; ++i )
      {
        Alpha = MaY0P[i][1] + MaY0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaY0P[i][0] * cos ( Alpha );
        Y0 = Y0 + D;
      }

    Y1 = 0.0;
    for ( i = 1022; i >= 0; --i )
//  for ( i = 0; i < 1023; ++i )
      {
        Alpha = MaY1P[i][1] + MaY1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaY1P[i][0] * cos ( Alpha );
        Y1 = Y1 + D;
      }

    Y2 = 0.0;
    for ( i = 510; i >= 0; --i )
//  for ( i = 0; i < 511; ++i )
      {
        Alpha = MaY2P[i][1] + MaY2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaY2P[i][0] * cos ( Alpha );
        Y2 = Y2 + D;
      }

    Y3 = 0.0;
    for ( i = 191; i >= 0; --i )
//  for ( i = 0; i < 192; ++i )
      {
        Alpha = MaY3P[i][1] + MaY3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaY3P[i][0] * cos ( Alpha );
        Y3 = Y3 + D;
      }

    Y4 = 0.0;
    for ( i = 80; i >= 0; --i )
//  for ( i = 0; i < 81; ++i )
      {
        Alpha = MaY4P[i][1] + MaY4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaY4P[i][0] * cos ( Alpha );
        Y4 = Y4 + D;
      }

    Y5 = 0.0;
    for ( i = 25; i >= 0; --i )
//  for ( i = 0; i < 26; ++i )
      {
        Alpha = MaY5P[i][1] + MaY5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaY5P[i][0] * cos ( Alpha );
        Y5 = Y5 + D;
      }

    Y = Y0 + Y1 * t + Y2 * t2 + Y3 * t3 + Y4 * t4 + Y5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    MarHY = Y;

// NEXT - Z - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Z0 = 0.0;
    for ( i = 354; i >= 0; --i )
//  for ( i = 0; i < 355; ++i )
      {
        Alpha = MaZ0P[i][1] + MaZ0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaZ0P[i][0] * cos ( Alpha );
        Z0 = Z0 + D;
      }

    Z1 = 0.0;
    for ( i = 251; i >= 0; --i )
//  for ( i = 0; i < 252; ++i )
      {
        Alpha = MaZ1P[i][1] + MaZ1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaZ1P[i][0] * cos ( Alpha );
        Z1 = Z1 + D;
      }

    Z2 = 0.0;
    for ( i = 132; i >= 0; --i )
//  for ( i = 0; i < 133; ++i )
      {
        Alpha = MaZ2P[i][1] + MaZ2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaZ2P[i][0] * cos ( Alpha );
        Z2 = Z2 + D;
      }

    Z3 = 0.0;
    for ( i = 55; i >= 0; --i )
//  for ( i = 0; i < 56; ++i )
      {
        Alpha = MaZ3P[i][1] + MaZ3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaZ3P[i][0] * cos ( Alpha );
        Z3 = Z3 + D;
      }

    Z4 = 0.0;
    for ( i = 19; i >= 0; --i )
//  for ( i = 0; i < 20; ++i )
      {
        Alpha = MaZ4P[i][1] + MaZ4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaZ4P[i][0] * cos ( Alpha );
        Z4 = Z4 + D;
      }

    Z5 = 0.0;
    for ( i = 6; i >= 0; --i )
//  for ( i = 0; i < 7; ++i )
      {
        Alpha = MaZ5P[i][1] + MaZ5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaZ5P[i][0] * cos ( Alpha );
        Z5 = Z5 + D;
      }

    Z = Z0 + Z1 * t + Z2 * t2 + Z3 * t3 + Z4 * t4 + Z5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    MarHZ = Z;

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

  } // END - clc_marrct ()

// MARS SPHERICAL COORDINATES:
// Data in MARSD.DAT

void clc_marlon ( double t )
  {
    // External data: double MarHL, MarHB, MarHR;
    // Mars, heliocentric long, lat, radius.

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

    L0 = 6.20347711583;           // DIFFERENT FOR EACH PLANET
    SL = 0.0;
//  for ( i = 1216; i >= 1; --i )
    for ( i = 1; i < 1217; ++i )
      {
        Alpha = MaL0P[i][1] + MaL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }

    L0 = L0 + SL;

    L1 = 3340.85627474342;
    SL = 0.0;
//  for ( i = 685; i >= 1; --i )
    for ( i = 1; i < 686; ++i )
      {
        Alpha = MaL1P[i][1] + MaL1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaL1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L1 = L1 + SL;

    SL = 0.0;
//  for ( i = 309; i >= 0; --i )
    for ( i = 0; i < 310; ++i )
      {
        Alpha = MaL2P[i][1] + MaL2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaL2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L2 = SL;

    SL = 0.0;
//  for ( i = 128; i >= 0; --i )
    for ( i = 0; i < 129; ++i )
      {
        Alpha = MaL3P[i][1] + MaL3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaL3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L3 = SL;

    SL = 0.0;
//  for ( i = 35; i >= 0; --i )
    for ( i = 0; i < 36; ++i )
      {
        Alpha = MaL4P[i][1] + MaL4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaL4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L4 = SL;

    SL = 0.0;
//  for ( i = 14; i >= 0; --i )
    for ( i = 0; i < 15; ++i )
      {
        Alpha = MaL5P[i][1] + MaL5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaL5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L5 = SL;

    L = L0 + L1 * t + L2 * t2 + L3 * t3 + L4 * t4 + L5 * t5;

    L = modfeh ( L, TwoPi );
    if ( L < 0.0 )
      L = L + TwoPi;

    MarHL = L;

// NEXT - HELIOCENTRIC LATITUDE:

    SL = 0.0;
//  for ( i = 440; i >= 0; --i )
    for ( i = 0; i < 441; ++i )
      {
        Alpha = MaB0P[i][1] + MaB0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaB0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B0 = SL;

    SL = 0.0;
//  for ( i = 286; i >= 0; --i )
    for ( i = 0; i < 287; ++i )
      {
        Alpha = MaB1P[i][1] + MaB1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaB1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B1 = SL;

    SL = 0.0;
//  for ( i = 129; i >= 0; --i )
    for ( i = 0; i < 130; ++i )
      {
        Alpha = MaB2P[i][1] + MaB2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaB2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B2 = SL;

    SL = 0.0;
//  for ( i = 40; i >= 0; --i )
    for ( i = 0; i < 41; ++i )
      {
        Alpha = MaB3P[i][1] + MaB3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaB3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B3 = SL;

    SL = 0.0;
//  for ( i = 10; i >= 0; --i )
    for ( i = 0; i < 11; ++i )
      {
        Alpha = MaB4P[i][1] + MaB4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaB4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B4 = SL;

    SL = 0.0;
//  for ( i = 4; i >= 0; --i )
    for ( i = 0; i < 5; ++i )
      {
        Alpha = MaB5P[i][1] + MaB5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaB5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B5 = SL;

    B = B0 + B1 * t + B2 * t2 + B3 * t3 + B4 * t4 + B5 * t5;
//  B = modfeh ( B, TwoPi );
    MarHB = B;

// NEXT - HELIOCENTRIC RADIUS VECTOR:

    R0 = 1.53033488276;
    SL = 0.0;
//  for ( i = 1117; i >= 1; --i )
    for ( i = 1; i < 1118; ++i )
      {
        Alpha = MaR0P[i][1] + MaR0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaR0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R0 = R0 + SL;

    SL = 0.0;
//  for ( i = 595; i >= 0; --i )
    for ( i = 0; i < 596; ++i )
      {
        Alpha = MaR1P[i][1] + MaR1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaR1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R1 = SL;

    SL = 0.0;
//  for ( i = 312; i >= 0; --i )
    for ( i = 0; i < 313; ++i )
      {
        Alpha = MaR2P[i][1] + MaR2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaR2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R2 = SL;

    SL = 0.0;
//  for ( i = 110; i >= 0; --i )
    for ( i = 0; i < 111; ++i )
      {
        Alpha = MaR3P[i][1] + MaR3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaR3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R3 = SL;

    SL = 0.0;
//  for ( i = 27; i >= 0; --i )
    for ( i = 0; i < 28; ++i )
      {
        Alpha = MaR4P[i][1] + MaR4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaR4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R4 = SL;

    SL = 0.0;
//  for ( i = 8; i >= 0; --i )
    for ( i = 0; i < 9; ++i )
      {
        Alpha = MaR5P[i][1] + MaR5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = MaR5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R5 = SL;
    R = R0 + R1 * t + R2 * t2 + R3 * t3 + R4 * t4 + R5 * t5;
    MarHR = R;
  } // - clc_marlon ()
                                                                                                                        