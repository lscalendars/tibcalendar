/*
License for Tibastro - Tibetan almanach calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "tbstructures.h"
#include "utils.h"
#include "astrology.h"

/*
 * Very simple function to calculate yearly informations in indian and chinese
 * astrology. See KTC p. 142.
 * Takes a month as input, and fills its rabjung, yor, year_animal, year_element
 * and year_gender fields.
 */

void
get_year_astro_data (tib_year * year)
{
  if (!year->astro_data)
    year->astro_data = new_tib_year_astro_data ();
  if (year->year < 1027)
    {
      year->astro_data->rabjung = 0;
      year->astro_data->yor = 0;
    }
  else
    {
      year->astro_data->rabjung = ((year->year - 1027) / 60) + 1;
      year->astro_data->yor =
	(unsigned char) ((year->year - 1027L) -
			 60L * (year->astro_data->rabjung - 1L));
    }
  year->astro_data->animal =
    (unsigned char) ((year->year + 1200L - 7L) % 12L);
  year->astro_data->element =
    (unsigned char) (((year->year + 1200L) / 2L - 3L) % 5L);
  year->astro_data->gender = (unsigned char) (year->year % 2L);	// 1 = female, odd; 0 // TODO: test with negative years
  year->astro_data->sme_ba = (unsigned char) (1L + (3007L - year->year) % 9L);	// the nine numbers
  // WARNING: do not compute years superior to 3007 with this function!
}

// a function to returning the chinese month for a specified month and system
void
get_chinese_month (tib_month * month, astro_system * asys)
{
  if (asys->type == TSURPHU)
    month->astro_data->c_month = (unsigned char) month->month;
  else if (asys->type == PHUGPA)
    month->astro_data->c_month = ((unsigned char) (month->month) + 2) % 12;
  else				// SHERAB_LING
    {
      month->astro_data->c_month = (unsigned char) month->month;
      // this code is from T4.c get_chimonth
      if (month->true_month[0] >= 14833L && month->true_month[0] <= 14847L)
	month->astro_data->c_month = month->astro_data->c_month + 1;
      else if (month->true_month[0] >= 14866L
	       && month->true_month[0] <= 14882L)
	month->astro_data->c_month = month->astro_data->c_month + 1;
      else if (month->true_month[0] >= 14900L
	       && month->true_month[0] <= 14918L)
	month->astro_data->c_month = month->astro_data->c_month + 1;
      else if (month->true_month[0] >= 14933L
	       && month->true_month[0] <= 14948L)
	month->astro_data->c_month = month->astro_data->c_month + 1;
      // month->astro_data->c_month = (unsigned char) month->month + 2; // This is from T4.c in prn_bir2_cal but it's not used
    }
  if (month->astro_data->c_month > 12)
    month->astro_data->c_month = month->astro_data->c_month - 12;
  // a little adjustment so that c_month is 1-12 instead of 0-11
  if (month->astro_data->c_month == 0)
    month->astro_data->c_month = 12;
}

