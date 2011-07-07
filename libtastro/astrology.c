#include "tbday.h"
#include "utils.h"

/*
 * Very simple function to calculate yearly informations in indian and chinese
 * astrology. See KTC p. 142.
 * Takes a month as input, and fills its rabjung, yor, year_animal, year_element
 * and year_gender fields.
 */

void get_year_astro(tib_month *month)
{
  if ( month->year < 1027 )
      {
        month->rabjung = 0;
        month->yor = 0;
      }
    else
     {
       month->rabjung = ((month->year - 1027) / 60 ) + 1;
       month->yor = (unsigned char) (month->year - 1027) - 60 * ( month->rabjung - 1 );
     } 
    month->year_animal = (unsigned char) ( month->year + 1200 - 7 ) % 12;
    month->year_element = (unsigned char) (( month->year + 1200 ) / 2 - 3 ) % 5;
    month->year_gender = (unsigned char) (month->year % 2); // 1 = female, odd; 0
    month->year_sme_ba = (unsigned char) (1 + ( 3007 - month->year ) % 9); // the nine numbers
    // WARNING: do not compute years superior to 3007 with this function!
    
}

/* Function to calculate daily informations. Currently it fills the lm_db, yoga
 * and karana fields of a tib_day.
 * See KTC p.42 for the details.
 */
void get_day_infos(tib_day *td)
{
  long int moonlong[5] = {0,0,0,0,0}; // moon longitude at daybreak
  long int lista[5], listb[5];
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
    td->lm_db = (unsigned char) moonlong[0];

   // Now calculate yoga, sbyor ba:
   // we simply add the true solar longitude and the true moon longitude
   // at daybreak.
   // This is strictly wrong, we should use the Sun's longitude at daybreak, 
   // but in the Tibetan tradition such an adjustment is not made.
    add_lst ( lista, moonlong, td->nyidag, 27, 67 );
    td->yoga = (unsigned char) lista[0];

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
    td->karana = (unsigned char) (( lista[0] * 60 + lista[1]) / 27);
    if ( td->karana == 0 )
      td->karana = 7;
    else if ( td->karana == 57 )
      td->karana = 8;
    else if ( td->karana == 58 )
      td->karana = 9;
    else if ( td->karana == 59 )
      td->karana = 10;
    else
      td->karana = ( td->karana - 1 ) % 7;
}
