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

#include <math.h>
#include <time.h>
#include "jd.h"
#include "bcd.h"

double AINT ( double );
void UTET ( int, int, int, int, int, int, double *, double * );
void jul_chk ( void );
long int gregd2jul ( long int, long int, long int );
void jul2date ( long int, struct tmst * );

extern signed char bcda[BCDMAX];

struct hold_struc
  {
    int     y;
    double  dt;
  };

// NASA values, from "Historical Values of Delta T":
double dtN[18] = { 10600.0, 9600.0, 8600.0, 7700.0, 6700.0, 5700.0, 4700.0,
                   800.0, 3000.0, 2200.0, 1600.0, 1100.0, 750.0, 470.0, 300.0,
                   180.0, 110.0, 7.0 };

// These are from 1620 to 2000 inclusive, by even years:
double dtm[191] = { 121.0, 112.0, 103.0, 95.0, 88.0, 82.0, 77.0, 72.0, 68.0,
 63.0, 60.0, 56.0, 53.0, 51.0, 48.0, 46.0, 44.0, 42.0, 40.0, 38.0, 35.0, 33.0,
 31.0, 29.0, 26.0, 24.0, 22.0, 20.0, 18.0, 16.0, 14.0, 12.0, 11.0, 10.0, 9.0,
  8.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 8.0, 8.0, 9.0, 9.0, 9.0, 9.0, 9.0, 10.0,
 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 11.0, 11.0, 11.0, 11.0, 11.0, 12.0,
 12.0, 12.0, 12.0, 13.0, 13.0, 13.0, 14.0, 14.0, 14.0, 14.0, 15.0, 15.0, 15.0,
 15.0, 15.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 16.0, 15.0, 15.0, 14.0,
 13.0, 13.1, 12.5, 12.2, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 11.9, 11.6, 11.0,
 10.2, 9.2, 8.2, 7.1, 6.2, 5.6, 5.4, 5.3, 5.4, 5.6, 5.9, 6.2, 6.5, 6.8, 7.1,
  7.3, 7.5, 7.6, 7.7, 7.3, 6.2, 5.2, 2.7, 1.4, -1.2, -2.8, -3.8, -4.8, -5.5,
 -5.3, -5.6, -5.7, -5.9, -6.0, -6.3, -6.5, -6.2, -4.7, -2.8, -0.1, 2.6, 5.3,
  7.7, 10.4, 13.3, 16.0, 18.2, 20.2, 21.1, 22.4, 23.5, 23.8, 24.3, 24.0, 23.9,
 23.9, 23.7, 24.0, 24.3, 25.3, 26.2, 27.3, 28.2, 29.1, 30.0, 30.7, 31.4, 32.2,
 33.1, 34.0, 35.0, 36.5, 38.3, 40.2, 42.2, 44.5, 46.5, 48.5, 50.5, 52.2, 53.8,
 54.9, 55.8, 56.9, 58.3, 60.0, 61.6, 63.0, 65.0 };

