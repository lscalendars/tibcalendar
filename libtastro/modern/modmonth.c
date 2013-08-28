#include "modmonth.h"
#include "../jd.h"
#include "sunmon.h"
#include <stdio.h> // for printf (for errors, should be changed)
#include <math.h> // for floor

// we define here the approximate longitude the moon travels in 24 hours
// 360 / 27.321582 =
#define MOON_SPEED 13.176396594

#if 0
/* puts a large angle in the correct range 0 - 360 degrees */
double range_degrees(double d)
{
	d = fmod( d, 360.);
	if(d<0.) d += 360.;
	return d;
}

/* puts a large angle in the correct range 0 - 2PI radians */
double range_radians (double r)
{
	r = fmod( r, 2.*M_PI );
	if (r<0.) r += 2.*M_PI;
	return r;
}
#endif

struct new_moon_details
  {
    int jd;
    int Zsignstart;    // Range: 0-11
    int Zsignend;      // Range: 0-11
    int mnthnumK;      // Range: 1-12 - Kalacakra month number: 1 = nag zla
    int intercal;
    int nxtintercal;   // Flag for repeated month - the next is intercalary
    int delaymth;
// These new, may not be needed in the long term:
    double julf;  // Julian date of true new Moon    
    double meansol; // Mean Sun at true new Moon.    
  } newmon[20];   // Maximum index reached in testing = 17

void get_new_moon ( double * NM_jdat, double * NM_sol, double juldat, double moon_elongation )
  {
    int done = 0;
    double sun_longitude, moon_longitude = 0.0;
    do
      {
        juldat = juldat - ( moon_elongation / MOON_SPEED );
        sun_mon ( juldat, &sun_longitude, &moon_longitude ); // This adds Delta T
        moon_elongation = moon_longitude - sun_longitude;
        if ( moon_elongation > -0.00001 && moon_elongation < 0.00001 )
          done = 1;
        if ( moon_elongation < -330.0 )
          moon_elongation = moon_elongation + 360.0;
        else if ( moon_elongation > 330.0 )
          moon_elongation = moon_elongation - 360.0;
        if ( moon_elongation < -15.0 || moon_elongation > 15.0 )
          {
            printf ("error\n");
            //printf ( "ERROR - Sun: %f, Moon: %f\n", sol, mol );
            //getch ();
          }
      } while ( !done );
    *NM_jdat = juldat;
    *NM_sol = sun_longitude;
  } // END - get_new_moon

unsigned char intercalfollows = 0; // TODO: make it a global option