void
get_month_astro_data (tib_month * month, astro_system * asys)
{
  unsigned char tmp_e, tmp_g;	// for a small optimization
  unsigned char month_number = (unsigned char) month->month;	// easier to work with
  if (!month->year)
    printf
      ("error: you should call get_month_astro with a month with valid year field");
  if (!month->astro_data)
    month->astro_data = new_tib_month_astro_data ();
  // we need year astrological data to get the month astrological data:
  if (!month->year->astro_data)
    get_year_astro_data (month->year);
  // we set the two values...
  tmp_e = month->year->astro_data->element;
  tmp_g = month->year->astro_data->gender;
  // TODO: change for switch loop
  if (asys->type == TSURPHU)
    {
      // See KTC p.198
      // TODO: instead of TSURPHU and PHUGPA, add a switch asys->element_calculation
      month->astro_data->animal = (month_number + 10) % 12;
      // element
      if ((tmp_e == WOOD && tmp_g == MALE)
	  || (tmp_e == EARTH && tmp_g == FEMALE))
	month->astro_data->element = ((month_number - 1) / 2) % 5;
      if ((tmp_e == WOOD && tmp_g == FEMALE)
	  || (tmp_e == IRON && tmp_g == MALE))
	month->astro_data->element = (1 + (month_number - 1) / 2) % 5;
      if ((tmp_e == FIRE && tmp_g == MALE)
	  || (tmp_e == IRON && tmp_g == FEMALE))
	month->astro_data->element = (2 + (month_number - 1) / 2) % 5;
      if ((tmp_e == EARTH && tmp_g == MALE)
	  || (tmp_e == WATER && tmp_g == FEMALE))
	month->astro_data->element = (4 + (month_number - 1) / 2) % 5;
      if ((tmp_e == FIRE && tmp_g == FEMALE)
	  || (tmp_e == WATER && tmp_g == MALE))
	month->astro_data->element = (3 + (month_number - 1) / 2) % 5;
    }
  else				// PHUGPA or SHERAB_LING
    {
      month->astro_data->animal = month_number % 12;
      month->astro_data->element = (tmp_e + 1 + (month_number + 1) / 2) % 5;
    }

  // now getting the chinese month
  get_chinese_month (month, asys);

  // TODO: really understand... Is it really for Tsurphu too?
  if (tmp_g == MALE && month_number > 10)
    month->astro_data->element = (tmp_e + (month_number + 1) / 2) % 5;

  // finally, the gender:
  month->astro_data->gender = (month_number + 1) % 2;
}


/* Function to calculate daily informations. Currently it fills the lm_db, yoga
 * and karana fields of a tib_day.
 * See KTC p.42 for the details.
 * updateflg is 1 if  we just need to update the fields (set for the previous day) or 0 if we need to fill everything
 */
