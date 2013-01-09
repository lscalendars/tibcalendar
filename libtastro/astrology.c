#include "tbstructures.h"
#include "utils.h"
#include "astrology.h"

/*
 * Very simple function to calculate yearly informations in indian and chinese
 * astrology. See KTC p. 142.
 * Takes a month as input, and fills its rabjung, yor, year_animal, year_element
 * and year_gender fields.
 */

void get_year_astro_data(tib_year *year)
{
  if (!year->astro_data)
    year->astro_data = new_tib_year_astro_data();
  if ( year->year < 1027 )
      {
        year->astro_data->rabjung = 0;
        year->astro_data->yor = 0;
      }
    else
     {
       year->astro_data->rabjung = ((year->year - 1027) / 60 ) + 1;
       year->astro_data->yor = (unsigned char) (year->year - 1027) - 60 * (year->astro_data->rabjung - 1 );
     } 
    year->astro_data->animal = (unsigned char) ( year->year + 1200 - 7 ) % 12;
    year->astro_data->element = (unsigned char) (( year->year + 1200 ) / 2 - 3 ) % 5;
    year->astro_data->gender = (unsigned char) (year->year % 2); // 1 = female, odd; 0 // TODO: test with negative years
    year->astro_data->sme_ba = (unsigned char) (1 + ( 3007 - year->year ) % 9); // the nine numbers
    // WARNING: do not compute years superior to 3007 with this function!
}

void get_month_astro_data(tib_month *month, astro_system *asys)
{
  unsigned char tmp_e, tmp_g; // for a small optimization
  unsigned char month_number = (unsigned char) month->month; // easier to work with
  if (!month->year)
    printf("error: you should call get_month_astro with a month with valid year field");
  if (!month->astro_data)
    month->astro_data = new_tib_month_astro_data();
  // we need year astrological data to get the month astrological data:
  if (!month->year->astro_data)
    get_year_astro_data(month->year);
  // we set the two values...
  tmp_e = month->year->astro_data->element;
  tmp_g = month->year->astro_data->gender;
        if ( asys->type == TSURPHU )
          {
            // TODO: indicate the source. Code taken from tcg
            month->astro_data->animal = (month_number + 10 ) % 12;
            month->astro_data->c_month = month_number;
            // element
            if ( ( tmp_e == WOOD && tmp_g == MALE ) || ( tmp_e == EARTH && tmp_g == FEMALE ) )
              month->astro_data->element = ((month_number - 1 ) / 2)%5;
            if ( ( tmp_e == WOOD && tmp_g == FEMALE ) || ( tmp_e == IRON && tmp_g == MALE ) )
              month->astro_data->element = (1 + (month_number - 1 ) / 2)%5;
            if ( ( tmp_e == FIRE && tmp_g == MALE ) || ( tmp_e == IRON && tmp_g == FEMALE ) )
              month->astro_data->element = (2 + (month_number - 1 ) / 2)%5;
            if ( ( tmp_e == EARTH && tmp_g == MALE ) || ( tmp_e == WATER && tmp_g == FEMALE ) )
              month->astro_data->element = (4 + (month_number - 1 ) / 2)%5;
            if ( ( tmp_e == FIRE && tmp_g == FEMALE ) || ( tmp_e == WATER && tmp_g == MALE ) )
              month->astro_data->element = (3 + (month_number - 1 ) / 2)%5;
          }
        else
          {
            month->astro_data->animal = month_number % 12;
            month->astro_data->c_month = (month_number + 2) % 12;
            month->astro_data->element = (tmp_e + 1 + (month_number + 1 ) / 2)%5;
          }
          
        // TODO: really understand... Is it really for Tsurphu too?
        if (tmp_g == MALE && month_number > 10)
           month->astro_data->element = tmp_e + (month_number + 1) /2;
           
        // finally, the gender:
        month->astro_data->gender = (month_number + 1 ) % 2;
  }
    
    
/* Function to calculate daily informations. Currently it fills the lm_db, yoga
 * and karana fields of a tib_day.
 * See KTC p.42 for the details.
 */
