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
#include "jupic.dat"
#include "jupid.dat"

double JupHL, JupHB, JupHR; // heliocentric long, lat, radius.
double JupHX, JupHY, JupHZ; // heliocentric x, y, z.

extern double TwoPi; // = 6.283185307179586476;

double modfeh ( double, double );

void clc_juplon ( double );
void clc_juprct ( double );

// JUPITER  RECTANGULAR COORDINATES:
// Data in JUPIC.DAT

void clc_juprct ( double t )
  {
    // External data: double JupHX, JupHY, JupHZ;
    // Jupiter, heliocentric x, y, z.

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
    for ( i = 1271; i >= 0; --i )
//  for ( i = 0; i < 1272; ++i )
      {
        Alpha = JuX0P[i][1] + JuX0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuX0P[i][0] * cos ( Alpha );
        X0 = X0 + D;
      }

    X1 = 0.0;
    for ( i = 637; i >= 0; --i )
//  for ( i = 0; i < 638; ++i )
      {
        Alpha = JuX1P[i][1] + JuX1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuX1P[i][0] * cos ( Alpha );
        X1 = X1 + D;
      }

    X2 = 0.0;
    for ( i = 357; i >= 0; --i )
//  for ( i = 0; i < 358; ++i )
      {
        Alpha = JuX2P[i][1] + JuX2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuX2P[i][0] * cos ( Alpha );
        X2 = X2 + D;
      }

    X3 = 0.0;
    for ( i = 189; i >= 0; --i )
//  for ( i = 0; i < 190; ++i )
      {
        Alpha = JuX3P[i][1] + JuX3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuX3P[i][0] * cos ( Alpha );
        X3 = X3 + D;
      }

    X4 = 0.0;
    for ( i = 72; i >= 0; --i )
//  for ( i = 0; i < 73; ++i )
      {
        Alpha = JuX4P[i][1] + JuX4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuX4P[i][0] * cos ( Alpha );
        X4 = X4 + D;
      }

    X5 = 0.0;
    for ( i = 14; i >= 0; --i )
//  for ( i = 0; i < 15; ++i )
      {
        Alpha = JuX5P[i][1] + JuX5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuX5P[i][0] * cos ( Alpha );
        X5 = X5 + D;
      }

    X = X0 + X1 * t + X2 * t2 + X3 * t3 + X4 * t4 + X5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    JupHX = X;

// NEXT - Y - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Y0 = 0.0;
    for ( i = 1271; i >= 0; --i )
//  for ( i = 0; i < 1272; ++i )
      {
        Alpha = JuY0P[i][1] + JuY0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuY0P[i][0] * cos ( Alpha );
        Y0 = Y0 + D;
      }

    Y1 = 0.0;
    for ( i = 637; i >= 0; --i )
//  for ( i = 0; i < 638; ++i )
      {
        Alpha = JuY1P[i][1] + JuY1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuY1P[i][0] * cos ( Alpha );
        Y1 = Y1 + D;
      }

    Y2 = 0.0;
    for ( i = 357; i >= 0; --i )
//  for ( i = 0; i < 358; ++i )
      {
        Alpha = JuY2P[i][1] + JuY2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuY2P[i][0] * cos ( Alpha );
        Y2 = Y2 + D;
      }

    Y3 = 0.0;
    for ( i = 189; i >= 0; --i )
//  for ( i = 0; i < 190; ++i )
      {
        Alpha = JuY3P[i][1] + JuY3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuY3P[i][0] * cos ( Alpha );
        Y3 = Y3 + D;
      }

    Y4 = 0.0;
    for ( i = 72; i >= 0; --i )
//  for ( i = 0; i < 73; ++i )
      {
        Alpha = JuY4P[i][1] + JuY4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuY4P[i][0] * cos ( Alpha );
        Y4 = Y4 + D;
      }

    Y5 = 0.0;
    for ( i = 14; i >= 0; --i )
//  for ( i = 0; i < 15; ++i )
      {
        Alpha = JuY5P[i][1] + JuY5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuY5P[i][0] * cos ( Alpha );
        Y5 = Y5 + D;
      }

    Y = Y0 + Y1 * t + Y2 * t2 + Y3 * t3 + Y4 * t4 + Y5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    JupHY = Y;

// NEXT - Z - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Z0 = 0.0;
    for ( i = 215; i >= 0; --i )
//  for ( i = 0; i < 216; ++i )
      {
        Alpha = JuZ0P[i][1] + JuZ0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuZ0P[i][0] * cos ( Alpha );
        Z0 = Z0 + D;
      }

    Z1 = 0.0;
    for ( i = 123; i >= 0; --i )
//  for ( i = 0; i < 124; ++i )
      {
        Alpha = JuZ1P[i][1] + JuZ1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuZ1P[i][0] * cos ( Alpha );
        Z1 = Z1 + D;
      }

    Z2 = 0.0;
    for ( i = 67; i >= 0; --i )
//  for ( i = 0; i < 68; ++i )
      {
        Alpha = JuZ2P[i][1] + JuZ2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuZ2P[i][0] * cos ( Alpha );
        Z2 = Z2 + D;
      }

    Z3 = 0.0;
    for ( i = 38; i >= 0; --i )
//  for ( i = 0; i < 39; ++i )
      {
        Alpha = JuZ3P[i][1] + JuZ3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuZ3P[i][0] * cos ( Alpha );
        Z3 = Z3 + D;
      }

    Z4 = 0.0;
    for ( i = 10; i >= 0; --i )
//  for ( i = 0; i < 11; ++i )
      {
        Alpha = JuZ4P[i][1] + JuZ4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuZ4P[i][0] * cos ( Alpha );
        Z4 = Z4 + D;
      }

    Z5 = 0.0;
    for ( i = 4; i >= 0; --i )
//  for ( i = 0; i < 5; ++i )
      {
        Alpha = JuZ5P[i][1] + JuZ5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuZ5P[i][0] * cos ( Alpha );
        Z5 = Z5 + D;
      }

    Z = Z0 + Z1 * t + Z2 * t2 + Z3 * t3 + Z4 * t4 + Z5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    JupHZ = Z;

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

  } // END - clc_juplon ()