void
get_day_astro_data (tib_day * td, astro_system * asys,
		    unsigned char updateflg)
{
  long int moonlong[5] = { 0, 0, 0, 0, 0 };	// moon longitude at daybreak
  long int lista[5];		// cannot be used unitialized
  long int listb[5] = { 0, 0, 0, 0, 0 };	// this one can
  long int tmp;

  if (!td->astro_data)
    td->astro_data = new_tib_day_astro_data ();
  // for almost all computation, we need the month number and the astrological data for this month
  if (!td->month)
    return;			//TODO: error
  if (!td->month->astro_data)
    get_month_astro_data (td->month, asys);

  // Earth-lords and anniversaries, must be checked each day
  check_sadag ((unsigned char) td->month->month, (unsigned char) td->tt,
	       td->astro_data);
  check_anniversary ((unsigned char) (td->month->month),
		     (unsigned char) (td->tt), td, asys);

  // Lunar day data:
  // this is what we do first, as it is the only data computed for ommited days
  // the lunar day data does not change for update of duplicated day (it is the same lunar day!)
  if (updateflg != 1 || td->duplicated != SECOND_OF_DUPLICATED)
    {
      // first we compute the value (chinese_month -1)*30 + tt that will be used later on:
      // it is the number of lunar days since first chinese month, then we can derive the useful values
      tmp = ((long int) (td->month->astro_data->c_month) - 1L) * 30L + td->tt;
      // trigram
      if (asys->type == SHERAB_LING)	//TODO: check
	td->astro_data->l_trigram = (unsigned char) ((tmp + 4L) % 8L);
      else			// If Chinese month is number 1, Trigram is Li, index = 1
	td->astro_data->l_trigram = (unsigned char) (tmp % 8L);
      // sme ba
      if (asys->type == SHERAB_LING)	//TODO: check 
	td->astro_data->l_sme_ba = (unsigned char) ((tmp + 6L) % 9L);
      else			// If Chinese month is number 1, lunar "sme ba" is 1
	td->astro_data->l_sme_ba = (unsigned char) (tmp % 9L);
      if (td->astro_data->l_sme_ba == 0)
	td->astro_data->l_sme_ba = 9;
      // If Chinese month is number 1, Animal is Tiger, index = 11
      td->astro_data->l_animal = (unsigned char) ((tmp + 10L) % 12L);
      // TODO: compute td->astro_data->l_element, see KTC p. 207
    }
  // if it is an ommited day, we can stop here, none of the other astrological data makes sense...
  if (td->ommited == OMMITED)
    return;

  // Solar day data:
  // Animal, element, trigram and number, are based on Rigthig, p. 113 (57a)
  // Agrees Baidkar (VKP_1, p.224)
  // Citsjam has trigram different by 4. But Citsmel agrees.
  // now an optimization: if we want just to update the data that are set for previous day and previous day is the first of a duplicated day,
  // almost everything is already set up, we just need a few solar day data adjustments (TODO: is it just unneccessary code?)
  if (updateflg == 1 && td->ommited != PREVIOUS_OMMITED)
    {
      // this is how they are traditionnaly computed, with the previous one
      td->astro_data->s_animal = (td->astro_data->s_animal + 1) % 12;
      td->astro_data->s_trigram = (td->astro_data->s_trigram + 1) % 8;
      td->astro_data->c_lunar_mansion =
	(td->astro_data->c_lunar_mansion + 1) % 28;
      td->astro_data->s_sme_ba = td->astro_data->s_sme_ba % 9 + 1;	// TODO: test
      // in the case of the update of a duplicated day, this is the only data that changes...
    }
  else				// if it's the first time, the most simple computation is with the julian day
    {
      td->astro_data->s_animal = (unsigned char) ((td->gd - 2L) % 12L);
      td->astro_data->c_lunar_mansion =
	(unsigned char) ((td->gd - 17L) % 28L);
      td->astro_data->s_sme_ba = (unsigned char) ((td->gd - 2L) % 9L + 1L);
      td->astro_data->s_trigram = (unsigned char) ((td->gd - 6L) % 8L);
    }

  // for the element, it's more simple with the general day
  td->astro_data->s_element = (unsigned char) (((td->gd - 3L) / 2L) % 5L);

  // Calculate lunar mansion at daybreak:
  // The idea here is that we computed the solar longitude at a time where the
  // sun and the moon are apart from tt*12° (54 nadis), so we can compute the
  // moon longitude
  moonlong[1] = 54L;		// 1/30th of a revolution
  mul_lst (moonlong, moonlong, td->tt, 27, asys->sun_f);
  add_lst (moonlong, moonlong, td->nyidag, 27, asys->sun_f);
  // now we substract the hours, knowing that the moon moves one angular nadi
  // in one time nadi. We have the hours in the true weekday
  // method is not sure... see KTC p.43
  if (td->duplicated == FIRST_OF_DUPLICATED)
    {
      listb[0] = 1L;
    }
  else
    {
      copy_lst (listb, td->gzadag);
      listb[0] = 0L;
      listb[4] =
	(long) (((long long int) asys->sun_f * (long long int) listb[4]) /
		707LL);
      listb[5] = 0;
    }

  // This gives moon longitude at daybreak:
  sub_lst (moonlong, moonlong, listb, 27, asys->sun_f);
  copy_lst (td->astro_data->moonlong_db, moonlong);	// TODO: could be optimized a little...

  // Now calculate yoga, sbyor ba:
  // we simply add the true solar longitude and the true moon longitude
  // at daybreak.
  // This is strictly wrong, we should use the Sun's longitude at daybreak, 
  // but in the Tibetan tradition such an adjustment is not made.
  // TODO: interestingly, this operation would imply multiplication of two values written as lists
  // and this would be the only one of such operations... maybe tibetans did not know how to do it?
  add_lst (td->astro_data->yoga, moonlong, td->nyidag, 27, asys->sun_f);

  // Now calculate karana, byed pa:
  // Karanas are numbered from 0 to 7 for the changing karanas, and from 7 to
  // 10 for the fixed ones.
  // lista is the angle between the sun and the moon (same error as in the
  // yoga calculation)
  sub_lst (lista, moonlong, td->nyidag, 27, asys->sun_f);
  // lista[0]*60+lista[1] is the number of nadis of the angle
  // now a trick: in 1/60th of a full circle, there are 27 nadis
  // as there are 60 nadis in 1/27th of a full circle.
  // so now we can set td->karana to a temporary value which is the number of
  // 60th of a full circle. (Karana is half a lunar day, so 6°, so 1/60th of a
  // full circle).
  td->astro_data->karana = (unsigned char) ((lista[0] * 60 + lista[1]) / 27);
  if (td->astro_data->karana == 0)
    td->astro_data->karana = 7;
  else if (td->astro_data->karana == 57)
    td->astro_data->karana = 8;
  else if (td->astro_data->karana == 58)
    td->astro_data->karana = 9;
  else if (td->astro_data->karana == 59)
    td->astro_data->karana = 10;
  else
    td->astro_data->karana = (td->astro_data->karana - 1) % 7;

  // if we are updating the second of a duplicated day, we can stop now, everything afterwards will be the same as the day before
  if (td->duplicated == SECOND_OF_DUPLICATED && updateflg == 1)
    return;

  // now computing the sideral day data corresponding to the mean solar longitude
  tmp =
    ((td->nyibar[0] * 60L + td->nyibar[1]) * 60L + td->nyibar[2]) * 6L +
    td->nyibar[3];
  // tmp is now the number of shvasa (1/583200th of circle) of the solar longitude
  // we want to divide it in 12*30*60th (21600th) of a circle:
  tmp = tmp / 27L;		// 21600/583200 = 1/27
  // now the number of nadis:
  td->astro_data->sideral_day[2] = (unsigned char) (tmp % 60L);
  tmp = tmp / 60L;		// tmp is the number of days:
  td->astro_data->sideral_day[1] = (unsigned char) (tmp % 30L);
  td->astro_data->sideral_day[0] = (unsigned char) (tmp / 30L);

}

