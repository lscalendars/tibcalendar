#include "astronomy.h"
#include "vsop.h"

/****************************************
 *** General introduction: timescales ***
 ****************************************
 *
 * This explanation is bad and might contain errors due to my very limited
 * knowledge on the topic, but I think it might yet be useful. If you want to
 * go further, please read the articles given in the links. 
 *
 * First it's important to consider some definitions:
 *
 *  - Atomic time: the time measured by the atomic clocks, in other words
 *      the highly constant "tick" of all non-relativistic timescales (the
 *      second unit is defined by this atomic time). TAI is an atomic timescale
 *      which means it doesn't depend on the length of the day or anything
 *      related to our experience.
 *
 *  - Earth Rotation time: the time of our experience of the time on Earth,
 *      with days, sunrises, sunsets, etc. In this time, two timescales are very
 *      important: 
 *       * UT1, the "natural" time that varies according to some highly complex
 *         mechanisms and that we can only measure and interpolate, not predict
 *         with high precision.
 *       * UTC, the time you have on your computer, which is the TAI corrected
 *         by leap seconds so that |UT1-UTC|<0.9s. Leap seconds are thus added
 *         or removed according to day length duration, with limited
 *         predictability.
 *
 *  - Dynamical time: it are approximations of the "absolute" time in which the
 *      Physics formulae occur. This takes into account many things I don't
 *      understand about relativity. A famous one was called ET (Ephemeris Time)
 *      but has been replaced by TDB (Barycentric Dynamical Time). TDT
 *      (Terrestial Dynamical Time) is same as TDB but viewed from the Earth,
 *      it differs from TDB by at most a few miliseconds.
 *      This topic is simply too complex, and, as we'll see below, is not that
 *      important for calendrical calculations.
 *
 * Now, what is the time we are using? For the Calendrical calculations (Tibetan
 * but not only), Local Mean Solar Time (LMST) is used. Though UT1 went through
 * several definitions and is now disconnected from the sun [1], its use as LMST
 * is still relevant, and this is what we will do. So all our data will
 * be in UT1 timescale. It is important here to notice that there is a small
 * discrepancy of maximum 0.9s between UTC and UT1, so computations made
 * with UTC (for instance real time) will be less precise. You can
 * find some functions in the SOFA software[2] to get conversion between the two.
 *
 * Internally, we will do some ephemeris calculations, and ephemeris algorithms
 * (at least the ones we use) are made to work with TDB, so we should have a
 * function to convert UT1 from TDB. The problem here is that there is no 
 * function to convert an unpredictable time to a relativistic time! The first
 * solution to this is to consider that we are not computing high-precision
 * ephemeris for outer system bodies, and thus we can approximate TDB by TAI + a
 * constant (32.184s). This is what is advised by the authors of
 * VSOP2013[3, p.4]. So the main problem is to get an apprxomation of the
 * difference between UT1 and approximate TDT/TDB (a value called Delta T).
 * The NASA has done it for us by interpolating the measures of UT1
 * discrepancy[4][5]. This is what this function is doing.
 *
 * To have a better explanation about timescales, see:
 *  - http://stjarnhimlen.se/comp/time.html
 *  - http://www.ucolick.org/~sla/leapsecs/timescales.html
 *  - http://www.jstor.org/stable/10.1086/655938
 * 
 * And about DeltaT:
 *  - http://www.ucolick.org/~sla/leapsecs/deltat.html
 *  - http://user.online.be/felixverbelen/dt.htm
 * 
 * [1]: http://www.ucolick.org/~sla/leapsecs/dutc.html#UT1
 * [2]: http://www.iausofa.org/
 * [3]: ftp://ftp.imcce.fr/pub/ephem/planets/vsop2013/solution/README.pdf
 * [4]: http://eclipse.gsfc.nasa.gov/SEcat5/deltat.html
 * [5]: http://eclipse.gsfc.nasa.gov/SEcat5/deltatpoly.html
 *
 *********************
 *** This function ***
 *********************
 *
 * This function computed the Delta T value for a julian day, and gives the 
 * result in seconds. It is a C implementation of [5]. Note that this function
 * is rather time consuming (though not as much as ephemeris calculations) and
 * gives a result according to the day; so you can save time by calling this
 * function only once for each day where you do ephemeris calculations.
 *
 */
