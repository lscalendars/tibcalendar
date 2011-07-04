/* TC_PCALC.C */

/*********************************************************************************
License for TCG - Tibetan Calendar software for "grub rtsis"

Copyright (c) 2009-2011 Edward Henning

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

#include <stdlib.h>
#include <stdio.h> // for printf, currently used for errors, but should be changed...
#include <string.h>		// for memset
#include "tbday.h"
#include "tbplanets.h"
#include "utils.h"
#include "jd.h"			// for jd_to_wd

tib_day *
new_tib_day()
{
  tib_day *td = malloc (sizeof (tib_day));
  memset (td, 0, sizeof (tib_day));
  tib_month *tm = malloc (sizeof (tib_month));
  memset (tm, 0, sizeof (tib_month));
  td->month = tm;
  return td;
}

void
free_tib_day(tib_day *td)
{
  free(td->month);
  free(td);
}

/* Function to calculate true month, "zla ba rnam par dag pa" from a tibetan date
 * inputs: 
 *  - epch is the epoch
 *  - y is the tibetan year
 *  - m is the tibetan month
 * output is a table of two int representing the true month: 
 *  - first is the integer part
 *  - second is the remainder mod. 65
 *  - returned in the argument zla_dagp
 * reference : KTC p. 15
 * operation is first getting M, then M*1;2 + 0;55 (65)
 */
void
phugpa_zla_dag (const epoch epch, long int y, long int m, long int zd[2])
{
  long int yr, a, b;
  yr = y - epch.year;
  a = 12 * yr + m - 3;		// intermediate value (M)
  b = 2 * a + epch.eyr_a;
  zd[1] = b % 65;
  zd[0] = a + b / 65;
}

/* In the following functions, some variable represent always the same things:
 *  - ty: the almost tibetan year (numbered as western years), with the
 *    particularity that all years are considered to have 12 month, and thus
 *    can be different from the true tibetan year for a few month if preceding
 *    year has 13 lunar months. This is useful for calculations but will
 *    need adjustments before being printed to the user
 *  - tm: the count of month from the beginning of ty (first month: tm=0).
 *    tm and ty are intermediate values.
 *  - adj_mth: the adjusted month as appearing, with the following convention:
 *      - if tm is the first month of a doubled month, adj_mth = -tm
 *      - else adj_mth is the month appearing in the calendar (can be different
 *        from tm because of doubled month)
 *  - zd[2]: the true month (zd[0]), with the remainder mod 65, called
 *    intercalation index in KTC (zd[1])
 *    In the case of zd[1] index becoming 0 or 1, we modify zd[0] for
 *    the first month with zd[1] of 0 or 1: zd[0] becomes zd[0]-1, but
 *    gets back to normal for the second month with zd[1] 0 or 1.
 *  - zeromthflg: zero month flag, needed for when the intercalation index 
 *    becomes 0 or 1:
 *      - if zeromthflg is 0, we haven't taken the 0th month into account yet
 *      - if zeromthflg is 1, it's the second time we take it into account
 */

/* Function to adjust month number, if needed
 * inputs:
 *  - epch: the epoch, especially the two remainders for the intercalary month (cf. KTC p. 51)
 *  - zd, tm, zeromthfg: explained above
 * returns adj_mth, as explained above, and also modifies zd[0] and zeromthfg
 * if necessary.
 */

long int
phugpa_adj_zla (long int tm, long int zd[2], const epoch epch,
		unsigned char *zeromthfg)
{
  long int adj_mth;
  if (zd[1] == epch.zlasho || zd[1] == epch.zlasho + 1)
    {
      // for month taking the name of the following month, the convention is to
      // return -tm
      //adj_mth = -tm;
      return -tm;
    }
  else
    {
      if (zd[1] > epch.zlasho + 1)
	{
	  adj_mth = tm - 1;
	}
      else
	{
	  // here we modify zd[0] for the first month with a zd[1] of 0 or 1
	  if (zd[1] == 0 || zd[1] == 1)
	    {
	      if (!*zeromthfg)
		{
		  *zeromthfg = 1;
		  adj_mth = tm - 1;
		  zd[0] = zd[0] - 1;
		}
	      else
		{
		  *zeromthfg = 0;
		  adj_mth = tm;
		  zd[0] = zd[0] + 1;
		}
	    }
	  else			// Arrive here if 1 < zladag[1] < zlasho
	    {
	      adj_mth = tm;
	    }
	}
    }
	  if (adj_mth == 0)
	    {
	      adj_mth = 12;
	    }
  return adj_mth;
}


/*
 * Function taking the julian day and returning the true year and month. To
 * do so it computes the julian day for the new moon of the consecutive month
 * until we find a new moon that is after the julian day we want, and we go back
 * one month earlier.
 * 
 * The first approximation for the month and year is 3 month before the western
 * date. It is not traditional, but there is no traditional way of looking for
 * daily data for a julian day... It doesn't change anything for the computation
 * though.
 * 
 * inputs:
 *  - jd: the julian day for which we want the date
 *  - epch: the epoch
 * outputs:
 *  - it fills the month fields with everything needed
 */