// Two small functions to get the lunar mansion element and day of week element
// first the data
static unsigned char lunar_mansion_elements[27] =
  { 3, 2, 2, 0, 3, 1, 3, 2, 1, 2, 2, 3, 3, 3, 3, 2, 0, 0,
  1, 1, 0, 0, 1, 0, 2, 1, 1
};

unsigned char
get_lunar_mansion_element (unsigned char lm)
{
  return lunar_mansion_elements[lm];
}

static unsigned char dow_elements[7] = { 0, 2, 1, 2, 1, 3, 0 };

unsigned char
get_dow_element (unsigned char dow)
{
  return dow_elements[dow];
}

/*
// TODO: finish integration
// pc1 = prochen1
// pc2 = prochen2
// doweek = day of week
// lunmanx = 
void get_phrochen ( long int *pc1, long int *pc2, long int doweek, long int lunmanx,
long int nxt_lm, long int monlong1 ) // VKP, 1,148
  {
    long int x;
    x = lunmanx;
    if ( x > 21L )
      x = x + 1L;
    else if ( x == 21L && monlong1 >= 30L )
      x = x + 1L;

    if ( doweek == 1L )
      *pc1 = x;
    else if ( doweek == 0L )
      *pc1 = x - 24L;
    else
      *pc1 = x - ( doweek - 1L ) * 4L;
    if ( *pc1 < 0L )
      *pc1 = *pc1 + 28L;

    x = nxt_lm;
    if ( x > 21L )
      x = x + 1L;
    else if ( x == 21L )
      x = x + 1L;

    if ( doweek == 1L )
      *pc2 = x;
    else if ( doweek == 0L )
      *pc2 = x - 24L;
    else
      *pc2 = x - ( doweek - 1L ) * 4L;
    if ( *pc2 < 0L )
      *pc2 = *pc2 + 28L;
  }
*/


