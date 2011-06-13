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
#include "venuc.dat"
#include "venud.dat"

double VenHL, VenHB, VenHR; // heliocentric long, lat, radius.
double VenHX, VenHY, VenHZ; // heliocentric x, y, z.

extern double TwoPi; // = 6.283185307179586476;

double modfeh ( double, double );

void clc_venlon ( double );
void clc_venrct ( double );

// VENUS RECTANGULAR COORDINATES:
// Data in VENUC.DAT

void clc_venrct ( double t )
  {
    // External data: double VenHX, VenHY, VenHZ;
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
    for ( i = 684; i >= 0; --i )
//  for ( i = 0; i < 685; ++i )
      {
        Alpha = VeX0P[i][1] + VeX0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeX0P[i][0] * cos ( Alpha );
        X0 = X0 + D;
      }

    X1 = 0.0;
    for ( i = 405; i >= 0; --i )
//  for ( i = 0; i < 406; ++i )
      {
        Alpha = VeX1P[i][1] + VeX1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeX1P[i][0] * cos ( Alpha );
        X1 = X1 + D;
      }

    X2 = 0.0;
    for ( i = 132; i >= 0; --i )
//  for ( i = 0; i < 133; ++i )
      {
        Alpha = VeX2P[i][1] + VeX2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeX2P[i][0] * cos ( Alpha );
        X2 = X2 + D;
      }

    X3 = 0.0;
    for ( i = 24; i >= 0; --i )
//  for ( i = 0; i < 25; ++i )
      {
        Alpha = VeX3P[i][1] + VeX3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeX3P[i][0] * cos ( Alpha );
        X3 = X3 + D;
      }

    X4 = 0.0;
    for ( i = 11; i >= 0; --i )
//  for ( i = 0; i < 12; ++i )
      {
        Alpha = VeX4P[i][1] + VeX4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeX4P[i][0] * cos ( Alpha );
        X4 = X4 + D;
      }

    X5 = 0.0;
    for ( i = 3; i >= 0; --i )
//  for ( i = 0; i < 4; ++i )
      {
        Alpha = VeX5P[i][1] + VeX5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeX5P[i][0] * cos ( Alpha );
        X5 = X5 + D;
      }

    X = X0 + X1 * t + X2 * t2 + X3 * t3 + X4 * t4 + X5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    VenHX = X;

// NEXT - Y - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Y0 = 0.0;
    for ( i = 684; i >= 0; --i )
//  for ( i = 0; i < 685; ++i )
      {
        Alpha = VeY0P[i][1] + VeY0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeY0P[i][0] * cos ( Alpha );
        Y0 = Y0 + D;
      }

    Y1 = 0.0;
    for ( i = 405; i >= 0; --i )
//  for ( i = 0; i < 406; ++i )
      {
        Alpha = VeY1P[i][1] + VeY1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeY1P[i][0] * cos ( Alpha );
        Y1 = Y1 + D;
      }

    Y2 = 0.0;
    for ( i = 132; i >= 0; --i )
//  for ( i = 0; i < 133; ++i )
      {
        Alpha = VeY2P[i][1] + VeY2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeY2P[i][0] * cos ( Alpha );
        Y2 = Y2 + D;
      }

    Y3 = 0.0;
    for ( i = 24; i >= 0; --i )
//  for ( i = 0; i < 25; ++i )
      {
        Alpha = VeY3P[i][1] + VeY3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeY3P[i][0] * cos ( Alpha );
        Y3 = Y3 + D;
      }

    Y4 = 0.0;
    for ( i = 11; i >= 0; --i )
//  for ( i = 0; i < 12; ++i )
      {
        Alpha = VeY4P[i][1] + VeY4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeY4P[i][0] * cos ( Alpha );
        Y4 = Y4 + D;
      }

    Y5 = 0.0;
    for ( i = 3; i >= 0; --i )
//  for ( i = 0; i < 4; ++i )
      {
        Alpha = VeY5P[i][1] + VeY5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeY5P[i][0] * cos ( Alpha );
        Y5 = Y5 + D;
      }

    Y = Y0 + Y1 * t + Y2 * t2 + Y3 * t3 + Y4 * t4 + Y5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    VenHY = Y;

// NEXT - Z - XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    Z0 = 0.0;
    for ( i = 189; i >= 0; --i )
//  for ( i = 0; i < 190; ++i )
      {
        Alpha = VeZ0P[i][1] + VeZ0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeZ0P[i][0] * cos ( Alpha );
        Z0 = Z0 + D;
      }

    Z1 = 0.0;
    for ( i = 116; i >= 0; --i )
//  for ( i = 0; i < 117; ++i )
      {
        Alpha = VeZ1P[i][1] + VeZ1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeZ1P[i][0] * cos ( Alpha );
        Z1 = Z1 + D;
      }

    Z2 = 0.0;
    for ( i = 48; i >= 0; --i )
//  for ( i = 0; i < 49; ++i )
      {
        Alpha = VeZ2P[i][1] + VeZ2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeZ2P[i][0] * cos ( Alpha );
        Z2 = Z2 + D;
      }

    Z3 = 0.0;
    for ( i = 11; i >= 0; --i )
//  for ( i = 0; i < 12; ++i )
      {
        Alpha = VeZ3P[i][1] + VeZ3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeZ3P[i][0] * cos ( Alpha );
        Z3 = Z3 + D;
      }

    Z4 = 0.0;
    for ( i = 2; i >= 0; --i )
//  for ( i = 0; i < 3; ++i )
      {
        Alpha = VeZ4P[i][1] + VeZ4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeZ4P[i][0] * cos ( Alpha );
        Z4 = Z4 + D;
      }

    Z5 = 0.0;
    for ( i = 2; i >= 0; --i )
//  for ( i = 0; i < 3; ++i )
      {
        Alpha = VeZ5P[i][1] + VeZ5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeZ5P[i][0] * cos ( Alpha );
        Z5 = Z5 + D;
      }

    Z = Z0 + Z1 * t + Z2 * t2 + Z3 * t3 + Z4 * t4 + Z5 * t5;

//  L = modfeh ( L, TwoPi );
//  if ( L < 0.0 )
//    L = L + TwoPi;

    VenHZ = Z;

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

  } // END - clc_venlon ()

