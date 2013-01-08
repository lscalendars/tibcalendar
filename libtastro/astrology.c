#include "tbstructures.h"
#include "utils.h"

/*
 * Very simple function to calculate yearly informations in indian and chinese
 * astrology. See KTC p. 142.
 * Takes a month as input, and fills its rabjung, yor, year_animal, year_element
 * and year_gender fields.
 */

void get_year_astro(tib_year *year)
{
  if (!year->astro_data)
    year->astro_data = new_tib_year_astro();
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

void get_month_astro(tib_month *month)
{
  if (!month->year)
    printf("error: you should call get_month_astro with a month with valid year field");
  if (!month->astro_data)
    month->astro_data = new_tib_month_astro();
  // we need year astrological data to get the month astrological data:
  if (!month->year->astro_data)
    get_year_astro(month->year);
  }
    
    
/* Function to calculate daily informations. Currently it fills the lm_db, yoga
 * and karana fields of a tib_day.
 * See KTC p.42 for the details.
 */
void get_day_astro(tib_day *td)
{
    if (!td->astro_data)
    td->astro_data = new_tib_day_astro();
  long int moonlong[5] = {0,0,0,0,0}; // moon longitude at daybreak
  long int lista[5], listb[5];
  long int tmp;
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
    td->astro_data->lm_db = (unsigned char) moonlong[0];

   // Now calculate yoga, sbyor ba:
   // we simply add the true solar longitude and the true moon longitude
   // at daybreak.
   // This is strictly wrong, we should use the Sun's longitude at daybreak, 
   // but in the Tibetan tradition such an adjustment is not made.
    add_lst ( lista, moonlong, td->nyidag, 27, 67 );
    td->astro_data->yoga = (unsigned char) lista[0];

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
    tmp = tmp / 27; // 21600/583200 = 1/27
    // now the number of nadis:
    td->astro_data->sideral_day[2] = (unsigned char) (tmp % 60);
    tmp = tmp/60; // tmp is the number of days:
    td->astro_data->sideral_day[1] = (unsigned char) (tmp % 30);
    td->astro_data->sideral_day[0] = (unsigned char) (tmp / 30);
   
   //for the next values, the most simple computation is with the julian day: 
   td->astro_data->animal = (unsigned char) ((td->gd - 2L ) % 12L);
   td->astro_data->element = (unsigned char) (((td->gd - 3L ) / 2L ) % 5L);
   td->astro_data->c_lunar_mansion = (unsigned char) ((td->gd - 17L ) % 28L);
   td->astro_data->s_sme_ba = (unsigned char) ((td->gd - 2L ) % 9L + 1L);
   //td->ld_sme_ba = 
   
}