// This function fills the solar term astrological data for a solar day
// see KTC p.50
// In a part of this function we work in panipalas (3600th of unit). This is basically the same
// as working with lists with two 0 at the end, but it makes calculations simpler...
// remember that solar terms are computed for the *mean*sun, not the true sun, so calculations are simple
void
get_solar_term (tib_day * td, astro_system * asys)
{
  long int mdsm[5];		// the mean daily solar motion
  long int gzadag[5];		// a copy of gzadag, but we set gzadag[0] to 0
  long int sldb_1[5];		// solar longitude at daybreak
  long int sldb_2[5];		// solar longitude at daybreak for next day
  long int sldb_1_p, sldb_2_p;	// the same things, but expressed in panipala
  long int fstl; // the first solar term longitude, in panipalas
  long int sun_f = asys->sun_f; // by default, but we change the value for some systems
  unsigned char st1, st2;

  td->astro_data->solar_term = 0; // if there is no solar term change, we just put 0
  if (td->ommited == OMMITED)
    return;

  // setting gzadag[0] to 0, so that it represents only the hours
  // at which the lunar day starts in current solar day
  copy_lst (gzadag, td->gzadag);
  gzadag[0] = 0;

  // Comment on the numbering of solar terms:
  // * each solar term is 27;0,0,0,0 / 24 = 1;7,30,0,0 (this is why we just have to work with 360th of unit: the two last are 0)
  // * the numbering of the solar terms are 1 dbugs, 2 sgang, 2 dbugs, 3 sgang (the one which is at 0°), etc.
  // * the numbering we use here does not start with 0 but with 1 (so at the end, if the number is 0, we make it 24)
  // * first solar term (1 dbugs) starts at 345° = 23,37,30 in a tropical zodiac (see KTC p. 355)
  // * in order to have directly a good numbering (starting at 0), we should consider 0th (=last) solar term, it starts at 22,30,0
  // * the longitude used by tibetans for solar terms is the "normal" longitude minus 1;43,30, see KTC p. 51
  // * for Sherab Ling they use 1;39,0, a value given by them (TODO: understand it...)
  // * we call this value (of 1;43,30 or 1;39,0)  long_shift
  // * the 0th solar term (1 dbugs) starts at 22;30,0 - long_shift (we call it fstl)
  // * thus, if we want the solar term we are in for a certain longitude L we can do L / (fstl)
  
  // Algorithm is the following:
  // 1. get solar longitude at daybreak and next daybreak
  // 2. get solar terms for each solar longitude, if it is the same, nothing to do
  // 3. if it is not, do the difference between solar longitude of solar term and solar longitude at daybreak
  // 4. divide it by the mean solar motion, and get the time of day at which it occurs

  switch (asys->type)
    {
    case SHERAB_LING:
      // A full year is 365;15,31,1,121 (317)
      // Each year, mean sun moves 27;0,0,0,0
      // which makes 27;0,0,0,0 / 365;15,31,1,121 (317) per day
      // = 0;4,26,0,78348 (115787), a given value, that we put to the factor 4815377
      // (78348/115787)*4815377 = 3258355,06 , we safely round it to 3258355
      // it makes 0;4,26,0,3258355 (4815377)
      // we don't have to change sun_f
      // TODO: here we suppose sun_f is 4815377, might not always be the case?...
      set_lst (mdsm, 0, 4, 26, 0, 3258355L);
      // here what we take is 22;30,0 - 1;39,0 = 20;51,0 = 20*60*60 + 51 *60 = 75060 panipalas
      fstl = 75060;
      break;
    default: // we consider all other systems to be the same
      // for sun daily motion, what we use in planet calculation (see KTC p.64) is 0;4,26,0,93156 (149209)
      // if we put to a factor of 67, (93156/149209) *48 = 41,8... so it won't be a good approximation
      // instead we put gzadag to the factor 149209, in order to have a round value:
      gzadag[4] = gzadag[4] * 2227; // 149209 = 2227 * 67
      set_lst (mdsm, 0, 4, 26, 0, 93156L);
      sun_f = 149209;
      // here what we take is 22;30,0 - 1;43,30 = 20;46,30 = 74790 panipalas
      fstl = 74790;
      return;
      break;
    }
    
  // 1. get solar longitude at daybreak
  // This is actually *never* done in traditional calculations, as it requires to multiply a list by
  // another, an operation yet to be found in Tibet... computers can do it though...
  // We have the mean solar longitude at beginning of lunar day (nyibar)
  // and we have the hours from daybreak for this longitude (gzadag with gzadag[0] = 0)
  // so we just have to do nyibar - gzadag * mdsm to get solar longitude at daybreak
  mul_lst_lst (sldb_1, gzadag, mdsm, 27, sun_f);
  sub_lst (sldb_1, td->nyibar, sldb_1, 27, sun_f);
  
  // For first of duplicated days, td->gzadag and td->nyibar are set for the second day
  // so the solar longitude at daybreak is actually less by mdsm, and solar longitude at daybreak
  // of next day is the result we just found!
  if (td->duplicated == FIRST_OF_DUPLICATED)
    {
      copy_lst(sldb_2, sldb_1);
      sub_lst(sldb_1, sldb_1, mdsm, 27, sun_f);
    }
  else
    {
      // now we have solar longitude at daybreak for current day
      // we just add the mean solar motion and we get solar mean longitude for next day
      add_lst (sldb_2, sldb_1, mdsm, 27, sun_f);
    }

  // now we start to work in panipalas:
  // we convert sldb_1 and sldb_2:
  sldb_1_p = sldb_1[0] * 60L * 60L + sldb_1[1] * 60L + sldb_1[2];
  sldb_2_p = sldb_2[0] * 60L * 60L + sldb_2[1] * 60L + sldb_2[2];

  // 2. get solar terms for each solar longitude, if it is the same, nothing to do
  // to get the solar term for a longitude, the first step is to do longitude - first solar term longitude
  // which gives the longitude after 0th solar term
  sldb_1_p = sldb_1_p - fstl;
  if (sldb_1_p < 0)
     sldb_1_p = sldb_1_p + 97200;
  sldb_2_p = sldb_2_p - fstl;
  if (sldb_2_p < 0)
     sldb_2_p = sldb_2_p + 97200;
  
  // one solar term longitude interval is 1*60*60 + 7*60 + 30 = 4050
  // then we can just divide by the longitude of a solar term interval in order to have the solar term for the longitude:
  st1 = (unsigned char) (sldb_1_p / 4050);
  st2 = (unsigned char) (sldb_2_p / 4050);
  
  if (st1 == st2) // the two longitudes are in the same solar term, so there is no solar term change
    return;

    // 3. if it is not, do the difference between solar longitude of (new) solar term and first solar longitude at daybreak
    // the solar term longitude is st2 *4050
    // we use sldb_2 and sldb_2_p to store the value of the difference of longitude
    // first we compute the longitude of solar term
    sldb_2_p = st2*4050 + fstl; 
    if (sldb_2_p > 97200)
       sldb_2_p = sldb_2_p - 97200;
    // we make it a whole list
    sldb_2[2] = sldb_2_p % 60;
    sldb_2_p = sldb_2_p / 60;
    sldb_2[1] = sldb_2_p % 60;
    sldb_2[0] = sldb_2_p / 60;
    sldb_2[3] = 0; sldb_2[4] = 0;
    // and we substract the solar longitude at daybreak
    sub_lst(sldb_2, sldb_2, sldb_1, 27, sun_f);
    // now sldb_2 is the difference of longitudes between solar term and mean solar longitude at daybreak
    
    // 4. divide it by the mean solar motion, and get the time of day at which it occurs
    // we just have to divide it by mean daily solar motion in order to get the time it took from daybreak to solar term
    div_lst_lst(td->astro_data->st_ct, sldb_2, mdsm, sun_f);
  
   // if the system is not SHERAB LING, we set the last term to the good factor (67 instead of 149209)
   if (asys->type != SHERAB_LING)
     td->astro_data->st_ct[4] = td->astro_data->st_ct[4] / 2227;

    // now we have the time, let's just set the solar term:
    if (st2==0)
       st2 = 24;
    td->astro_data->solar_term = st2;
}