// VENUS SPHERICAL COORDINATES:
// Data in VENUD.DAT

void clc_venlon ( double t )
  {
    // External data: double VenHL, VenHB, VenHR;
    // Vencury heliocentric long, lat, radius.

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

    L0 = 3.17614666774; // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 366; i >= 1; --i )   // AND THESE
//  for ( i = 1; i < 367; ++i )    // AND THESE
      {
        Alpha = VeL0P[i][1] + VeL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }

    L0 = L0 + SL;

    L1 = 10213.52943052898; // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 214; i >= 1; --i )
//  for ( i = 1; i < 215; ++i )
      {
        Alpha = VeL1P[i][1] + VeL1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeL1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L1 = L1 + SL;

    SL = 0.0;
    for ( i = 69; i >= 0; --i )
//  for ( i = 0; i < 70; ++i )
      {
        Alpha = VeL2P[i][1] + VeL2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeL2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L2 = SL;

    SL = 0.0;
    for ( i = 8; i >= 0; --i )
//  for ( i = 0; i < 9; ++i )
      {
        Alpha = VeL3P[i][1] + VeL3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeL3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L3 = SL;

    SL = 0.0;
    for ( i = 4; i >= 0; --i )
//  for ( i = 0; i < 5; ++i )
      {
        Alpha = VeL4P[i][1] + VeL4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeL4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L4 = SL;

    SL = 0.0;
    for ( i = 4; i >= 0; --i )
//  for ( i = 0; i < 5; ++i )
      {
        Alpha = VeL5P[i][1] + VeL5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeL5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L5 = SL;

    L = L0 + L1 * t + L2 * t2 + L3 * t3 + L4 * t4 + L5 * t5;

    L = modfeh ( L, TwoPi );
    if ( L < 0.0 )
      L = L + TwoPi;

    VenHL = L;

// NEXT - HELIOCENTRIC LATITUDE:

    SL = 0.0;
    for ( i = 209; i >= 0; --i )
//  for ( i = 0; i < 210; ++i )
      {
        Alpha = VeB0P[i][1] + VeB0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeB0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B0 = SL;

    SL = 0.0;
    for ( i = 132; i >= 0; --i )
//  for ( i = 0; i < 133; ++i )
      {
        Alpha = VeB1P[i][1] + VeB1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeB1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B1 = SL;

    SL = 0.0;
    for ( i = 58; i >= 0; --i )
//  for ( i = 0; i < 59; ++i )
      {
        Alpha = VeB2P[i][1] + VeB2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeB2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B2 = SL;

    SL = 0.0;
    for ( i = 14; i >= 0; --i )
//  for ( i = 0; i < 15; ++i )
      {
        Alpha = VeB3P[i][1] + VeB3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeB3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B3 = SL;

    SL = 0.0;
    for ( i = 4; i >= 0; --i )
//  for ( i = 0; i < 5; ++i )
      {
        Alpha = VeB4P[i][1] + VeB4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeB4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B4 = SL;

    SL = 0.0;
    for ( i = 3; i >= 0; --i )
//  for ( i = 0; i < 4; ++i )
      {
        Alpha = VeB5P[i][1] + VeB5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeB5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    B5 = SL;

    B = B0 + B1 * t + B2 * t2 + B3 * t3 + B4 * t4 + B5 * t5;
//  B = modfeh ( B, TwoPi );
    VenHB = B;

// NEXT - HELIOCENTRIC RADIUS VECTOR:

    R0 = 0.72334820905; // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 329; i >= 1; --i )
//  for ( i = 1; i < 330; ++i )
      {
        Alpha = VeR0P[i][1] + VeR0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeR0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R0 = R0 + SL;

    SL = 0.0;
    for ( i = 179; i >= 0; --i )
//  for ( i = 0; i < 180; ++i )
      {
        Alpha = VeR1P[i][1] + VeR1P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeR1P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R1 = SL;

    SL = 0.0;
    for ( i = 62; i >= 0; --i )
//  for ( i = 0; i < 63; ++i )
      {
        Alpha = VeR2P[i][1] + VeR2P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeR2P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R2 = SL;

    SL = 0.0;
    for ( i = 6; i >= 0; --i )
//  for ( i = 0; i < 7; ++i )
      {
        Alpha = VeR3P[i][1] + VeR3P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeR3P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R3 = SL;

    SL = 0.0;
    for ( i = 2; i >= 0; --i )
//  for ( i = 0; i < 3; ++i )
      {
        Alpha = VeR4P[i][1] + VeR4P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeR4P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R4 = SL;

    SL = 0.0;
    for ( i = 1; i >= 0; --i )
//  for ( i = 0; i < 2; ++i )
      {
        Alpha = VeR5P[i][1] + VeR5P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = VeR5P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    R5 = SL;
    R = R0 + R1 * t + R2 * t2 + R3 * t3 + R4 * t4 + R5 * t5;
    VenHR = R;
  } // - clc_venlon ()
                                                                                            