void get_day_astro_data(tib_day *td, astro_system *asys)
{
    if (!td->astro_data)
    td->astro_data = new_tib_day_astro_data();
  long int moonlong[5] = {0,0,0,0,0}; // moon longitude at daybreak
  long int lista[5], listb[5];
  long int tmp;
  unsigned char tmp_uc;
  // Calculate lunar mansion at daybreak:
  // The idea here is that we computed the solar longitude at a time where the
  // sun and the moon are apart from tt*12° (54 nadis), so we can compute the
  // moon longitude
    moonlong[1] = 54;  // 1/30th of a revolution
    mul_lst ( moonlong, moonlong, td->tt, 27, 67 );
    add_lst ( moonlong, moonlong, td->nyidag, 27, 67 );
    // now we substract the hours, knowing that the moon moves one angular nadi
    // in one time nadi. We have the hours in the true weekday
    copy_lst(listb, td->gzadag);
    // method is not sure... see KTC p.43
    if ( td->duplicated == FIRST_OF_DUPLICATED ) 
      listb[0] = 1;
    else
      listb[0] = 0;
    listb[4] = ( 67 * listb[4] ) / 707;
    // This gives moon longitude at daybreak:
    sub_lst ( moonlong, moonlong, listb, 27, 67 );
    copy_lst(td->astro_data->moonlong_db, moonlong); // TODO: could be optimized a little...

   // Now calculate yoga, sbyor ba:
   // we simply add the true solar longitude and the true moon longitude
   // at daybreak.
   // This is strictly wrong, we should use the Sun's longitude at daybreak, 
   // but in the Tibetan tradition such an adjustment is not made.
    add_lst (td->astro_data->yoga, moonlong, td->nyidag, 27, 67 );

    // Now calculate karana, byed pa:
    // Karanas are numbered from 0 to 7 for the changing karanas, and from 7 to
    // 10 for the fixed ones.
    // lista is the angle between the sun and the moon (same error as in the
    // yoga calculation)
    sub_lst ( lista, moonlong, td->nyidag, 27, 67 );
    // lista[0]*60+lista[1] is the number of nadis of the angle
    // now a trick: in 1/60th of a full circle, there are 27 nadis
    // as there are 60 nadis in 1/27th of a full circle.
    // so now we can set td->karana to a temporary value which is the number of
    // 60th of a full circle. (Karana is half a lunar day, so 6°, so 1/60th of a
    // full circle).
    td->astro_data->karana = (unsigned char) (( lista[0] * 60 + lista[1]) / 27);
    if ( td->astro_data->karana == 0 )
      td->astro_data->karana = 7;
    else if ( td->astro_data->karana == 57 )
      td->astro_data->karana = 8;
    else if ( td->astro_data->karana == 58 )
      td->astro_data->karana = 9;
    else if ( td->astro_data->karana == 59 )
      td->astro_data->karana = 10;
    else
      td->astro_data->karana = ( td->astro_data->karana - 1 ) % 7;
    
    // now computing the sideral day data corresponding to the mean solar longitude
    tmp = ( ( td->nyibar[0] * 60 + td->nyibar[1] ) * 60 + td->nyibar[2] ) * 6 + td->nyibar[3];
    // tmp is now the number of shvasa (1/583200th of circle) of the solar longitude
    // we want to divide it in 12*30*60th (21600th) of a circle:
    tmp = tmp / 27L; // 21600/583200 = 1/27
    // now the number of nadis:
    td->astro_data->sideral_day[2] = (unsigned char) (tmp % 60L);
    tmp = tmp/60L; // tmp is the number of days:
    td->astro_data->sideral_day[1] = (unsigned char) (tmp % 30L);
    td->astro_data->sideral_day[0] = (unsigned char) (tmp / 30L);
   
   // Solar day data:
   // for these values, the most simple computation is with the julian day
   td->astro_data->s_animal = (unsigned char) ((td->gd - 2L ) % 12L);
   td->astro_data->element = (unsigned char) (((td->gd - 3L ) / 2L ) % 5L);
   td->astro_data->c_lunar_mansion = (unsigned char) ((td->gd - 17L ) % 28L);
   td->astro_data->s_sme_ba = (unsigned char) ((td->gd - 2L ) % 9L + 1L);

   // for all further computation, we need the month number and the astrological data for this month
   if (!td->month)
      return;
   if (!td->month->astro_data)
     get_month_astro_data(td->month, asys);

   // Lunar day data:
   // first we compute the value (chinese_month -1)*30 + tt that will be used later on:
   // it is the number of lunar days since first chinese month, then we can derive the useful values
   tmp_uc = (td->month->astro_data->c_month - 1) * 30 + (unsigned char) td->tt;
   // If Chinese month is number 1, Trigram is Li, index = 1
   td->astro_data->trigram = tmp_uc % 8;
   // If Chinese month is number 1, lunar "sme ba" is 1
   td->astro_data->l_sme_ba = tmp % 9;
   if ( td->astro_data->l_sme_ba == 0 )
      td->astro_data->l_sme_ba = 9;
   // If Chinese month is number 1, Animal is Tiger, index = 11
   td->astro_data->l_animal = (tmp_uc + 10) % 12;
    
   // now checking for Earth-lords:
   check_sadag((unsigned char) td->month->month, (unsigned char) td->tt, td->astro_data);
   check_anniversary ((unsigned char) td->month->month, (unsigned char) td->tt, td->month->type, td->astro_data, asys );
}

