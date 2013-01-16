/*********************************************************************************
License for Tibastro - Tibetan astronomical and astrological calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

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

#include <stdio.h> // for printf, currently used for errors, but should be changed...
#include "tbday.h"
#include "tbmonth.h"
#include "utils.h"
#include "jd.h"			// for jd_to_wd
#include "system.h" // for epoch
#include "tbstructures.h"
#include "astrology.h" // when we want a next day or month, we need to update the astrology linked to it if any

/* Function returning the tib_day of a given tibetan date:
 * inputs:
 *    - year, month and day are the tibetan date for which we want the tib_day
 *    - asys is quite obvious
 *    - m_f_o_s (month first or second) is FIRST if, in the case of a doubled month, we take the first, or SECOND if we take the second
 *    - d_f_o_s (day first or second) is same for the day
 *
 * Warning: this function can return an ommited day, in this case, you can call get_next_day
 */
tib_day*
get_tib_day_from_tib_date (int y, int m, int d, astro_system *asys, unsigned char m_f_o_s, unsigned char d_f_o_s)
 {
  tib_day *td;
  tib_month *month;
 long int tt, ty, tm; // not necessary but make the code much clearer
 unsigned char zeromthfg; // just to record the zeromonthflag
 long int adj_mth;
 long int prv_gd, nxt_gd, prv2_gd;
 long int zd0rec; // used to cancel the potential effects on zd[0] of adj_zla
  long int nyibar[6] = { 0, 0, 0, 0, 0, 0 };	// mean solar longitude
  long int nyidag[6] = { 0, 0, 0, 0, 0, 0 };	// true solar longitude
  long int gzadag[6] = { 0, 0, 0, 0, 0, 0 };	// true weekday
  
td = new_tib_day();
month = td->month; // TODO: remove?
// this is always true
month->year->year = (long int) y;
month->month = (long int) m;
td->tt = d;
ty = (long int) y;
tm = (long int) m;
tt = (long int) d;
 /*    Finding the month   */
 // the idea here is to get the data for 1 month of the year, directly in Tibetan date, not passing
 // through julian date
// so first we get the true month for this month:
zla_dag (asys->epoch, ty, tm, month->true_month);
zd0rec = month->true_month[0]; // we remember the true month main part
zeromthfg = month->zero_month_flag;
// we adjust the month:
adj_mth = adj_zla (tm, month->true_month, &(month->zero_month_flag), asys);
// now zd is set, we check if there are some subltelties with double month
// if adj_mth is the month before the one we are looking for, we take the following one
if ( adj_mth == tm -1 || (tm ==1L && adj_mth == 12L) )
      {
          if (!zeromthfg)
          {
          tm = tm + 1;
          next_zla_dag(month->true_month);
          }
          adj_mth = adj_zla (tm, month->true_month, &(month->zero_month_flag), asys);
      }
month->asked_month = tm; // TODO: test...
    // if we are at the first month of a double month, we cancel the changes brought by adj_mth because we are not yet in a loop
    if ( adj_mth == -month->month && m_f_o_s == FIRST) // no else before the if, who knows...
      {
        month->true_month[0] = zd0rec;
        month->zero_month_flag = zeromthfg;
        month->type = FIRST_OF_DOUBLE;
      }
      else if ( adj_mth == -month->month && m_f_o_s == SECOND)
       {
       month->type = SECOND_OF_DOUBLE; // TODO: test if something else is needed
       }
      // now we can fill a few more fields
	   get_month_data (asys, month->true_month[0], month->rilcha, month->nyidru, month->gzadru);
     // Test the day loop for Phugpa 1/1/1977: first day is ommited
    // now we look for the day
     td->gd = get_tt_data (asys->epoch, month->true_month[0], month->gzadru, month->nyidru, month->rilcha, tt, td->nyidag, td->gzadag, td->nyibar);
     // we have our day
  // td->ommited and td->duplicated are the only variables that are not set, we
  // must give them a default value
  td->ommited = NORMAL;
  td->duplicated = NORMAL;
// We now have data for the current lunar day and both the one before and following.    
// we can set the type of day
// we can compute for the previous lunar day even if tt = 1 (cf. KTC23 ?)
  prv_gd = get_tt_data (asys->epoch, month->true_month[0], month->gzadru, month->nyidru, month->rilcha, tt-1L, nyidag, gzadag, nyibar);
  // we compute nxt_gd if the next lunar day is in the same month
  if (tt < 30L)
  nxt_gd = get_tt_data (asys->epoch, month->true_month[0], month->gzadru, month->nyidru, month->rilcha, tt+1L, nyidag, gzadag, nyibar);
  else
  nxt_gd = 0L;
  // we compute prv2_gd if tt>1, otherwise it would change the month
  if (tt >1L)
    prv2_gd = get_tt_data (asys->epoch, month->true_month[0], month->gzadru, month->nyidru, month->rilcha, tt-2L, nyidag, gzadag, nyibar);
  else
    prv2_gd = 0L;
  if (prv_gd == td->gd) // means that the lunar date is ommited, we go to the next one
    {
      td->ommited=OMMITED; // TODO: in this case, what is td->gd?
    }
  else if (td->gd - prv_gd == 2) // means that the lunar date is duplicated
   {
   if (d_f_o_s == FIRST)
   {
   // in case we take the first one:
   td->duplicated=FIRST_OF_DUPLICATED;
   //TODO: useful? should be moved to the printing function?
   td->gzadag[0] = (td->gzadag[0]+6)%7;
   td->gzadag[1] = 60L; td->gzadag[2] = 0L; td->gzadag[3] = 0L; td->gzadag[4] = 0L; td->gzadag[5] = 0L;
   td->gd = td->gd - 1; //TODO: ?
   } else { //d_f_o_s == SECOND
   td->duplicated=SECOND_OF_DUPLICATED;
   } 
   }
  if (nxt_gd && nxt_gd == td->gd)		// Few tested, all working properly
    {
      td->ommited = NEXT_OMMITED;
    }
  if (prv2_gd && prv2_gd == prv_gd)	// prv2_gd is set to 0 if we didn't check for previously ommited, thus it will be different from gd... 
    {
      td->ommited = PREVIOUS_OMMITED;
    }
    return td;
 }

