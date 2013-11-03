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
 *      but has been replaced by TDB (Barycentric Dynamical Time). This topic is
 *      simply too complex, and, as we'll see below, is not that important for
 *      calendrical calculations.
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
 * constant. This is what is advised by the authors of VSOP2013[3, p.4]. So the
 * main problem is to get an apprxomation of the difference between UT1 and TAI
 * (a value called Delta T). The NASA has done it for us by interpolating the
 * measures of UT1 discrepancy[4][5]. This is what this function is doing.
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
double get_deltaT ( double JDate )
  {
    double y;
    double u, u2, u3, u4, u5, u6;
    double dT;
    double t, t2, t3, t4, t5, t6, t7;

    int wy, wm, wd, wdow;

    // deltaT could be a little more precise if we substract jd_factor, but it seems
    // totally unnecessary

    jd_to_wd ((long int) floor(JDate + 0.5), &wd, &wm, &wy, &wdow);
    
    y = (double) wy + ((double) ( wm - 1 ) )/ 12.0 + ((double) (wd - 1 ))/ 360.0;

    if ( y < -500.0 )
      {
        u = (y - 1820.0 )/100.0;
        dT = -20.0 + 32.0 * u * u;
      }
    else if ( y >= -500.0 && y < 500.0 )
      {
        u = y / 100.0;
        u2 = u * u;
        u3 = u2 * u;
        u4 = u2 * u2;
        u5 = u2 * u3;
        u6 = u3 * u3;
        dT = 10583.6 - 1014.41 * u + 33.78311 * u2 - 5.952053 * u3
              - 0.1798452 * u4 + 0.022174192 * u5 + 0.0090316521 * u6;
      }
    else if ( y >= 500.0 && y < 1600.0 )
      {
        u = ( y - 1000.0 ) / 100.0;
        u2 = u * u;
        u3 = u2 * u;
        u4 = u2 * u2;
        u5 = u2 * u3;
        u6 = u3 * u3;
        dT = 1574.2 - 556.01 * u + 71.23472 * u2 + 0.319781 * u3
             - 0.8503463 * u4 - 0.005050998 * u5 + 0.0083572073 * u6;
      }
    else if ( y >= 1600.0 && y < 1700.0 )
      {
        t = y - 1600.0;
        t2 = t * t;
        t3 = t * t2;
        dT = 120.0 - 0.9808 * t - 0.01532 * t2 + t3 / 7129.0;
      }
    else if ( y >= 1700.0 && y < 1800.0 )
      {
        t = y - 1700.0;
        t2 = t * t;
        t3 = t * t2;
        t4 = t2 * t2;        
        dT = 8.83 + 0.1603 * t - 0.0059285 * t2 + 0.00013336 * t3 - t4 / 1174000.0;
      }
    else if ( y >= 1800.0 && y < 1860.0 )
      {
        t = y - 1800.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        t5 = t2 * t3;
        t6 = t3 * t3;
        t7 = t3 * t4;
        dT = 13.72 - 0.332447 * t + 0.0068612 * t2 + 0.0041116 * t3 - 0.00037436
             * t4 + 0.0000121272 * t5 - 0.0000001699 * t6 + 0.000000000875 * t7;
      }
    else if ( y >= 1860.0 && y < 1900.0 )
      {
        t = y - 1860.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        t5 = t2 * t3;
        dT = 7.62 + 0.5737 * t - 0.251754 * t2 + 0.01680668 * t3
                - 0.0004473624 * t4 + t5 / 233174.0;
      }
    else if ( y >= 1900.0 && y < 1920.0 )
      {
        t = y - 1900.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        dT = -2.79 + 1.494119 * t - 0.0598939 * t2 + 0.0061966 * t3 - 0.000197 * t4;
      }
    else if ( y >= 1920.0 && y < 1941.0 )
      {
        t = y - 1920.0;
        t2 = t * t;
        t3 = t2 * t;
        dT = 21.20 + 0.84493 * t - 0.076100 * t2 + 0.0020936 * t3;
      }
    else if ( y >= 1941.0 && y < 1961.0 )
      {
        t = y - 1950.0;
        t2 = t * t;
        t3 = t2 * t;
        dT = 29.07 + 0.407 * t - t2 / 233.0 + t3 / 2547.0;
      }
    else if ( y >= 1961.0 && y < 1986.0 )
      {
        t = y - 1975.0;
        t2 = t * t;
        t3 = t2 * t;
        dT = 45.45 + 1.067 * t - t2 / 260.0 - t3 / 718.0;
      }
    else if ( y >= 1986.0 && y < 2005.0 )
      {
        t = y - 2000.0;
        t2 = t * t;
        t3 = t2 * t;
        t4 = t2 * t2;
        t5 = t2 * t3;
        dT = 63.86 + 0.3345 * t - 0.060374 * t2 + 0.0017275 * t3 +
             0.000651814 * t4 + 0.00002373599 * t5;
      }
    else if ( y >= 2005.0 && y < 2050.0 )
      {
        t = y - 2000.0;
        t2 = t * t;
        dT = 62.92 + 0.32217 * t + 0.005589 * t2;
      }
    else if ( y >= 2050.0 && y < 2150.0 )
      {
        dT = -20.0 + 32.0 * ((y-1820.0)/100.0) * ((y-1820.0)/100.0)
             - 0.5628 * (2150.0 - y);
      }
    else if ( y >= 2150.0 )
      {
        u = ( y - 1820.0 ) / 100.0;
        dT = -20 + 32 * u * u;
      }
    return (dT);
  } // END - get_deltaT

time_t julian_date_to_julian_millenia(double jd) {
    double t;
    t = (jd - 2451545.0); // date is now modified julian with epoch J2000, MJD2000
    t = (t + get_deltaT (jd) / 86400.0); // We add the deltaT, converting it from seconds to fraction of day
    return t / 365250.0;  // This is now in Julian millenia.
}