int get_mod_new_year(int asked_year, float longitude)
{
    unsigned char mnth12_found = 0;
    unsigned char first_new_year_found = 0;
    unsigned char second_new_year_found = 0;
    unsigned char intercal_found = 0;  // FOR TESTING ONLY

// Kalacakra New Year cannot start before about 22nd Feb. Start searching from 1st Jan:

    double last_moon_elong = 0.0;
    double moon_elong = 0.0;
    unsigned int hit = 0;
    unsigned char finish = 0;
    unsigned char just_done_newmoon = 0;
    double jul_factor = 5.0/24.0 - longitude / 360.0;  // for 5am LMST at site
    double juldatf = 0.0;
    double sun_longitude = 0.0;
    double moon_longitude = 0.0;
    double sol_hold = 0.0;
    double NM_juldat = 0.0;
    double NM_sol = 0.0;
    double NM_meansol = 0.0;
    int first_Kmnth_flag = 0;
    int last_mnth_flag = 0;

    juldatf = (double) (wd_to_jd (1, 1, asked_year)) - 0.5 + jul_factor;
    // wd_to_jd - 0.5 is the beginning of UT day
    // jul_factor adds the timezone factor
    // juldatf is now equivalent to 5am LMST on our target meridian

// First, go through year finding new moons and setting month definitions.
    while ( finish == 0 )
      {
        sun_mon ( juldatf, &sun_longitude, &moon_longitude); // This adds Delta T
        moon_elong = moon_longitude - sun_longitude;
        sol_hold = sun_longitude;  // Solar longitude at 5am LMST.
        if ( moon_elong < 0.0 )
          moon_elong = moon_elong + 360.0;
        if ( moon_elong < last_moon_elong ) // We have gone past a New Moon
          {
            newmon[hit].jd = (int) (juldatf - jul_factor + 0.5);  // First day after new Moon.

// We need to find exact time of New Moon:
            get_new_moon ( &NM_juldat, &NM_sol, juldatf, moon_elong );
            just_done_newmoon = 1;

// We store an index number for the solar zodiac sign at new Moon.

//-----------------------------------------------------------------------

// For mean Sun at mean new Moon: 

// Use these four lines if month definition is mean Sun at mean new Moon:
// Should we calculate exact time of mean new moon - perhaps useful??
// fptgt3 = fopen ( "tst_msmm.txt", "a" ); // Mean Sun, true new Moon
            NM_meansol = getmeanSunmeannewMoon ( NM_juldat ); // This adds Delta T
            newmon[hit].julf = NM_juldat;
            newmon[hit].meansol = NM_meansol;
// This is the line actually to use:            
            newmon[hit].Zsignstart = (int) floor ( NM_meansol / 30.0 );

//-----------------------------------------------------------------------

// Use this line if month definition is true Sun at true new Moon:
//          newmon[hit].Zsignstart = (int) floor ( NM_sol / 30.0 );

//-----------------------------------------------------------------------

// Use these two lines if month definition is mean Sun at true new Moon:
//            NM_meansol = getmeanSun ( NM_juldat ); // This adds Delta T
//            newmon[hit].Zsignstart = (int) floor ( NM_meansol / 30.0 );

// Show the user we are actually doing something:
            //printf ( "Jul: %f, Mean Sun = %f, True Sun = %f\n",
            //          NM_juldat, NM_meansol, NM_sol );

// Use this line if month definition is daybreak, true Sun:
//          newmon[hit].Zsignstart = (int) floor ( sol_hold / 30.0 );

            if ( hit > 0 ) // Not our first new Moon
              {
                newmon[hit-1].Zsignend = newmon[hit].Zsignstart;
// Do we have a month in which the Sun does not change sign?
                if ( newmon[hit-1].Zsignstart == newmon[hit-1].Zsignend )
                  {
                    // Does intercalary take the month before or delay?
                    if ( intercalfollows == 0 ) // Tibetan style
                      newmon[hit-1].mnthnumK = newmon[hit-1].Zsignend + 2;
                    else  // Kalacakra style
                      newmon[hit-1].mnthnumK = newmon[hit-1].Zsignend + 1; //CHECK !!!????
                    if ( newmon[hit-1].mnthnumK > 12 )
                      newmon[hit-1].mnthnumK -= 12;
                    newmon[hit-1].intercal = 1;
                    newmon[hit].delaymth = 1;                    
                    if ( hit > 1 ) // Flag for repeated month - the next is intercalary
                      newmon[hit-2].nxtintercal = 1;
                    printf ( "Intercalary month: %d\n", newmon[hit-1].mnthnumK );
                    if ( hit < 3 && newmon[hit-1].mnthnumK == 12 ) // FOR
                                                               // TESTING
                      mnth12_found = 1;
                    if ( intercal_found )  // TESTING ONLY
                      printf ( "TWO INTERCALARY MONTHS!!!\n" );
                    intercal_found = 1;
                  }
                else  // Normal month
                  {
                    newmon[hit-1].mnthnumK = newmon[hit-1].Zsignend + 1;
                    if ( newmon[hit-1].mnthnumK > 12 )
                      newmon[hit-1].mnthnumK -= 12;
                    newmon[hit-1].intercal = 0;

// Following two "if"s are for checking for possible problems:
                    if ( hit < 3 && newmon[hit-1].mnthnumK == 12 )
                      mnth12_found = 1;
                    if ( newmon[hit-1].Zsignend - newmon[hit-1].Zsignstart == 2
                                                                           )
                      {
                        printf ( "DOUBLE SIGN ENTRY: %d\n",
                                 newmon[hit-1].Zsignstart + 1 );
                        //getch ();
                      }
                  }
              }
            if ( first_new_year_found == 0 )
              {
                if ( newmon[hit-1].mnthnumK == 1 )
                  {
                    first_Kmnth_flag = hit - 1;
                    first_new_year_found = 1;
                  }
// This to fix the missing of intercalary first months: 11/3/2008
// Point is: if we have already found the first month, do not mark a
// delayed month 1 as the first. Is there a similar problem with line above???
//                else if ( newmon[hit-1].mnthnumK == 12 && hit > 1
//                          && newmon[hit-2].mnthnumK != 12 )
//                  {
//                    first_Cmnth_flag = hit - 1;
//                  first_new_year_found = 1;
//                  }
              }
            if ( second_new_year_found == 0 && newmon[hit-1].mnthnumK == 1 )
              {
                last_mnth_flag = hit - 2;
                second_new_year_found = 1;
                finish = 1;
              }
            ++hit;
          }
// Do another day:
        last_moon_elong = moon_elong; // warning: we must not take here the value
        // modified by get_new_moon, it might be slightly negative and will confuse
        // the rest of the algorithm, which works well if we take the value we
        // had before get_new_moon

        if ( just_done_newmoon == 1 )
          {
            juldatf = juldatf + 26.0;
            just_done_newmoon = 0;
          }
        else
          juldatf = juldatf + 1.0;
      } // End of loop searching for new Moons

    if ( !mnth12_found )  // Keep this as an error check.
      {
        printf ( "Twelfth month not found\n" );
        //getch ();
      }
      return newmon[first_Kmnth_flag].jd;
}
