#include "tbday.h"

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
    //month->year_sme_ba = 1 + ( 3007 - month->year ) % 9;
}