/* an important function to transform a tibetan day into the next lunar date (or lunar day if lunar day is ommited)
 *  TODO: maybe we should add an option to pass ommited days and factorize a lot of code...
 * function supposes that if td->astro_data exists, then it is correct
 */
void
tib_day_next (tib_day *td, astro_system *asys)
{
  long int prv_gd, nxt_gd=0L;
    long int nyibar[6] = { 0, 0, 0, 0, 0, 0 };	// mean solar longitude
  long int nyidag[6] = { 0, 0, 0, 0, 0, 0 };	// true solar longitude
  long int gzadag[6] = { 0, 0, 0, 0, 0, 0 };	// true weekday
  if (td->duplicated == FIRST_OF_DUPLICATED)
      {
       // if we want day after a first of duplicated, almost nothing change:
       td->duplicated = SECOND_OF_DUPLICATED;
       // we have to recompute gzadag
       // TODO: this could be optimized a lot!
       td->gd = get_tt_data (asys->epoch, td->month->true_month[0], td->month->gzadru, td->month->nyidru, td->month->rilcha, td->tt, td->nyidag, td->gzadag, td->nyibar);
       if (td->astro_data)
          get_day_astro_data(td, asys, 1);
       //if (td->planet_data)
        //  get_day_planet_data(td, asys); TODO: uncomment when it's good
       return;
       }
       td->duplicated = NORMAL; // if we were on a second of duplicated, we just cancel td->duplicated
  switch(td->ommited)
  {
    case NEXT_OMMITED: // this can never happen on 30th
       td->ommited = OMMITED;
       td->tt = td->tt+1;
       if (td->astro_data)
          get_day_astro_data(td, asys, 1);
       return;
       break;
    case OMMITED:
       td->ommited = PREVIOUS_OMMITED;
       break;
    default:
      td->ommited = NORMAL;
      break;
  }
  // now ommited and duplicated are set for what concerns the day before
  // and we compute things thinking we are already on the next day
  // if we are the second of a duplicated day, the tt doesn't change, else yes
       td->tt = td->tt+1;
       if (td->tt > 30)
         {
           tib_month_next(td->month, asys);
           td->tt = 1;
         }

          prv_gd = td->gd; // we set prv_gd in all cases (think about a duplicated 1st lunar date
  
  td->gd = get_tt_data (asys->epoch, td->month->true_month[0], td->month->gzadru, td->month->nyidru, td->month->rilcha, td->tt, td->nyidag, td->gzadag, td->nyibar);
    if(prv_gd == td->gd)
     td->ommited = OMMITED;
  if (td->tt < 30)
    nxt_gd = get_tt_data (asys->epoch, td->month->true_month[0], td->month->gzadru, td->month->nyidru, td->month->rilcha, td->tt + 1, nyidag, gzadag, nyibar);
  if (nxt_gd && nxt_gd == td->gd)		// Few tested, all working properly
      td->ommited = NEXT_OMMITED;
  if (td->gd - prv_gd == 2) // means that the lunar date is duplicated
   {
   // in case we take the first one:
   td->duplicated=FIRST_OF_DUPLICATED;
   //TODO: useful? should be moved to the printing function?
   td->gzadag[0] = (td->gzadag[0]+6)%7;
   td->gzadag[1] = 60L; td->gzadag[2] = 0L; td->gzadag[3] = 0L; td->gzadag[4] = 0L; td->gzadag[5] = 0L;
   td->gd = td->gd - 1;
  }
  // here we consider that astro_data was correctly filled...
  if (td->astro_data)
    get_day_astro_data(td, asys, 1);
}