double get_deltaT ( double jd )
{
  double dT;
  double y;

  int wy, wm, wd, wdow;
  // retrieving gregorian calendar date
  jd_to_wd ((long int) floor(jd + 0.5), &wd, &wm, &wy, &wdow);

  // y is in fraction of year, for interpolation
  y = (double) wy + ((double) ( wm - 1 ) )/ 12.0 + ((double) (wd - 1 ))/ 360.0;
  
  // deltaT could be a little more precise if we substract jd_factor, but it seems
  // totally unnecessary

  if ( y < -500.0 )
    {
      y = (y - 1820.0 )/100.0;
      dT = -20.0 + 32.0 * y * y;
    }
  else if ( y >= -500.0 && y < 500.0 )
    {
      y = y / 100.0;
      dT = 10583.6 + y * (-1014.41 + y * (33.78311 + y * (-5.952053 + y * (-0.1798452 + y * (0.022174192 + y * 0.0090316521)))));
    }
  else if ( y >= 500.0 && y < 1600.0 )
    {
      y = ( y - 1000.0 ) / 100.0;
      dT = 1574.2 + y * (-556.01 + y * (71.23472 + y * (0.319781 +y * (-0.8503463 +y * (-0.005050998 + y * 0.0083572073)))));
    }
  else if ( y >= 1600.0 && y < 1700.0 )
    {
      y = y - 1600.0;
      dT = 120.0 + y * (-0.9808 + y * (-0.01532 + y / 7129.0));
    }
  else if ( y >= 1700.0 && y < 1800.0 )
    {
      y = y - 1700.0;
      dT = 8.83 + y * (0.1603 + y * (-0.0059285 + y * (0.00013336 - y / 1174000.0)));
    }
  else if ( y >= 1800.0 && y < 1860.0 )
    {
      y = y - 1800.0;
      dT = 13.72 + y * (-0.332447 + y * (0.0068612 + y * (0.0041116 + y * (-0.00037436 + y * (0.0000121272 + y * (-0.0000001699 + y * 0.000000000875))))));
    }
  else if ( y >= 1860.0 && y < 1900.0 )
    {
      y = y - 1860.0;
      dT = 7.62 + y * (0.5737 + y * (-0.251754 + y * (0.01680668 + y * (-0.0004473624 + y / 233174.0))));
    }
  else if ( y >= 1900.0 && y < 1920.0 )
    {
      y = y - 1900.0;
      dT = -2.79 + y * (1.494119 +y * (-0.0598939 + y * (0.0061966 - y * 0.000197)));
    }
  else if ( y >= 1920.0 && y < 1941.0 )
    {
      y = y - 1920.0;
      dT = 21.20 + y * (0.84493 + y * (-0.076100 + y * 0.0020936));
    }
  else if ( y >= 1941.0 && y < 1961.0 )
    {
      y = y - 1950.0;
      dT = 29.07 + y * (0.407 + y * (-1.0/233.0 + y / 2547.0));
    }
  else if ( y >= 1961.0 && y < 1986.0 )
    {
      y = y - 1975.0;
      dT = 45.45 + y * (1.067 + y * (-1.0/260.0 - y / 718.0));
    }
  else if ( y >= 1986.0 && y < 2005.0 )
    {
      y = y - 2000.0;
      dT = 63.86 + y * (0.3345 + y * (-0.060374 + y * (0.0017275 + y * (0.000651814 + y * 0.00002373599))));
    }
  else if ( y >= 2005.0 && y < 2050.0 )
    {
      y = y - 2000.0;
      dT = 62.92 + y * (0.32217 + y * 0.005589);
    }
  else if ( y >= 2050.0 && y < 2150.0 )
    {
      dT = -20.0 + 32.0 * ((y-1820.0)/100.0) * ((y-1820.0)/100.0)
           - 0.5628 * (2150.0 - y);
    }
  else if ( y >= 2150.0 )
    {
      y = ( y - 1820.0 ) / 100.0;
      dT = -20.0 + 32.0 * y * y;
    }
  return (dT);
}

time_t julian_date_to_julian_millenia(double jd) {
    double t;
    t = (jd - 2451545.0); // date is now modified julian with epoch J2000, MJD2000
    t = (t + get_deltaT (jd) / 86400.0); // We add the deltaT, converting it from seconds to fraction of day
    return t / 365250.0;  // This is now in Julian millenia.
}