double get_deltaT ( double JDate )
  {
    int y, c;
    double yfrac, cfrac;
    double T, dT;
    double x;
    // returns a number of seconds.

// For dates after 0 CE only:

    x = ( JDate - 1721058.0 ) / 365.25;  // Gives year number, CE.
    y = (int) x;
    c = y / 100;
    yfrac = x - floor (x);
    cfrac = ( (double) ( y - 100 * c ) + yfrac ) / 100.0;

//  printf ( "Y = %d, Yfrac = %f\n", y, yfrac );

    if ( y >= 1620 && y < 2000 )
      { // This uses data from table at Meeus 79.
        if ( ( y - 1620 ) % 2 == 0 )
          dT = dtm[ ( y - 1620 ) / 2 ];
        else
          dT = ( dtm[ ( y - 1620 ) / 2 ] +
            dtm[ ( y - 1620 ) / 2 + 1 ] ) / 2.0;
      }
    else if ( y == 2000 ) dT = 65.0;  // Some uncertainites about these:
    else if ( y == 2001 ) dT = 66.0;
    else if ( y == 2002 ) dT = 67.0;
    else if ( y == 2003 ) dT = 67.0;
    else if ( y == 2004 ) dT = 68.0;
    else if ( y == 2005 ) dT = 69.0;
    else if ( y < 1620 )                       // If before 948 CE
      {
        dT = dtN[c] + ( dtN[c+1] - dtN[c] ) * cfrac;
      }
//  else if ( y >= 948 && y < 1620 )           // If before 948 CE
//    {
//      T = ( (double) y - 2000.0 ) / 100.0;
//      dT = 102.0 + 102.0 * T + 25.3 * T * T;
//    }
//  else if ( y < 948 ) // If before 948 CE
//    {
//      T = ( (double) y - 2000.0 ) / 100.0;
//      dT = 2177.0 + 497.0 * T + 44.1 * T * T;
//    }
//  printf ( "Delta T = %f\n", dT );
//  getch ();
    return ( dT ); // in seconds.
  } // END - get_deltaT ()

//extern time_t JDtoDate(double jd, struct tm *event_date)
///* convert a Julian Date to a date-time group */
//{
//  long a, a1, z, b, c, d, e;
//  double f, day;

//  jd += 0.5;
//  z = (long) jd;
//  f = jd - z;

//  if (z < 2299161)
//  {
//    a = z;
//  }
//  else
//  {
//    a1 = (long) ((z - 1867216.25) / 36524.25);
//    a = z + 1 + a1 - (long)(a1 / 4);
//  }

//  b = a + 1524;
//  c = (long)((b - 122.1) / 365.25);
//  d = (long)(365.25 * c);
//  e = (long)((b - d)/30.6001);

//  day = b - d - (long)(30.6001 * e) + f;

//  if (e < 14)
//  {
//    event_date->tm_mon = (e - 1) - 1;
//  }
//  else
//  {
//    event_date->tm_mon = (e - 13) - 1;
//  }

//  if (event_date->tm_mon > (2 - 1))
//  {
//    event_date->tm_year = c - 4716 - 1900;
//  }
//  else
//  {
//    event_date->tm_year = c - 4715 - 1900;
//  }
//  event_date->tm_mday = (int)day;
//  day -= event_date->tm_mday;
//  day *= 24;
//  event_date->tm_hour = (int)day;
//  day -= event_date->tm_hour;
//  day *= 60;
//  event_date->tm_min = (int)day;
//  day -= event_date->tm_min;
//  day *= 60;
//  event_date->tm_sec = (int)day;

//  event_date->tm_isdst = -1;
//
//  return mktime(event_date);
//}

//extern double DatetoJD(struct tm *event_date)
///* convert a date-time group to a JD with fraction */
//{
//  int y, m;
//  double d;
//  int a, b;
//  double jd;
//
//  y = event_date->tm_year + 1900;
//  m = event_date->tm_mon + 1;
//  d = (double)(event_date->tm_mday) + (event_date->tm_hour / 24.0)
//    + (event_date->tm_min / 1440.0) + (event_date->tm_sec / 86400.0);

//  if (m == 1 || m == 2)
//  {
//    y--;
//    m += 12;
//  }
//
//  a = (int)(y / 100);
//  b = 2 - a + (int)(a / 4);
//
//  if (y < 1583)
//    if ((y < 1582) || (m < 10) || ((m == 10) && (d <= 15)))
//      b = 0;
//
//  jd = (long)(365.25 * (y + 4716)) + (long)(30.6001 * (m+1))
//     + d + b - 1524.5;
//
//  return jd;
//}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

// These are for intermediate calculations only:

extern signed char bcdx0[BCDMAX];
extern signed char bcdx1[BCDMAX];
extern signed char bcdx2[BCDMAX];
extern signed char bcdx3[BCDMAX];
extern signed char bcdx4[BCDMAX];
extern signed char bcdx5[BCDMAX];
extern signed char bcary[BCDMAX];