/* Function looping through a month and finding the tt and its caracteristics
 * td is the tibetan day structure that will be filled entirely, but its month
 * element must be already filled with the good month and year (it actually uses
 * only the true_month[0] part).
 */
 // TODO: modify it to use tib_day_next
void
find_day (tib_day *td, long int jd, epoch *epch)
{
  // month values
  long int rilcha[2];	// anomaly
  long int gzadru[6];	// mean weekday (for the month)
  long int nyidru[6];	// mean solar longitude (for the month)
  // mean values for a tt in the month
  long int nyibar[6] = { 0, 0, 0, 0, 0, 0 };	// mean solar longitude (for a precise tt)
  // true values for a tt in the month
  long int nyidag[6] = { 0, 0, 0, 0, 0, 0 };	// true solar longitude (for a tt)
  long int gzadag[6] = { 0, 0, 0, 0, 0, 0 };	// true weekday (for a tt)

  long int cur_mth = td->month->true_month[0];
  long int prv_gd = 0;
  long int nxt_gd = 0;
  long int prv2_gd = 0;

  long int tt = jd - td->month->start_gd - 2;	// current lunar day
  if (tt <= 0)
    tt = 1;

  // first we copy the values for the month in the values we're using
  // inside this function
  rilcha[0] = td->month->rilcha[0]; rilcha[1] = td->month->rilcha[1];
  copy_lst(gzadru, td->month->gzadru);
  copy_lst(nyidru, td->month->nyidru);
  td->gd =
    get_tt_data (epch, cur_mth, gzadru, nyidru, rilcha, tt, td->nyidag, td->gzadag,
		 td->nyibar);
  // first loop in order to get gd around jd
  // if gd == jd, it should be a normal day, and if gd == jd + 1, it should be a duplicated day
  while (td->gd != jd && td->gd != jd + 1)
    {
      tt = tt + 1;
      if (tt > 30)		// Big problem!!!
	{
	  printf
	    ("strange error (1): please report to the tibastro developpers.");
	  return;
	}
      td->gd =
	get_tt_data (epch, cur_mth, gzadru, nyidru, rilcha, tt, td->nyidag,
		     td->gzadag, td->nyibar);
    }
  // we found our day! and td-> gd, td->nyidag, td->gzadag and td->nyibar are already filled
  td->tt = tt;
  // getting previous, second previous and next day general day to know if 
  // there are ommited or duplicated days around...
  prv_gd =
    get_tt_data (epch, cur_mth, gzadru, nyidru, rilcha, tt - 1, nyidag,
		 gzadag, nyibar);
  // First, a check. Is it possible that current lunar day and previous both
  // calculate to same solar day? This would be if current lunar day is omitted
  // and entirely contained within the solar day. Maybe if lunar day 1 is omitted?
  // This has not been found in testing, and should not happen. TODO: test with 1/1 1977
  // The way we should find an omitted is when the following lunar day and
  // current calculate to same solar day.
  if (td->gzadag[0] == gzadag[0])	// This is now error code
    {
      printf
	("strange error (2): please report to the tibastro developpers.");
      return;
    }
  // TODO: does it work with tt=1?
  // Check for possible omitted lunar date, prior to current
  if (gzadag[1] >= 54)		// 54?? This should get all of them, in Phugpa TODO: understand this...
    {
      prv2_gd =
	get_tt_data (epch, cur_mth, gzadru, nyidru, rilcha, tt - 2, nyidag,
		     gzadag, nyibar);
    }
// Now get data for next lunar day
  nxt_gd =
    get_tt_data (epch, cur_mth, gzadru, nyidru, rilcha, tt + 1, nyidag,
		 gzadag, nyibar);
  // td->ommited and td->duplicated are the only variables that are not set, we
  // must give them a default value
  td->ommited = NORMAL;
  td->duplicated = NORMAL;
// We now have data for the current lunar day and both the one before and following.    
// we can set the type of day
// TODO: what about the case prv_gd == td-> gd?
  if (nxt_gd == td->gd)		// Few tested, all working properly
    {
      td->ommited = NEXT_OMMITED;
    }
  if (prv2_gd == prv_gd)	// prv2_gd is set to 0 if we didn't check for previously ommited, thus it will be different from gd...
    {
      td->ommited = PREVIOUS_OMMITED;
    }
  if (td->gd - prv_gd == 2)
    {
      // tt is duplicated
      if (td->gd != jd)
	{
	  td->duplicated = FIRST_OF_DUPLICATED; 
	}
      else
	{
	  td->duplicated = SECOND_OF_DUPLICATED;
	}
    }
}