// JUPITER SPHERICAL COORDINATES:
// Data in JUPID.DAT

void clc_juplon ( double t )
  {
    // External data: double JupHL, JupHB, JupHR;
    // Jupiter heliocentric long, lat, radius.

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

    L0 = 0.59954691495;  // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 759; i >= 1; --i )   // AND THESE
//  for ( i = 1; i < 760; ++i )    // AND THESE
      {
        Alpha = JuL0P[i][1] + JuL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }

    L0 = L0 + SL;

    L1 = 529.93480757497;       // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 368; i >= 1; --i )
//  for ( i = 1; i < 369; ++i )
      {
        Alpha = JuL1P[i][1] + JuL1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuL1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L1 = L1 + SL;

    SL = 0.0;
    for ( i = 190; i >= 0; --i )
//  for ( i = 0; i < 191; ++i )
      {
        Alpha = JuL2P[i][1] + JuL2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuL2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L2 = SL;

    SL = 0.0;
    for ( i = 108; i >= 0; --i )
//  for ( i = 0; i < 109; ++i )
      {
        Alpha = JuL3P[i][1] + JuL3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuL3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L3 = SL;

    SL = 0.0;
    for ( i = 44; i >= 0; --i )
//  for ( i = 0; i < 45; ++i )
      {
        Alpha = JuL4P[i][1] + JuL4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuL4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L4 = SL;

    SL = 0.0;
    for ( i = 9; i >= 0; --i )
//  for ( i = 0; i < 10; ++i )
      {
        Alpha = JuL5P[i][1] + JuL5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuL5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L5 = SL;

    L = L0 + L1 * t + L2 * t2 + L3 * t3 + L4 * t4 + L5 * t5;

    L = modfeh ( L, TwoPi );
    if ( L < 0.0 )
      L = L + TwoPi;

    JupHL = L;

// NEXT - HELIOCENTRIC LATITUDE:

    SL = 0.0;
    for ( i = 248; i >= 0; --i )
//  for ( i = 0; i < 249; ++i )
      {
        Alpha = JuB0P[i][1] + JuB0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuB0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B0 = SL;

    SL = 0.0;
    for ( i = 140; i >= 0; --i )
//  for ( i = 0; i < 141; ++i )
      {
        Alpha = JuB1P[i][1] + JuB1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuB1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B1 = SL;

    SL = 0.0;
    for ( i = 80; i >= 0; --i )
//  for ( i = 0; i < 81; ++i )
      {
        Alpha = JuB2P[i][1] + JuB2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuB2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B2 = SL;

    SL = 0.0;
    for ( i = 41; i >= 0; --i )
//  for ( i = 0; i < 42; ++i )
      {
        Alpha = JuB3P[i][1] + JuB3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuB3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B3 = SL;

    SL = 0.0;
    for ( i = 11; i >= 0; --i )
//  for ( i = 0; i < 12; ++i )
      {
        Alpha = JuB4P[i][1] + JuB4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuB4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B4 = SL;

    SL = 0.0;
    for ( i = 4; i >= 0; --i )
//  for ( i = 0; i < 5; ++i )
      {
        Alpha = JuB5P[i][1] + JuB5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuB5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B5 = SL;

    B = B0 + B1 * t + B2 * t2 + B3 * t3 + B4 * t4 + B5 * t5;
//  B = modfeh ( B, TwoPi );
    JupHB = B;

// NEXT - HELIOCENTRIC RADIUS VECTOR:

    R0 = 5.20887429471;    // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 744; i >= 1; --i )
//  for ( i = 1; i < 745; ++i )
      {
        Alpha = JuR0P[i][1] + JuR0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuR0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R0 = R0 + SL;

    SL = 0.0;
    for ( i = 380; i >= 0; --i )
//  for ( i = 0; i < 381; ++i )
      {
        Alpha = JuR1P[i][1] + JuR1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuR1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R1 = SL;

    SL = 0.0;
    for ( i = 189; i >= 0; --i )
//  for ( i = 0; i < 190; ++i )
      {
        Alpha = JuR2P[i][1] + JuR2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuR2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R2 = SL;

    SL = 0.0;
    for ( i = 97; i >= 0; --i )
//  for ( i = 0; i < 98; ++i )
      {
        Alpha = JuR3P[i][1] + JuR3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuR3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R3 = SL;

    SL = 0.0;
    for ( i = 45; i >= 0; --i )
//  for ( i = 0; i < 46; ++i )
      {
        Alpha = JuR4P[i][1] + JuR4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuR4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R4 = SL;

    SL = 0.0;
    for ( i = 8; i >= 0; --i )
//  for ( i = 0; i < 9; ++i )
      {
        Alpha = JuR5P[i][1] + JuR5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = JuR5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R5 = SL;
    R = R0 + R1 * t + R2 * t2 + R3 * t3 + R4 * t4 + R5 * t5;
    JupHR = R;
  } // - clc_juplon ()
                                             