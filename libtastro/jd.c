/*********************************************************************************
License for Tibastro - Tibetan astrlogy software

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

#include "jd.h"


long int
wd_to_jd (int day, int month, int year)
{

// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

  if (year>1582 || (year==1582 && (month > 10 || (month ==10 && day >=15))))
    {
      // - the following is from ESAA, 1992, p. 604.
      return (long int) day +
	(1461L * ((long int) year + 4800L + ((long int) month - 14L) / 12L)) /
	4L + (367L * ((long int) month - 2L -
	  12L * (((long int) month - 14L) / 12L))) / 12L -
	(3L * (((long int) year + 4900L +
	   ((long int) month - 14) / 12L) / 100L)) / 4L - 32075L;
    }
  else				// Julian:
    {
      return 367L * (long int) year -
	(7L * ((long int) year + 5001L + ((long int) month - 9L) / 7L)) / 4L +
	(275L * (long int) month) / 9L + (long int) day + 1729777L;
    }

}

// test=0 for using long long int and 1 for bcd
void
jd_to_wd (long int jd, western_date * westdate)
{
  long int l, n, j, k, i, doweek, wy, wm, wd;

// This algorithm is from photcopied notes, from James Neely.
// Also, checked with ESAA, 1992, p. 604
// Calculates date, at noon on which the Julian date starts.
// Julian 0 starts Greenwich mean noon on 1st Jan 4713 BC, Julian proleptic
// calendar.
// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

// First, get day of week:

  doweek = jd - 7L * ((jd + 1) / 7L) + 2L;
  if (doweek == 7L)
    doweek = 0L;

  if (jd >= 2299161L)		// Gregorian calendar:
    {
    // This has been tested between March 1, 1600 and Jan 31, 2100
      l = jd + 68569L;
      n = (4L * l) / 146097L;
      l = l - (146097L * n + 3L) / 4L;
      
      // maximum value for l : 2305843009213693, which will be after the death of the sun...
      wy = (long int) (4000LL * ((long long int) l + 1LL) / 1461001LL);

      l = l - (1461L * wy) / 4L + 31L;
      wm = (80L * l) / 2447L;
      wd = l - (2447L * wm) / 80L;
      l = wm / 11L;
      wm = wm + 2L - 12L * l;
      wy = 100L * (n - 49L) + wy + l;

// j = month, k = day, i = year
    }
  else				// Julian calendar
    {
      j = jd + 1402;
      k = (j - 1) / 1461;
      l = j - 1461 * k;
      n = (l - 1) / 365 - l / 1461;
      i = l - 365 * n + 30;
      j = (80 * i) / 2447;
      wd = i - (2447 * j) / 80;
      i = j / 11;
      wm = j + 2 - 12 * i;
      wy = 4 * k + n + i - 4716;
    }

  westdate->year = (int) wy;
  westdate->month = (int) wm;
  westdate->day = (int) wd;
  westdate->dow = (int) doweek;

}