/* Main routine to return Tibetan day for a solar day
 * jd is Julian day number
 * td is the structure for returning the data  
 * tibetan years are numbered like western calendar years
 */
void
get_td_from_jd (long int jd, tib_day *td, astro_system *sys)
{
  if (jd <= sys->epoch->spz_j)
    {
      printf("error: day asked is before the epoch...\n");
      return;
    }
  // first we find the month and year
  find_month_and_year (jd, sys, td->month);
  // Then we find the day inside the month
  find_day (td, jd, sys->epoch);
}


unsigned int gza_short_flag = 0;	// TODO: let the user choose if he wants it or not

/* Function to calculate true solar longitude and weekday, "nyi dag" and "gza' dag".
 *  - nyibar[6] is the corrected month mean solar longitude (mean solar longitude for a certain lunar day)
 *  - tsebar[6] is the corrected month mean weekday (mean weekday for a certain lunar day)
 *  - rilcha[2] is the anomaly
 *  - tt is the tithi for which we make the computation
 *  - nyidag[6] is the true solar longitude (result)
 *  - gza_dagp[6] is the true weekday (result)
 * See KTC p. 24 and following for details
 */
void
nyi_dag_and_gza_dag (long int nyibar[6], long int tsebar[6],
		       long int rilcha[2], long int tt, long int nyidag[6],
		       long int gzadag[6])
{
  long int test, tquo, trem, tot, rilpo;
  long int lista[6] = { 0, 0, 0, 0, 0, 0 };	// four temporary values
  long int listb[6] = { 0, 0, 0, 0, 0, 0 };
  long int listc[6] = { 0, 0, 0, 0, 0, 0 };
  long int listd[6] = { 0, 0, 0, 0, 0, 0 };
  long int nyiwor[6] = { 0, 0, 0, 0, 0, 0 };
  long int gzawor[5] = { 0, 0, 0, 0, 0 };
  unsigned char nyidor = 0;	// an indicator

  long long int z;		// TODO: could be removed

  // table on KTC p. 24
  static const long int gzabye[14] =
    { 5, 5, 4, 3, 2, 1, -1, -2, -3, -4, -5, -5, -5, 5 };
  static const long int gzadom[14] =
    { 5, 10, 15, 19, 22, 24, 25, 24, 22, 19, 15, 10, 5, 0 };
  // table on KTC p. 33
  static const long int nyibye[6] = { 4, 1, 1, 4, 6, 6 };
  static const long int nyidom[6] = { 6, 10, 11, 10, 6, 0 };

  // constants for the correction:
  static long int sl_first_corr[6] = { 6, 45, 0, 0, 0, 0 };
  static long int sl_second_corr[6] = { 13, 30, 0, 0, 0, 0 };

  // first we get the true solar longitude
  sub_lst (nyiwor, nyibar, sl_first_corr, 27, 67);	// KTC 31
  test = 60 * nyiwor[0] + nyiwor[1];
  if (test < 810)
    nyidor = 0;
  else
    {
      nyidor = 1;
      sub_lst (nyiwor, nyiwor, sl_second_corr, 27, 67);
      test = 60 * nyiwor[0] + nyiwor[1];
    }
  trem = test % 135;
  tquo = test / 135;
  if (tquo == 0)
    tquo = 6;

  lista[2] = (trem * 60 + nyiwor[2]) * nyibye[(int) tquo - 1];
  lista[3] = nyiwor[3] * nyibye[(int) tquo - 1];
  lista[4] = nyiwor[4] * nyibye[(int) tquo - 1];

  div_lst_6 (lista, lista, 135, 67, 1);
  add_lst (lista, zerolst, lista, 27, 67);

  // listc is the intermediate correction, KTC p.34
  listb[1] = nyidom[(int) tquo - 1];
  if (tquo == 3 || tquo == 4 || tquo == 5)	// Then, subtract:
    sub_lst (listc, listb, lista, 27, 67);
  else
    add_lst (listc, listb, lista, 27, 67);

  if (nyidor == 0)
    sub_lst (nyidag, nyibar, listc, 27, 67);
  else
    add_lst (nyidag, nyibar, listc, 27, 67);

  // then the true weekday (we keep listc, it will be useful)

  clear_lst (lista);
  clear_lst (listb);

  // this part of the calculation is described on KTC p. 25
  rilpo = rilcha[0] + tt;
  trem = rilpo % 14;
  if (trem == 0)
    trem = 14;
  listb[1] = gzadom[(int) trem - 1];

  if (gza_short_flag)		// Flag for less accurate weekday adjustment
    {
      tot = (rilcha[1] * gzabye[(int) trem - 1L] * 60 * 6 * 707) / 126;
      // Maximum value = 125 * 5 * 254520 = 159075000 - OK
      // rilcha[1] * gzabye[(int)trem - 1L] * 60 * 6 * 707 is called chasa
    }
  else
    {
      z = (long long) (30 * rilcha[1] + tt) * gzabye[(int) trem - 1];
      tot = (z * 707 * 360) / 3780;
    }
  if (tot < 0)			// KTC 26
    {
      lista[4] = -tot;
      add_lst (lista, lista, zerolst, 7, 707);
      sub_lst (listd, listb, lista, 7, 707);
    }
  else
    {
      lista[4] = tot;
      add_lst (lista, lista, zerolst, 7, 707);
      add_lst (listd, listb, lista, 7, 707);
    }
  if ((rilpo / 14) % 2 == 0)
    add_lst (gzawor, tsebar, listd, 7, 707);
  else
    sub_lst (gzawor, tsebar, listd, 7, 707);

  gzawor[4] = (long) (((long long) gzawor[4] * 67) / 707);
//  The above is: gzawor[4] = ( 67 * gzawor[4] ) / 707;

  if (nyidor == 0)
    sub_lst (gzadag, gzawor, listc, 7, 67);
  else
    add_lst (gzadag, gzawor, listc, 7, 67);

// Convert back the lowest fractional part:
  gzadag[4] = (long) (((long long) gzadag[4] * 707) / 67);
}

