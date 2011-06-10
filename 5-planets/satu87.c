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
#include "saturnc.dat"
#include "saturnd.dat"

double SatHL, SatHB, SatHR; // heliocentric long, lat, radius.
double SatHX, SatHY, SatHZ; // heliocentric x, y, z.

extern double TwoPi; // = 6.283185307179586476;

double modfeh ( double, double );

void clc_satlon ( double );
void clc_satrct ( double );

// SATURN RECTANGULAR COORDINATES:
// Data in SATURNC.DAT

void clc_satrct ( double t )
  {
    // External data: double SatHX, SatHY, SatHZ;
    // Saturn, heliocentric x, y, z.

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
    for ( i = 2046; i >= 0; --i )
//  for ( i = 0; i < 2047; ++i )
      {
        Alpha = SaX0P[i][1] + SaX0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaX0P[i][0] * cos ( Alpha );
        X0 = X0 + D;
      }

    X1 = 0.0;
    for ( i = 1022; i >= 0; --i )
//  for ( i = 0; i < 1023; ++i )
      {
        Alpha = SaX1P[i][1] + SaX1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaX1P[i][0] * cos ( Alpha );
        X1 = X1 + D;
      }

    X2 = 0.0;
    for ( i = 510; i >= 0; --i )
//  for ( i = 0; i < 511; ++i )
      {
        Alpha = SaX2P[i][1] + SaX2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaX2P[i][0] * cos ( Alpha );
        X2 = X2 + D;
      }

    X3 = 0.0;
    for ( i = 249; i >= 0; --i )
//  for ( i = 0; i < 250; ++i )
      {
        Alpha = SaX3P[i][1] + SaX3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaX3P[i][0] * cos ( Alpha );
        X3 = X3 + D;
      }

    X4 = 0.0;
    for ( i = 109; i >= 0; --i )
//  for ( i = 0; i < 110; ++i )
      {
        Alpha = SaX4P[i][1] + SaX4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaX4P[i][0] * cos ( Alpha );
        X4 = X4 + D;
      }

    X5 = 0.0;
    for ( i = 39; i >= 0; --i )
//  for ( i = 0; i < 40; ++i )
      {
        Alpha = SaX5P[i][1] + SaX5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaX5P[i][0] * cos ( Alpha );
        X5 = X5 + D;
      }

    X = X0 + X1 * t + X2 * t2 + X3 * t3 + X4 * t4 + X5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    SatHX = X;

// NEXT - Y - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Y0 = 0.0;
    for ( i = 2046; i >= 0; --i )
//  for ( i = 0; i < 2047; ++i )
      {
        Alpha = SaY0P[i][1] + SaY0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaY0P[i][0] * cos ( Alpha );
        Y0 = Y0 + D;
      }

    Y1 = 0.0;
    for ( i = 1022; i >= 0; --i )
//  for ( i = 0; i < 1023; ++i )
      {
        Alpha = SaY1P[i][1] + SaY1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaY1P[i][0] * cos ( Alpha );
        Y1 = Y1 + D;
      }

    Y2 = 0.0;
    for ( i = 510; i >= 0; --i )
//  for ( i = 0; i < 511; ++i )
      {
        Alpha = SaY2P[i][1] + SaY2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaY2P[i][0] * cos ( Alpha );
        Y2 = Y2 + D;
      }

    Y3 = 0.0;
    for ( i = 249; i >= 0; --i )
//  for ( i = 0; i < 250; ++i )
      {
        Alpha = SaY3P[i][1] + SaY3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaY3P[i][0] * cos ( Alpha );
        Y3 = Y3 + D;
      }

    Y4 = 0.0;
    for ( i = 109; i >= 0; --i )
//  for ( i = 0; i < 110; ++i )
      {
        Alpha = SaY4P[i][1] + SaY4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaY4P[i][0] * cos ( Alpha );
        Y4 = Y4 + D;
      }

    Y5 = 0.0;
    for ( i = 39; i >= 0; --i )
//  for ( i = 0; i < 40; ++i )
      {
        Alpha = SaY5P[i][1] + SaY5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaY5P[i][0] * cos ( Alpha );
        Y5 = Y5 + D;
      }

    Y = Y0 + Y1 * t + Y2 * t2 + Y3 * t3 + Y4 * t4 + Y5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    SatHY = Y;

// NEXT - Z - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Z0 = 0.0;
    for ( i = 419; i >= 0; --i )
//  for ( i = 0; i < 420; ++i )
      {
        Alpha = SaZ0P[i][1] + SaZ0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaZ0P[i][0] * cos ( Alpha );
        Z0 = Z0 + D;
      }

    Z1 = 0.0;
    for ( i = 233; i >= 0; --i )
//  for ( i = 0; i < 234; ++i )
      {
        Alpha = SaZ1P[i][1] + SaZ1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaZ1P[i][0] * cos ( Alpha );
        Z1 = Z1 + D;
      }

    Z2 = 0.0;
    for ( i = 90; i >= 0; --i )
//  for ( i = 0; i < 91; ++i )
      {
        Alpha = SaZ2P[i][1] + SaZ2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaZ2P[i][0] * cos ( Alpha );
        Z2 = Z2 + D;
      }

    Z3 = 0.0;
    for ( i = 44; i >= 0; --i )
//  for ( i = 0; i < 45; ++i )
      {
        Alpha = SaZ3P[i][1] + SaZ3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaZ3P[i][0] * cos ( Alpha );
        Z3 = Z3 + D;
      }

    Z4 = 0.0;
    for ( i = 21; i >= 0; --i )
//  for ( i = 0; i < 22; ++i )
      {
        Alpha = SaZ4P[i][1] + SaZ4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaZ4P[i][0] * cos ( Alpha );
        Z4 = Z4 + D;
      }

    Z5 = 0.0;
    for ( i = 8; i >= 0; --i )
//  for ( i = 0; i < 9; ++i )
      {
        Alpha = SaZ5P[i][1] + SaZ5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaZ5P[i][0] * cos ( Alpha );
        Z5 = Z5 + D;
      }

    Z = Z0 + Z1 * t + Z2 * t2 + Z3 * t3 + Z4 * t4 + Z5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    SatHZ = Z;

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

  } // END - clc_satlon ()

