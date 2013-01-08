#include <stdio.h>
#include "tbcal.h"
#include "tbday.h"
#include "system.h"
#include "../cli/translation.h" // TODO: horrible... all this needs reorganization
#include "../cli/ui.h" // TODO: horrible... all this needs reorganization
#include "jd.h" // ?
#include "astrology.h"

void print_year_info(FILE *f, tib_year *year, astro_system *asys)
 {
    fprintf (f, "New Year: %ld, %s-%s-%s\n", year->year,
                                  get_element_5_str(year->astro_data->element),
                                  get_gender_str(year->astro_data->gender),
                                  get_animal_str(year->astro_data->animal));
 }
 
void print_month_info(FILE *f, tib_month *month, astro_system *asys)
 {
  switch(month->month_type)
   {
   // TODO: factorize code
   case FIRST_OF_DOUBLE:
          fprintf (f, "Tibetan Lunar Month: %ld (Intercalary) - %s-%s-%s",
          month->month, get_animal_str(month->astro_data->animal),
          get_gender_str(month->astro_data->gender), get_animal_str(month->astro_data->animal));
          break;
    case SECOND_OF_DOUBLE: // TODO: there was a reference to epch !=2, see why
          fprintf (f, "Tibetan Lunar Month: %ld (Delayed) - %s-%s-%s",
          month->month, get_animal_str(month->astro_data->animal),
          get_gender_str(month->astro_data->gender), get_animal_str(month->astro_data->animal));
          break;
    default:
          fprintf (f, "Tibetan Lunar Month: %ld - %s-%s-%s",
          month->month, get_animal_str(month->astro_data->animal),
          get_gender_str(month->astro_data->gender), get_animal_str(month->astro_data->animal));
          break;
    }
 }
 
void print_day_info(FILE *f, tib_day *day, astro_system *asys)
 {
   
 }

/* Function returning the tib_day of a new year */
// TODO: rewrite (or write another function) to find the tib_day for any tibetan date
tib_day*
get_new_year(int year, astro_system *asys)
 {
   tib_day *td;
  tib_month *month;
 long int zd[2];
 unsigned char zeromthfg;
 long int adj_mth;
 long int prv_gd, nxt_gd;
 long int zd0rec; // used to cancel the potential effects on zd[0] of adj_zla
  long int nyibar[6] = { 0, 0, 0, 0, 0, 0 };	// mean solar longitude
  long int nyidag[6] = { 0, 0, 0, 0, 0, 0 };	// true solar longitude
  long int gzadag[6] = { 0, 0, 0, 0, 0, 0 };	// true weekday
  
td = new_tib_day();
month = td->month;
// this is always true
month->year->year = (long int) year;
month->month = 1L;
 /*    Finding the month   */
 // the idea here is to get the data for 1 month of the year, directly in Tibetan date, not passing
 // through julian date
// so first we get the true month for this month:
zla_dag (asys->epoch, (long int) year, 1, zd);
zd0rec = zd[0]; // we remember the true month main part
// we adjust the month:
adj_mth = adj_zla (1, zd, &zeromthfg, asys);
// now zd is set, we check if there are some subltelties with double month
// if adj_mth is the last month of previous year, we take the following one
if ( adj_mth == 12L ) // TODO: test for 1935, 2000 (Phugpa)
      {
        if ( !zeromthfg ) // Should always work TODO: really needed?
          {
          //zla_dag(asys->epoch, (long int) year, 2, zd);
          next_zla_dag(zd);
          adj_mth = adj_zla (2, zd, &zeromthfg, asys);
          if ( adj_mth == -1L ) // TODO: test for 1935, 2000 (Phugpa)
      {
        zd[0] = zd0rec;
        zeromthfg = 0;
        month->month_type = FIRST_OF_DOUBLE;
      }
          }
        else
          // we shouldn't arrive at the first of a double 12th month of previous year...
          printf("strange error(12): please report it to the tibastro developpers.");
      }
    // if we are at the first month of a double month, we cancel the changes brought by adj_mth because we are not yet in a loop
    else if ( adj_mth == -1L ) // TODO: test for 1935, 2000 (Phugpa)
      {
        zd[0] = zd0rec;
        zeromthfg = 0;
        month->month_type = FIRST_OF_DOUBLE;
      }
      // now we can fill a few more fields
     month->true_month[0] = zd[0];
	   month->true_month[1] = zd[1];
	   td->month = month;
	   get_month_data (asys->epoch, zd[0], month->rilcha, month->nyidru, month->gzadru);
     // TODO: test the day loop for Phugpa 1977: first day is ommited
    // now we look for the first day
     td->gd = get_tt_data (asys->epoch, zd[0], month->gzadru, month->nyidru, month->rilcha, 1, td->nyidag, td->gzadag, td->nyibar);
     // we have our day
     // TODO: if the day is ommited, do we want previous one or next one? do we change month or not?
  // td->ommited and td->duplicated are the only variables that are not set, we
  // must give them a default value
  td->ommited = NORMAL;
  td->duplicated = NORMAL;
// We now have data for the current lunar day and both the one before and following.    
// we can set the type of day
  prv_gd = get_tt_data (asys->epoch, zd[0], month->gzadru, month->nyidru, month->rilcha, 0, nyidag, gzadag, nyibar); // TODO: does it really work with 0? KTC23 seems to say so
  td->tt=1;
  if (prv_gd == td->gd) // means that the lunar date is ommited, we go to the next one
    {
    td->tt=2;
      //prv2_gd = prv_gd; 
      prv_gd = td->gd;
      td->gd = get_tt_data (asys->epoch, zd[0], month->gzadru, month->nyidru, month->rilcha, 2, td->nyidag, td->gzadag, td->nyibar);
      td->ommited=PREVIOUS_OMMITED;
      nxt_gd = get_tt_data (asys->epoch, zd[0], month->gzadru, month->nyidru, month->rilcha, 3, nyidag, gzadag, nyibar);
    }
  else if (td->gd - prv_gd == 2) // means that the lunar date is duplicated, we go to the first one
   {
   // we take the first one:
   td->duplicated=FIRST_OF_DUPLICATED;
   //TODO: useful?
   td->gzadag[0] = (td->gzadag[0]+6)%7;
   td->gzadag[1] = 60L; td->gzadag[2] = 0L; td->gzadag[3] = 0L; td->gzadag[4] = 0L; td->gzadag[5] = 0L;
   // if we took the second one:
   //td->duplicated=SECOND_OF_DUPLICATED;
   nxt_gd = get_tt_data (asys->epoch, zd[0], month->gzadru, month->nyidru, month->rilcha, 2, nyidag, gzadag, nyibar);
   }
  else
   {
     nxt_gd= get_tt_data (asys->epoch, zd[0], month->gzadru, month->nyidru, month->rilcha, 2, nyidag, gzadag, nyibar);
   }
  if (nxt_gd == td->gd)		// Few tested, all working properly
    {
      td->ommited = NEXT_OMMITED;
    }
/*  if (prv2_gd == prv_gd)	// prv2_gd is set to 0 if we didn't check for previously ommited, thus it will be different from gd... TODO: uncomment when it will make sense
    {
      td->ommited = PREVIOUS_OMMITED;
    }*/
    return td;
 }

void print_tib_cal(FILE *f, int year, astro_system *asys)
 {
   tib_day *td = get_new_year(year, asys);
   get_day_astro(td);
   print_tib_day(td);
 }