long int juldat; /** Julian day number **/
long int wd, wm, wy; /** Western dates **/
long int          doweek;      // Day of the week
extern char *dayoweek[7]; // 0 = Saturday

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void jul_chk ( void )
{
  long int jd, d, m, y, julday2;
  int more = 1;
// These routines checked from JD 0 to year 10,000 AD. Not checked for
// individual date accuracy yet.
//  jd = 2305447L + 60L;
//  jd = 2298000L;
//  jd = 1721000L;
  jd = 0L;
  do
    {
//    jul2date ( jd );

    if ( jd % 500L == 0L )
      printf ( "JD %ld = %ld/%ld/%ld\n", jd, wd, wm, wy );

    d = wd; m = wm; y = wy;
    wd = 0L; wm = 0L; wy = 0L;

    julday2 = gregd2jul ( d, m, y );
    if ( jd != julday2 )
      {
        printf ( "ERROR: JD = %ld, JULDAY2 = %ld\n", jd, julday2 );
        getch ();
      }
//  jd = jd + 167L;
    jd = jd + 1L;

    } while ( more );
}

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

long int gregd2jul ( long int D, long int M, long int Y )
{
  long int JD;
  int calndar;  // Gregorian = 1.

// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

  if ( Y > 1582L )
    calndar = 1;
  else if ( Y < 1582L )
    calndar = 0;
  else
    {
      if ( M > 10L )
        calndar = 1;
      else if ( M < 10L )
        calndar = 0;
      else
        {
          if ( D >= 15L )
            calndar = 1;
          else
            calndar = 0;
        }
    }
  if ( calndar )  // Gregorian:
    {
      // - the following is from ESAA, 1992, p. 604.
      JD = D + ( 1461 * ( Y + 4800L + ( M - 14L ) / 12L ) ) / 4L
         + ( 367L * ( M - 2L - 12L * ( ( M - 14L ) / 12L ) ) ) / 12L
         - ( 3L * ( ( Y + 4900L + ( M - 14 ) / 12L ) / 100L ) ) / 4L - 32075L;
    }
  else  // Julian:
    {
      JD = 367L * Y - ( 7L * ( Y + 5001L + ( M - 9L ) / 7L )) / 4L
           + ( 275L * M ) / 9L + D + 1729777L;
    }

  return ( JD );
}

void jul2date ( long int jd, struct tmst *te )
  {
    long int l, n, j, k, i;

// This algorithm is from photcopied notes, from James Neely.
// Also, checked with ESAA, 1992, p. 604
// Calculates date, at noon on which the Julian date starts.
// Julian 0 starts Greenwich mean noon on 1st Jan 4713 BC, Julian proleptic
// calendar.
// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

// Metonic cycle - 235 lunations occur in 19 years.

// First, get day of week:

  doweek = jd - 7L * (( jd + 1 ) / 7L ) + 2L;
  if ( doweek == 7L )
    doweek = 0L;
  if ( doweek > 7L )
    {
      printf ( "ERROR IN DAY OF WEEK ROUTINE:\n" );
      getch ();
    }

  if ( jd >= 2299161L )  // Gregorian calendar:
    {

// This has been tested between March 1, 1600 and Jan 31, 2100

    l = jd + 68569L;
    n = ( 4L * l ) / 146097L;
    l = l - ( 146097L * n + 3L ) / 4L;

//      wy = 4000L * ( l + 1L ) / 1461001L;

    l2bcd ( bcda, 4000L );

    mulbcdl ( bcda, bcda, l + 1L );
    divbcdl ( bcda, bcda, 1461001L );

    wy = bcd2l ( bcda );

    l = l - ( 1461L * wy ) / 4L + 31L;
    wm = ( 80L * l ) / 2447L;
    wd = l - ( 2447L * wm ) / 80L;
    l = wm / 11L;
    wm = wm + 2L - 12L * l;
    wy = 100L * ( n - 49L ) + wy + l;

// j = month, k = day, i = year
    }

  else // Julian calendar

    {
      j = jd + 1402L;
      k = ( j - 1L ) / 1461L;
      l = j - 1461L * k;
      n = ( l - 1L ) / 365L - l / 1461L;
      i = l - 365L * n + 30L;
      j = ( 80L * i ) / 2447L;
      wd = i - ( 2447L * j ) / 80L;
      i = j / 11L;
      wm = j + 2L - 12L * i;
      wy = 4L * k + n + i - 4716L;
    }

    te->yy = (int) wy;
    te->mm = (int) wm;
    te->dd = (int) wd;
    te->dow = (int) doweek;

  } // END - jul2date ( )

