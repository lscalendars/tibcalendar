/* ECLIPSE CALCULATOR */

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
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <math.h>
#include "jd.h"

double modfpos ( double, double );
int chk_eclipse ( double, int );

extern int  d_d, m_m, y_y;
extern char eclps_str[80];
extern double rad;

struct eclipse_details
  {
    int partial;
    int annular;
    int total;
    float mag;
  } eclps;

// This version always returns a positive result:
double modfpos ( double x, double y )
  {
    double N;
    if ( x > 0.0 )
      {
        N = floor ( x / y );
        return ( x - N * y );
      }
    N = ceil ( x / y );
    return ( y + x - N * y );
  } // END - modfpos ()

// Main eclipse calculation. Astronomical Algorithms, Jean Meeus, p. 379, etc.
int chk_eclipse ( double jd, int tt )
  {
    // tt is lunar date, just for recognition of type of eclipse
    double kayf, T, T2, T3, T4, x, juldat, umrad;
    double M, Mdash, F, N, E, F1, A1, Tdif, P, Q, W, Gamma, modGamma, u;

    rad = 3.141592653589793 / 180.0;
    eclps_str[0] = '\0';

    x = jd - 1683754.856874;
    if ( tt == 15 )
      x = x - 14.765;
    // Find approximate number of months:
    x = x / 29.530588861 + 0.1;
    kayf = floor ( x ) - 26000.0;
    if ( tt == 15 )
    kayf = kayf + 0.5;

    T = ( jd - 2451545.0 ) / 36525.0;
    T2 = T * T;
    T3 = T2 * T;
    T4 = T2 * T;

    juldat = 2451550.09766 + 29.530588861 * kayf
             + 0.00015437 * T2
             - 0.000000150 * T3
             + 0.00000000073 * T4;

    M = 2.5534 + 29.10535670 * kayf
        - 0.0000014 * T2
        - 0.00000011 * T3;
    M = modfpos ( M, 360.0 );

    Mdash = 201.5643 + 385.81693528 * kayf
            + 0.0107582 * T2
            + 0.00001238 * T3
            - 0.000000058 * T4;
    Mdash = modfpos ( Mdash, 360.0 );

    F = 160.7108 + 390.67050284 * kayf
        - 0.0016118 * T2
        - 0.00000227 * T3
        + 0.000000011 * T4;
    F = modfpos ( F, 360.0 );

    N = 124.7746 - 1.56375588 * kayf
        + 0.0020672 * T2
        + 0.00000215 * T3;
    N = modfpos ( N, 360.0 );

    E = 1.0 - 0.002516 * T - 0.0000074 * T2; // Meuss, p. 338

    F1 = F - 0.02665 * sin ( N * rad );

    A1 = 299.77 + 0.107408 * kayf - 0.009173 * T2;
    A1 = modfpos ( A1, 360.0 );

    if ( tt == 15 ) // Lunar eclipse possibility
      {
        Tdif = -0.4065 * sin ( Mdash * rad )
               + 0.1727 * E * sin ( M * rad );
      }
    else if ( tt == 30 ) // Solar eclipse possibility
      {
        Tdif = -0.4075 * sin ( Mdash * rad )
               + 0.1721 * E * sin ( M * rad );
      }
    Tdif = Tdif + 0.0161 * sin ( 2 * Mdash * rad )
         - 0.0097 * sin ( 2 * F1 * rad )
         + 0.0073 * E * sin ( (Mdash-M) * rad )
         - 0.0050 * E * sin ( (Mdash+M) * rad )
         - 0.0023 * sin ( (Mdash-2.0*F1 ) * rad )
         + 0.0021 * E * sin ( 2.0 * M * rad )
         + 0.0012 * sin ( (Mdash+2.0*F1) * rad )
         + 0.0006 * E * sin ( (2.0*Mdash+M) * rad )
         - 0.0004 * sin ( 3.0 * Mdash * rad )
         - 0.0003 * E * sin ( (M+2.0*F1) * rad )
         + 0.0003 * sin ( A1 * rad )
         - 0.0002 * E * sin ( (M-2.0*F1) * rad )
         - 0.0002 * E * sin ( (2.0*Mdash-M) * rad )
         - 0.0002 * sin ( N * rad );

// Astronomical Algorithms, Jean Meeus, p. 381:

    P = 0.2070 * E * sin ( M * rad )
        + 0.0024 * E * sin ( 2.0 * M * rad )
        - 0.0392 * sin ( Mdash * rad )
        + 0.0116 * sin ( 2.0 * Mdash * rad )
        - 0.0073 * E * sin ( (Mdash+M) * rad )
        + 0.0067 * E * sin ( (Mdash-M) * rad )
        + 0.0118 * sin ( 2.0 * F1 * rad );

    Q = 5.2207 - 0.0048 * E * cos ( M * rad )
        + 0.0020 * E * cos ( 2.0 * M * rad )
        - 0.3299 * cos ( Mdash * rad )
        - 0.0060 * E * cos ( (Mdash+M) * rad )
        + 0.0041 * E * cos ( (Mdash-M) * rad );

    W = cos ( F1 * rad );
    if ( W < 0.0 )
      W = -W;

    Gamma = ( P * cos ( F1 * rad ) + Q * sin ( F1 * rad ) ) *
            ( 1.0 - 0.0048 * W );

    u = 0.0059 + 0.0046 * E * cos ( M * rad )
        - 0.0182 * cos ( Mdash * rad )
        + 0.0004 * cos ( 2.0 * Mdash * rad )
        - 0.0005 * cos ( (M+Mdash) * rad );

    x = sin ( F * rad );
    if ( x < 0.0 )
      x = -x;
    if ( x > 0.36 )
      return (0);   // Certainly no eclipse.

    eclps.partial = 0;
    eclps.annular = 0;
    eclps.total = 0;

    modGamma = Gamma;
    if ( Gamma < 0.0 )
      modGamma = -Gamma;

    if ( tt == 15 ) // Lunar eclipse possibility
      {
        umrad = 0.7403 - u;
        eclps.mag = ( 1.0128 - u - modGamma ) / 0.5450;
        if ( eclps.mag <= 0.0 )
          return (0);  // No eclipse
        if ( eclps.mag >= 1.0 )
          sprintf ( eclps_str, "Total Lunar Eclipse, mag: %.2f", eclps.mag );
        else
          sprintf ( eclps_str, "Partial Lunar Eclipse, mag: %.2f", eclps.mag );
        if ( eclps.mag >= 1.0 )
          eclps.total = 1;
        else
          eclps.partial = 1;
      }
    else if ( tt == 30 ) // Solar eclipse possibility
      {
        if ( modGamma > ( 1.5433 + u ) )
          return (0); // No eclipse visible from Earth's surface
        if ( modGamma > 0.9972 && modGamma < (1.5433+u) )
          {
            eclps.partial = 1;
            eclps.mag = ( 1.5433 + u - modGamma ) / ( 0.5461 + 2.0 * u );
            sprintf ( eclps_str, "Partial Solar Eclipse, max mag: %.2f.",
                                                 eclps.mag );
          }
        if ( modGamma <= 0.9972 )
          {
            eclps.total = 1;
            if ( u > 0.0 )
              {
                eclps.annular = 1;
                sprintf ( eclps_str, "Annular Solar Eclipse." );
              }
            else
              {
                eclps.total = 1;
                sprintf ( eclps_str, "Total Solar Eclipse." );
              }
          }
      }
    return (1);
  }