/*
 *
 * Earth Lords
 *
 */

// Check for main Earth-lords, "sa bdag"  
// m is the month and t is the day in the month
// they are casted as unsigned char instead of long int in order to reduce the cost
void
check_sadag (unsigned char m, unsigned char t, tib_day_astro_data * tda)
{
  tda->yk = 0;
  tda->zph = 0;
  tda->kbz = 0;
  tda->kth = 0;
  tda->nn = 0;

// First, "yan kwong": // Data from Kongleg.
  if ((m == 1 && t == 13) ||
      (m == 2 && t == 11) ||
      (m == 3 && t == 9) ||
      (m == 4 && t == 7) ||
      (m == 5 && t == 5) ||
      (m == 6 && t == 3) ||
      (m == 7 && t == 1) ||
      (m == 7 && t == 29) ||
      (m == 8 && t == 27) ||
      (m == 9 && t == 25) ||
      (m == 10 && t == 23) || (m == 11 && t == 21) || (m == 12 && t == 19))
    tda->yk = 1;

// Now, "zin phung": These data are taken from Mongolian data in VKP2.DOC TODO: ?
  if (((m == 1 || m == 2 || m == 3)
       && (t == 1 || t == 7 || t == 13 || t == 19 || t == 25))
      || ((m == 4 || m == 5 || m == 6)
	  && (t == 6 || t == 12 || t == 18 || t == 24 || t == 30))
      || ((m == 7 || m == 8 || m == 9)
	  && (t == 3 || t == 9 || t == 15 || t == 21 || t == 27))
      || ((m == 10 || m == 11 || m == 12)
	  && (t == 4 || t == 10 || t == 16 || t == 22 || t == 28)))
    tda->zph = 1;

// Now, "klu bzlog":
  if (((m == 1) && (t == 5 || t == 10 || t == 15)) ||
      ((m == 2) && (t == 8 || t == 18 || t == 20 || t == 22 || t == 28)) ||
      ((m == 4) && (t == 20 || t == 25)) ||
      ((m == 5) && (t == 8 || t == 15)) ||
      ((m == 6) && (t == 11 || t == 13 || t == 15 || t == 23)) ||
      ((m == 7) && (t == 5 || t == 6)) ||
      ((m == 8) && (t == 3 || t == 6 || t == 9 || t == 13 || t == 16)) ||
      ((m == 9) && (t == 9 || t == 10 || t == 19)) ||
      ((m == 10) && (t == 9 || t == 10 || t == 19 || t == 26)) ||
      ((m == 11) && (t == 2 || t == 6 || t == 16 || t == 20 || t == 26)))
    tda->kbz = 1;

// Now, "klu thebs":
  if (((m == 1) && (t == 14)) ||
      ((m == 2) && (t == 10)) ||
      ((m == 3) && (t == 25)) ||
      ((m == 4) && (t == 8 || t == 15)) ||
      ((m == 5) && (t == 20 || t == 22)) ||
      ((m == 6) && (t == 5 || t == 20 || t == 25)) ||
      ((m == 7) && (t == 9 || t == 19)) ||
      ((m == 8) && (t == 5 || t == 15)) ||
      ((m == 9) && (t == 1 || t == 11 || t == 21 || t == 22 || t == 23)) ||
      ((m == 10) && (t == 8 || t == 15 || t == 18)) ||
      ((m == 11) && (t == 7 || t == 15 || t == 21)))
    tda->kth = 1;

// Now, "nyi nag": // Data from Vaidurya dkar po.
// value is 2 if month is 1 (the string to print is different (?)
  if (m == 1 && t == 7)
    tda->nn = 2;
  else if ((m == 2 && t == 14) ||
	   (m == 3 && t == 21) ||
	   (m == 4 && t == 8) ||
	   (m == 5 && t == 16) ||
	   (m == 6 && t == 24) ||
	   (m == 7 && t == 9) ||
	   (m == 8 && t == 18) ||
	   (m == 9 && t == 27) ||
	   (m == 10 && t == 10) ||
	   (m == 11 && t == 20) || (m == 12 && t == 30))
    tda->nn = 1;
}

