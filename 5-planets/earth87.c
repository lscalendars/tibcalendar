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
#include "earthc.dat"
#include "earthd.dat"

double EarHL, EarHB, EarHR; // Earth, heliocentric long, lat, radius.
double EarHX, EarHY, EarHZ; // Earth, heliocentric x, y, z.

extern double TwoPi;

void clc_earlon ( double );
void clc_earrct ( double );
double modfeh ( double, double );

void clc_earlon ( double t ) // t is Julian millenia, ET.
  {
    double SL;
    double L0, L1, L2, L3, L4, L5;
    double B0, B1, B2, B3, B4;
    double R0, R1, R2, R3, R4, R5;
    double D, Alpha, t2, t3, t4, t5;
    int i;

    t2 = t * t;
    t3 = t * t2;
    t4 = t2 * t2;
    t5 = t2 * t3;

    L0 = 1.75347045673;         // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 558; i >= 1; --i )
      {
        Alpha = EaL0P[i][1] + EaL0P[i][2] * t;
        Alpha = modfeh ( Alpha, TwoPi );
        D = EaL0P[i][0] * cos ( Alpha );
        SL = SL + D;
      }
    L0 = L0 + SL;

    L1 = 6283.31966747491;      // DIFFERENT FOR EACH PLANET
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

// NOW, LATITUDE:

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

// NEXT - HELIOCENTRIC RADIUS VECTOR:

    R0 = 1.00013988799;         // DIFFERENT FOR EACH PLANET
    SL = 0.0;
    for ( i = 525; i >= 1; --i )
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
    for ( i = 9; i >= 0; --i ) // 10 terms
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
  } // END - clc_earlon ()

// RECTANGULAR COORDINATES:

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

// NEXT - Y;

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

// NEXT - Z:

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
                                                    