// Two small functions to get the lunar mansion element and day of week element
// first the data
static unsigned char lunar_mansion_elements[27] = { 3, 2, 2, 0, 3, 1, 3, 2, 1, 2, 2, 3, 3, 3, 3, 2, 0, 0,
                       1, 1, 0, 0, 1, 0, 2, 1, 1 };
unsigned char get_lunar_mansion_element (unsigned char lm)
{
  return lunar_mansion_elements[lm];
}

static unsigned char dow_elements[7] = { 0, 2, 1, 2, 1, 3, 0 };
unsigned char get_dow_element (unsigned char dow)
{
  return dow_elements[dow];
}

/*
 *
 * Earth Lords
 *
 */

// Check for main Earth-lords, "sa bdag"  
// m is the month and t is the day in the month
// they are casted as unsigned char instead of long int in order to reduce the cost
void check_sadag (unsigned char m, unsigned char t, tib_day_astro_data *tda)
  {
// First, "yan kwong": // Data from Kongleg.
    if ( ( m == 1 && t == 13 ) || 
    ( m == 2 && t == 11 ) || 
    ( m == 3 && t == 9 ) || 
    ( m == 4 && t == 7 ) || 
    ( m == 5 && t == 5 ) || 
    ( m == 6 && t == 3 ) || 
    ( m == 7 && t == 1 ) || 
    ( m == 7 && t == 29 ) || 
    ( m == 8 && t == 27 ) || 
    ( m == 9 && t == 25 ) || 
    ( m == 10 && t == 23 ) || 
    ( m == 11 && t == 21 ) || 
    ( m == 12 && t == 19 ) 
    )
    tda->yk = 1;

// Now, "zin phung": These data are taken from Mongolian data in VKP2.DOC TODO: ?
    if (
    (( m == 1 || m == 2 || m == 3 ) && ( t == 1 || t == 7 || t == 13 || t == 19 || t == 25 )) ||
    (( m == 4 || m == 5 || m == 6 ) && ( t == 6 || t == 12 || t == 18 || t == 24 || t == 30 )) ||
    (( m == 7 || m == 8 || m == 9 ) && ( t == 3 || t == 9 || t == 15 || t == 21 || t == 27 )) ||
    (( m == 10 || m == 11 || m == 12 ) && ( t == 4 || t == 10 || t == 16 || t == 22 || t == 28 ))
    )
    tda->zph = 1;

// Now, "klu bzlog":
    if (
    (( m == 1 ) && ( t == 5 || t == 10 || t == 15 )) ||
    (( m == 2 ) && ( t == 8 || t == 18 || t == 20 || t == 22 || t == 28 )) ||
    (( m == 4 ) &&  ( t == 20 || t == 25 )) ||
    (( m == 5 ) && ( t == 8 || t == 15 )) ||
    (( m == 6 ) && ( t == 11 || t == 13 || t == 15 || t == 23 )) ||
    (( m == 7 ) && ( t == 5 || t == 6 )) ||
    (( m == 8 ) && ( t == 3 || t == 6 || t == 9 || t == 13 || t == 16 )) ||
    (( m == 9 ) && ( t == 9 || t == 10 || t == 19 )) ||
    (( m == 10 ) && ( t == 9 || t == 10 || t == 19 || t == 26 )) ||
    (( m == 11 ) && ( t == 2 || t == 6 || t == 16 || t == 20 || t == 26 ))
    )
    tda->kbz = 1;

// Now, "klu thebs":
    if (
       ( ( m == 1 ) && ( t == 14 )) ||
       (( m == 2 ) && ( t == 10 )) ||
       (( m == 3 ) && ( t == 25 )) ||
       (( m == 4 ) && ( t == 8 || t == 15 )) ||
       (( m == 5 ) && ( t == 20 || t == 22 )) ||
       (( m == 6 ) && ( t == 5 || t == 20 || t == 25 )) ||
       (( m == 7 ) && ( t == 9 || t == 19 )) ||
       (( m == 8 ) && ( t == 5 || t == 15 )) ||
       (( m == 9 ) && ( t == 1 || t == 11 || t == 21 || t == 22 || t == 23 )) ||
       (( m == 10 ) && ( t == 8 || t == 15 || t == 18 )) ||
       (( m == 11 ) && ( t == 7 || t == 15 || t == 21 ))
       )
       tda->kth=1;

// Now, "nyi nag": // Data from Vaidurya dkar po.
// value is 2 if month is 1 (the string to print is different (?)
    if ( m == 1 && t == 7 )
      tda->nn = 2;
    else if (( m == 2 && t == 14 ) ||
    ( m == 3 && t == 21 ) ||
    ( m == 4 && t == 8 ) ||
    ( m == 5 && t == 16 ) ||
    ( m == 6 && t == 24 ) ||
    ( m == 7 && t == 9 ) ||
    ( m == 8 && t == 18 ) ||
    ( m == 9 && t == 27 ) ||
    ( m == 10 && t == 10 ) ||
    ( m == 11 && t == 20 ) ||
    ( m == 12 && t == 30 ))
    tda->nn=1;
  } 
  