void UTET ( int IYEAR, int MONTH, int JDAY, int IH, int IM, int IS,
            double *UT, double *ET )
  {
    int early = 1;
    double Y, y;
    double M;
    double DELTAT;
    double A;
    double B;
    double C;
    double D;
    double x;
    int i, a, b;
//  double DJ1800 = 2378497.0;

    y = (double) IYEAR;
    if ( MONTH > 2 )
      {
        Y = (double) IYEAR;
        M = (double) MONTH;
      }
    else
      {
        Y = (double) IYEAR - 1.0;
        M = (double) MONTH + 12;
      }
    if ( Y < 0.0 )
      C = -0.75;
    else
      C = 0.0;
    if ( IYEAR >= 1582 )
      {
        if ( IYEAR > 1582 )
          early = 0;
        else if ( MONTH > 10 )
          early = 0;
        else if ( MONTH == 10 && JDAY >=15 )
          early = 0;
      }
    if ( early ) // Dates before 15/10/1582
      {
        B = 0.0;
      }
    else
      {
        A = AINT ( Y / 100.0 );
        B = 2.0 - A + AINT ( A / 4.0 );
      }
    D = (double) IH / 24.0 + (double) IM / 1440.0 + (double) IS / 86400.0;

    *UT = AINT ( 365.25 * Y + C ) + AINT ( 30.6001 * ( M + 1.0 ) )
         + (double) JDAY + D + 1720994.5 + B;

//  getch ();
// Now, computation of ET:

   DELTAT = get_deltaT ( *UT );
   goto finishoff;

// Delta T from Meeus, 78:

   x = ( y - 2000.0 ) / 100.0;

   if ( y < 948.0 )
     DELTAT = 2177.0 + 497.0 * x + 44.1 * x * x;
   else if ( y < 1601.0 || y > 1999.0 )
     {
       DELTAT = 102.0 + 102.0 * x + 25.3 * x * x;
       if ( y > 1999.0 )
         DELTAT = DELTAT + 0.37 * ( y - 2100.0 );
     }
   else // for 1601 to 2000
     {
       if ( y < 1620.0 )
         DELTAT = 110.0;  // No real figures available.
       else
// This uses data from table at Meeus 79.
       {
         if ( ( IYEAR - 1620 ) % 2 == 0 )
           DELTAT = dtm[ ( IYEAR - 1620 ) / 2 ];
         else
           DELTAT = ( dtm[ ( IYEAR - 1620 ) / 2 ] +
           dtm[ ( IYEAR - 1620 ) / 2 + 1 ] ) / 2.0;
       }
     }

// Delta T formula from Bresim:

//  x = ( *UT - 2378497.0 ) / 36525.0 - 0.1;

//  DELTAT = ( -15.0 + 32.5 * x * x ) / 86400.0;

//             ( ( *UT - DJ1800 ) / 36525.0 - 0.1 ) *
//             ( ( *UT - DJ1800 ) / 36525.0 - 0.1 )

finishoff:

    *ET = *UT + DELTAT / 86400.0;

    printf ( "Date %d-%d-%d - UT: %f, DeltaT = %f secs\n", JDAY, MONTH, IYEAR,
             *UT, DELTAT );
//           *UT, DELTAT * 86400.0 );
  }

double AINT ( double x )
  {
    if ( x < 0.0 )
      return ( ceil ( x ) );
    return ( floor ( x ) );
  }
      