/*
 * Function to get all the data for a tt:
 * inputs:
 *  - gzadru
 *  - nyidru
 *  - rilcha
 *  - tt
 * output:
 *  - nyidag
 *  - gzadag
 *  - nyibar
 * returns:
 *  - spi zag
 */
int
get_tt_data (epoch *epch, long int cur_mth, long int gzadru[6],
	     long int nyidru[6], long int rilcha[2], long int tt,
	     long int nyidag[6], long int gzadag[6], long int nyibar[6])
{
  long int tsedru[6] = { 0, 0, 0, 0, 0, 0 };	// lunar mean weekday (for a tt in any month)
  long int nyilon[6] = { 0, 0, 0, 0, 0, 0 };	// solar longitude (for a tt in any month)
  long int tsebar[6] = { 0, 0, 0, 0, 0, 0 };	// mean weekday (for a precise tt)
  // constant to calculate lunar day mean weekday, "tshes kyi dhru ba", see KTC p. 23
  static long int tse_drup_const[6] = { 0, 59, 3, 4, 16, 0 };
  // constant to calculate lunar day solar longitude, "nyi ma'i longs spyod", see KTC p. 23
  static long int nyi_long_const[6] = { 0, 4, 21, 5, 43, 0 };

  // small optimization: if tt=0, less calculations are needed as a lot of results will be 0
  if (tt != 0)
    {
      // first computing tsedru
      mul_lst (tsedru, tse_drup_const, tt, 7, 707);
      // then nyilon
      mul_lst (nyilon, nyi_long_const, tt, 27, 67);
      add_lst (tsebar, gzadru, tsedru, 7, 707);
      add_lst (nyibar, nyidru, nyilon, 27, 67);
      nyi_dag_and_gza_dag (nyibar, tsebar, rilcha, tt, nyidag, gzadag);
    }
  else
    {
      nyi_dag_and_gza_dag (nyidru, gzadru, rilcha, 0, nyidag, gzadag);
    }
  return spi_zag (epch, cur_mth, tt, gzadag[0]);
}

/* Function computing the general day (spyi zhag)
 * Calculates Julian for the integer Julian day count at noon UT
 * on weekday of "gza' dag".
 * input:
 *  - cur_mth: the true month (zd[0]) as defined above
 *  - tt: the lunar day
 *  - epch: the epoch
 *  - sd: the integer part of the true weekday (gzadag[0])
 * output:
 *  - the general day
 * See KTC 46 for details.
 */
long int
spi_zag (epoch *epch, long int cur_mth, long int tt, long int sd)
{
  long int spizag;		// the result
  long int b, c;		// intermediate values
  spizag = cur_mth * 30 + tt;
  c = spizag + epch->spz_c;
  b = spizag + epch->spz_b;
  b = b + c / 707;
  spizag = spizag - b / 64;
  c = (spizag + epch->spz_f) % 7;
  b = sd;
  if (c != b)
    {
      if (c > 4 && b < 2)
	b += 7;
      else if (b > 4 && c < 2)
	c += 7L;
      spizag = spizag + b - c;
    }
  //if ( b - c > 2 )
  //  printf ( "\nerror in general day routine: %ld\n", b - c );
  return spizag + epch->spz_j;
}