// Routine to check for special anniversaries linked with the life of the Buddha.
// At the end, some commentaries show more anniversaries.
// It simply fills the tda->anniversary field
// Month numbers would currently be wrong for Error Correction system. (?)
void check_anniversary (unsigned char m, unsigned char t, unsigned char month_type, tib_day_astro_data *tda, astro_system *asys )
  {
    // if the month is delayed, we just pass, feasts are celebrated only once!
    if (month_type == SECOND_OF_DOUBLE)
      return;
    switch ( m )
      {
        case 1:
          if ( t == 1 )
            tda->anniversary = ANN_DEM_MIR;
        break;
        case 3:
          if ( t == 15 )
           tda->anniversary = ANN_REV_KALACAKRA;
        break;
        case 4:
          // 7 for Phugpa, 8 for Tsurphu
          if ( t == 7 && asys->type != TSURPHU )
             tda->anniversary = ANN_BIRTH;
          else if ( t == 8 && asys->type == TSURPHU)
            tda->anniversary = ANN_BIRTH;
          else if ( t == 15 )
             tda->anniversary = ANN_ENL_PARI;
        break;
        case 6:
          if ( t == 4 )
             tda->anniversary = ANN_TURN_WHEEL;
          else if ( t == 15 )
             tda->anniversary = ANN_ENTRY_WOMB;
        break;
        case 9:
          if ( t == 22 )
             tda->anniversary = ANN_DESCENT_REALM;
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