// SATURN SPHERICAL COORDINATES:
// Data in SATURND.DAT

void clc_satlon ( double t )
  {
    // External data: double SatHL, SatHB, SatHR;
    // Saturn heliocentric long, lat, radius.

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

    L0 = 0.87401354029;            // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 1151; i >= 1; --i )  // AND THESE
//  for ( i = 1; i < 1152; ++i )   // AND THESE
      {
        Alpha = SaL0P[i][1] + SaL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }

    L0 = L0 + SL;

    L1 = 213.54295595986;       // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 641; i >= 1; --i )
//  for ( i = 1; i < 642; ++i )
      {
        Alpha = SaL1P[i][1] + SaL1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaL1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L1 = L1 + SL;

    SL = 0.0;
    for ( i = 320; i >= 0; --i )
//  for ( i = 0; i < 321; ++i )
      {
        Alpha = SaL2P[i][1] + SaL2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaL2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L2 = SL;

    SL = 0.0;
    for ( i = 147; i >= 0; --i )
//  for ( i = 0; i < 148; ++i )
      {
        Alpha = SaL3P[i][1] + SaL3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaL3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L3 = SL;

    SL = 0.0;
    for ( i = 67; i >= 0; --i )
//  for ( i = 0; i < 68; ++i )
      {
        Alpha = SaL4P[i][1] + SaL4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaL4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L4 = SL;

    SL = 0.0;
    for ( i = 26; i >= 0; --i )
//  for ( i = 0; i < 27; ++i )
      {
        Alpha = SaL5P[i][1] + SaL5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaL5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L5 = SL;

    L = L0 + L1 * t + L2 * t2 + L3 * t3 + L4 * t4 + L5 * t5;

    L = modfeh ( L, TwoPi );
    if ( L < 0.0 )
      L = L + TwoPi;

    SatHL = L;

// NEXT - HELIOCENTRIC LATITUDE:

    SL = 0.0;
    for ( i = 499; i >= 0; --i )
//  for ( i = 0; i < 500; ++i )
      {
        Alpha = SaB0P[i][1] + SaB0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaB0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B0 = SL;

    SL = 0.0;
    for ( i = 259; i >= 0; --i )
//  for ( i = 0; i < 260; ++i )
      {
        Alpha = SaB1P[i][1] + SaB1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaB1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B1 = SL;

    SL = 0.0;
    for ( i = 110; i >= 0; --i )
//  for ( i = 0; i < 111; ++i )
      {
        Alpha = SaB2P[i][1] + SaB2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaB2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B2 = SL;

    SL = 0.0;
    for ( i = 57; i >= 0; --i )
//  for ( i = 0; i < 58; ++i )
      {
        Alpha = SaB3P[i][1] + SaB3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaB3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B3 = SL;

    SL = 0.0;
    for ( i = 25; i >= 0; --i )
//  for ( i = 0; i < 26; ++i )
      {
        Alpha = SaB4P[i][1] + SaB4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaB4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B4 = SL;

    SL = 0.0;
    for ( i = 10; i >= 0; --i )
//  for ( i = 0; i < 11; ++i )
      {
        Alpha = SaB5P[i][1] + SaB5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaB5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B5 = SL;

    B = B0 + B1 * t + B2 * t2 + B3 * t3 + B4 * t4 + B5 * t5;
//  B = modfeh ( B, TwoPi );
    SatHB = B;

// NEXT - HELIOCENTRIC RADIUS VECTOR:

    R0 = 9.55758135801;         // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 1204; i >= 1; --i )
//  for ( i = 1; i < 1205; ++i )
      {
        Alpha = SaR0P[i][1] + SaR0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaR0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R0 = R0 + SL;

    SL = 0.0;
    for ( i = 638; i >= 0; --i )
//  for ( i = 0; i < 639; ++i )
      {
        Alpha = SaR1P[i][1] + SaR1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaR1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R1 = SL;

    SL = 0.0;
    for ( i = 341; i >= 0; --i )
//  for ( i = 0; i < 342; ++i )
      {
        Alpha = SaR2P[i][1] + SaR2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaR2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R2 = SL;

    SL = 0.0;
    for ( i = 156; i >= 0; --i )
//  for ( i = 0; i < 157; ++i )
      {
        Alpha = SaR3P[i][1] + SaR3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaR3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R3 = SL;

    SL = 0.0;
    for ( i = 63; i >= 0; --i )
//  for ( i = 0; i < 64; ++i )
      {
        Alpha = SaR4P[i][1] + SaR4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaR4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R4 = SL;

    SL = 0.0;
    for ( i = 27; i >= 0; --i )
//  for ( i = 0; i < 28; ++i )
      {
        Alpha = SaR5P[i][1] + SaR5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = SaR5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R5 = SL;
    R = R0 + R1 * t + R2 * t2 + R3 * t3 + R4 * t4 + R5 * t5;
    SatHR = R;
  } // - clc_satlon ()
        