void
find_month_and_year (long int jd, epoch epch, tib_month *month)
{
  long int tm, ty, adj_mth, gd; 
  int wd, wm, wy, wdow; 
  // a flag to take the zeroth month into account twice
  unsigned char zeromthfg = 0;
  unsigned char done = 0;
  long int zd[2] = { 0, 0 };	// anomaly
  long int nyibar[6] = { 0, 0, 0, 0, 0, 0 };	// mean solar longitude
  long int nyidag[6] = { 0, 0, 0, 0, 0, 0 };	// true solar longitude
  long int gzadag[6] = { 0, 0, 0, 0, 0, 0 };	// true weekday
  
  jd_to_wd (jd, &wd, &wm, &wy, &wdow);
  tm = (long int) wm - 3;
  ty = (long int) wy;
  if (tm < 1)
    {
      ty = ty - 1;
      tm = tm + 12;
    }
  
  while (!done)
    {
      if (!zeromthfg)		// We need to use the same data, twice.
	{
	  phugpa_zla_dag (epch, ty, tm, zd);
	}
      adj_mth = phugpa_adj_zla (tm, zd, epch, &zeromthfg);	// do not forget this can modify zd[0]
      // here the lunar day is 0 thus we don't need the corrections that
      // apply to other lunar days (KTC 23).
      get_month_data (epch, zd[0], month->rilcha, month->nyidru, month->gzadru);
      gd = get_tt_data (epch, zd[0], month->gzadru, month->nyidru, month->rilcha, 0, nyidag, gzadag, nyibar);

      // Now, have we gone far enough? We first find the true new Moon date 
      // immediately after or equal to our target date (only for coming back
      // to the month before afterwards)
      if (gd >= jd)
	done = 1;
      else
	{
	  month->year = ty;
    month->month = tm;
	  month->adjusted_month = adj_mth;
	  month->true_month[0] = zd[0];
	  month->true_month[1] = zd[1];
	  month->start_gd = gd;
	  if (!zeromthfg)
	    tm = tm + 1;
	  if (tm > 12)
	    {
	      tm = tm - 12;
	      ty = ty + 1;
	    }
	}
    }
  // month->year,month, etc. contains data about the month before
  // adjusting the year so that it is really the tibetan year
  if (month->adjusted_month == 12 && month->month == 1)
    month->year = month->year - 1;
  // we need to get the rilcha, nyidru and gzadru for the month
  get_month_data (epch, month->true_month[0], month->rilcha, month->nyidru, month->gzadru);
  get_year_astro(month);
}

/* Function looping through a month and finding the tt and its caracteristics
 * td is the tibetan day structure that will be filled entirely, but its month
 * element must be already filled with the good month and year (it actually uses
 * only the true_month[0] part).
 */
void
find_day (tib_day *td, long int jd, epoch epch)
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
  // This has not been found in testing, and should not happen
  // The way we should find an omitted is when the following lunar day and
  // current calculate to same solar day.
  if (td->gzadag[0] == gzadag[0])	// This is now error code
    {
      printf
	("strange error (2): please report to the tibastro developpers.");
      return;
    }
  // Check for possible omitted lunar date, prior to current
  if (gzadag[1] >= 54)		// 54?? This should get all of them, in Phugpa
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

/* Main routine to return Tibetan data for a solar day
 * jd is Julian day number, tdat is the structure for returning the data  
 * tibetan years are numbered like western calendar years
 * argument to add: epoch is not used yet but will contain the data of the epoch
 */
void
get_day_data (long int jd, tib_day *td)
{
  epoch epch = phugpa_epch;

  if (jd <= epch.spz_j)
    {
      printf("error: day asked is before the epoch...\n");
      return;
    }
  // first we find the month and year
  find_month_and_year (jd, epch, td->month);
  // Then we find the day inside the month
  find_day (td, jd, epch);
  // now we have our day... let's fill the planetary data
  get_planets_data(td, epch);
}

/* Function to calculate month mean Sun, "nyi ma'i dhru ba" at new moon
 *  - zd0 is the same as in rilchafp
 *  - nyidru[] is the result
 * See KTC p. 21 for details
 */
void
get_month_data (epoch epch, long int zd0, long int rilcha[2], long int nyidru[6], long int gzadru[6])
{
  long int a, b;
  static long int nyi_drup_const[5] = { 2, 10, 58, 1, 17 };
  static long int gza_drup_const[5] = { 1, 31, 50, 0, 480 } ;
  // first we compute rilcha
  b = zd0 + epch.ril_b;
  a = 2 * zd0 + epch.ril_a + b / 126;
  rilcha[1] = b % 126;
  rilcha[0] = a % 28;
  //printf("getmonthdatarilcha0 : %ld\nrilcha1 : %ld\n", rilcha[0], rilcha[1]);
  // then nyidru
  mul_lst (nyidru, nyi_drup_const, zd0, 27, 67);
  add_lst (nyidru, nyidru, epch.nyid, 27, 67);
  // then gzadru
  mul_lst (gzadru, gza_drup_const, zd0, 7, 707);
  add_lst (gzadru, gzadru, epch.gzad, 7, 707);
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
nyi_dagp_and_gza_dagp (long int nyibar[6], long int tsebar[6],
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
 *  - spi zagfp
 */
int
get_tt_data (epoch epch, long int cur_mth, long int gzadru[6],
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
      nyi_dagp_and_gza_dagp (nyibar, tsebar, rilcha, tt, nyidag, gzadag);
    }
  else
    {
      nyi_dagp_and_gza_dagp (nyidru, gzadru, rilcha, 0, nyidag, gzadag);
    }
  return spi_zagfp (epch, cur_mth, tt, gzadag[0]);
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
spi_zagfp (const epoch epch, long int cur_mth, long int tt, long int sd)
{
  long int spizag;		// the result
  long int b, c;		// intermediate values
  spizag = cur_mth * 30 + tt;
  c = spizag + epch.spz_c;
  b = spizag + epch.spz_b;
  b = b + c / 707;
  spizag = spizag - b / 64;
  c = (spizag + epch.spz_f) % 7;
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
  return spizag + epch.spz_j;
}