/* Routine to check for special anniversaries linked with the life of the Buddha.
 * At the end, some commentaries show more anniversaries.
 * It simply fills the td->astro_data->anniversary field
 * Month numbers would currently be wrong for Error Correction system. (?)
 *
 * Note p. 17 of Rigpa Calendar for 2013:
 * " According to Men Tsee Khang, the Tibetan Medical and Astrological Institute, if a
 * " day is missing, then any special event for that day will fall on the next day. However,
 * " some do celebrate on the day before.
 * " If a day is doubled, then any anniversary or special event will fall on the first day.
 * 
 * Counter Argument of Edward Henning (in a personal mail, 7/2/2013):
 * " For omitted days, in my view it should be the day (date)
 * " before - it makes no sense at all to use the day after. For example,
 * " if you want the full moon day, the 15th, let's sayd the Monday is the
 * " I4th and Tuesday is the 16th, with the 15th omitted. The actual 15the
 * " lunar day starts very early on the Monday, and finish very late on
 * " that same day, shortly before the Tuesday begins. 
 *
 * Example of anniversary falling on an ommited day: 1/1/1977 Phugpa
 * Example of anniversary falling on duplicated day: 15/3/1451 Phugpa
 *
 * TODO: update the code
 * TODO: build a system for monthly rituals
 * TODO: build a system to add other anniversaries in options
 * TODO: make a system for anniversaries in western dates
 */
void
check_anniversary (unsigned char m, unsigned char t, tib_day * td,
		   astro_system * asys)
{
  td->astro_data->anniversary = 0;
  // if the month is delayed, or the day is the second of a double, there is not feast
  // change this if you want the same behaviour as in EH's calendars (as of february 2013)
  if (td->month->type == SECOND_OF_DOUBLE
      || td->duplicated == SECOND_OF_DUPLICATED)
    return;
    
  switch (m)
    {
    case 1:
      if (t == 1)
	td->astro_data->anniversary = ANN_DEM_MIR;
      break;
    case 3:
      if (t == 15)
	td->astro_data->anniversary = ANN_REV_KALACAKRA;
      break;
    case 4:
      // 7 for Phugpa, 8 for Tsurphu
      if (t == 7 && asys->type != TSURPHU)
	td->astro_data->anniversary = ANN_BIRTH;
      else if (t == 8 && asys->type == TSURPHU)
	td->astro_data->anniversary = ANN_BIRTH;
      else if (t == 15)
	td->astro_data->anniversary = ANN_ENL_PARI;
      break;
    case 6:
      if (t == 4)
	td->astro_data->anniversary = ANN_TURN_WHEEL;
      else if (t == 15)
	td->astro_data->anniversary = ANN_ENTRY_WOMB;
      break;
    case 9:
      if (t == 22)
	td->astro_data->anniversary = ANN_DESCENT_REALM;
      break;
    default:
      break;
    }
// 1-14 : Anniversary of Milarepa.
// 1-15 : Anniversary of Marpa.
// 2-26 : Anniversary of Taranatha.
// 3-3 : Anniversary of Karma Pakshi.
// 4-29 : Anniversary of Mipham.
// 5-10 : Birth of Guru Rinpoche. 
// Note in Rigpa Calendar for 2013 (p. 42): According to the vision of Guru Chöwang and the tradition of Mindroling Monastery, the anniversary of Guru Rinpoche's birth is celebrated on the 10th day of the 5th month
// Note in Rigpa Calendar for 2013 (p. 46): According to those who follow the tradition of Sangyé Lingpa and his terma revelation, the Lama Gongdü, Guru Rinpoche's birth is celebrated on the 10th day of the 6th month.
// 5-15 : World Local Deities Day.
// 8-19 : Anniversary of Dilgo Khyentse Rinpoche.
// 8-23 : Anniversary of Karmapa Mikyo Dorje.
// 9-16 : Anniversary of 16th Karmapa.
// 10-10 : White Tara day.
// 10-25 : Anniversary of Tshongkhapa.
// 11-3 : Anniversary of First Karmapa.
// 11-6 : Nine bad omens.
// 11-7 : Ten auspicious omens.
// 11-26 : Anniversary of Jamgon Kongtrul.
// 12-18 : Anniversary of Longchen Rabjampa.
// 12-29 : Mahakala Rituals.
}
