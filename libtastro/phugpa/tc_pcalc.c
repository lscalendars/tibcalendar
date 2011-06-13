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

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "tcn.h"
#include "ui.h"
#include "tc.ext"
#include "bcd.h"
#include "bcd.ext"

// Local to this file only:

int num_next_omit = 0;
int num_prev_omit = 0;

//long int prv_nyibar[6]; // ??? for Tsurphu planet calculations
long int hld_gzadag[6], hld_nyidag[6], hld_nyibar[6];                
long int hld_juldat, prv2_juldat, prv_juldat, cur_juldat, nxt_juldat;
//long int prv_gzadag[6], prv_nyidag[6], prv_nyibar[6];                
//long int nxt_gzadag[6], nxt_nyidag[6], nxt_nyibar[6];                

long int  spz_c, spz_b, spz_f, spz_j;
long int gzada[6];
long int nyida[6];
long int nyidm[6], gzadm[6], tsedm[6], nyilm[6];

static long int nyidmg[6] = { 2, 10, 58, 1, 17, 0 };
static long int gzadmg[6] = { 1, 31, 50, 0, 480, 0 };
static long int tsedmg[6] = { 0, 59, 3, 4, 16, 0 };
static long int nyilmg[6] = { 0, 4, 21, 5, 43, 0 }; // Each lunar day

static long int nyifac[6] = { 6, 45, 0, 0, 0, 0 };
static long int nyihaf[6] = { 13, 30, 0, 0, 0, 0 };
static long int nyibye[6] = { 4, 1, 1, 4, 6, 6};
static long int nyidom[6] = { 6, 10, 11, 10, 6, 0};

static long int gzabye[14] = { 5, 5, 4, 3, 2, 1, -1, -2, -3,
                                                  -4, -5, -5, -5, 5};
static long int gzadom[14] = { 5, 10, 15, 19, 22, 24, 25, 24, 22,
                                                   19, 15, 10, 5, 0};

/**** Figures for Generalised phug lugs, year -1000 (1 ****/

static long int gda1[6] = { 5, 35, 36, 4, 160, 0 };
static long int nda1[6] = { 26, 45, 53, 4, 26, 0 };

/**** Figures for Generalised Tshurphu, year -1000 (2)****/

static long int gda2[6] = { 5, 38, 22, 3, 701, 0 }; // OK
static long int nda2[6] = { 0, 7, 25, 0, 30, 0 }; // OK

// Routine to set main values for "grub rtsia":
// Equivalent "byed rtsis" routine removed

void set_grub ( void )
  {
    int  i;
    for ( i = 0; i < 6; ++i )
      nyidm[i] = nyidmg[i];
    for ( i = 0; i < 6; ++i )
      gzadm[i] = gzadmg[i];
    for ( i = 0; i < 6; ++i )
      tsedm[i] = tsedmg[i];
    for ( i = 0; i < 6; ++i )
      nyilm[i] = nyilmg[i];
    sun_f = 67L;
    gza_f = 707L;
  } // END - set_grub ()

// Routine to set epoch parameters

void set_epoch ( void )
  {
    char chr;
    int  i;
    epch = 0;

// Generalised phug, year -1000

    set_grub ();
    for ( i = 0; i < 6; ++i )
      gzada[i] = gda1[i];
    for ( i = 0; i < 6; ++i )
      nyida[i] = nda1[i];
    epch_yr = -1000L;
    eyr_a = 7L;  // Intercalation index
    ril_a = 6L;
    ril_b = 61L;
    spz_c = 10L;
    spz_b = 26L;
    spz_f = 5L;
    spz_j = 1355847L;
    rahupart = 93L; // 230 fraction for Rahu cycle
    dragkadd = 6663418L; // This is added for "drag po'i rkang 'dzin".

    meradd = 2080L; // Figures to be added for planets for the
    venadd = 277L; // nyin zhag dal ba. These are days of cycle.
    maradd = 4L;
    jupadd = 511L;
    satadd = 2995L;
    zlasho1 = 48L;
    zlasho2 = 49L;
    zlapure = 0;
    tsurlug = 0;
  } // END - set_epoch ()

// Main routine to return Tibetan data for a solar day
// jd is Julian day number, tdat is the structure for returning the data  
void get_tpdata ( long jd, struct tib_ddat * tdat )  
  {
    long x, num_sd, num_ld, num_mth;
    long ld, lm, last_tm, last_ty, last_nm_jd, last_tt_jd, last_zladag0, last_zladag1;
    long last_adj_mth, Cmonth, tib_y;
    long tt_test;
    int have_last_tt, prv_omit_chk, byedpa;
    int i, done;

    memset ( tdat, 0, sizeof ( *tdat) );
    jul2date ( jd ); // this places values in wd, wm & wy globals
    sprintf ( outbuf, "\nTarget: %ld - %ld/%ld/%ld", jd, wd, wm, wy );
//  printf ( "\n%s\n", outbuf );
//    fprintf ( fptgt, "%s\n", outbuf );    
    tdat->w_d = (int) wd;
    tdat->w_m = (int) wm;
    tdat->w_y = (int) wy;      
// Step forwards until we have found the correct month. Start by setting the Tibetan month
// equal to the western one minus three, as an approximation.

// Or, this: 11,312 lunar days equals 11,135 solar days.

    num_sd = jd - spz_j;
    l2bcd ( bcdx0, num_sd );
    mulbcdl ( bcdx0, bcdx0, 11312L );
    divbcdl ( bcdx1, bcdx0, 11135L );
    num_ld = bcd2l ( bcdx1 );
    num_mth = num_ld / 30L; - 1L; 
// This is either the month we want (rarely) or the one before.

    tm = wm - 3L; // tm & ty are globals. Legacy.
    ty = wy;
    tt = 0L;  // Global, used by other routines, so must be set.
    if ( tm < 1L )
      {
        --ty;
        tm = tm + 12L;
      }
    last_ty = ty;
    last_tm = tm;
    zeromthfg = 0;
    done = 0;
    do 
      {    
        if ( !zeromthfg )   // We need to use the same data, twice.
          zla_dag (ty, tm);          
        adj_zla ();
        if ( zladag[0] == num_mth ) // to here
          done = 1;
        else
          {
            last_ty = ty;
            last_tm = tm;
            last_adj_mth = adj_mth;
            last_zladag0 = zladag[0];
            last_zladag1 = zladag[1];
            last_nm_jd = juldat;
            if ( !zeromthfg )  
              ++tm;
            if ( tm > 12L )
              {
                tm = tm - 12L;
                ++ty;
              }
          } 
      } while (!done);

//  printf ( "Determined year: %ld, month: %ld (%ld), count: %ld\n", ty, tm, adj_mth, zladag[0] );

    done = 0;
    zeromthfg = 0;
    do 
      {    
        if ( !zeromthfg )   // We need to use the same data, twice.
          zla_dag (ty, tm);  // KTC 16        
        adj_zla ();
        
        sprintf ( outbuf, "Checking year: %ld, month: %ld (%ld), zla_dag: %ld", 
                          ty, tm, adj_mth, zladag[0] );
//      printf ( "%s\n", outbuf );
//        fprintf ( fptgt, "%s\n", outbuf );    
        
        cur_mth = zladag[0];
        gza_dru (cur_mth);  
        nyi_dru (cur_mth);  
        rilchaf ( cur_mth ); 
        tse_dru ( 0L );  
        nyi_lon ( 0L );
        add_gen (tsebar, gzadru, tsedru, 7L, gza_f ); // KTC 23
        add_gen (nyibar, nyidru, nyilon, 27L, sun_f ); // KTC 23
        nyi_dag (nyibar);  // Must be done before gza_dag - KTC 31
        gza_dag (tsebar);  // KTC 24            
        spi_zagf (); // This puts Julian day number into juldat.
        // Now, have we gone far enough? We need the true new Moon date 
        // immediately after or equal to our target date:
        if ( juldat >= jd )
          done = 1;
        else
          {
            last_ty = ty;
            last_tm = tm;
            last_adj_mth = adj_mth;
            last_zladag0 = zladag[0];
            last_zladag1 = zladag[1];
            last_nm_jd = juldat;
            if ( !zeromthfg )  
              ++tm;
            if ( tm > 12L )
              {
                tm = tm - 12L;
                ++ty;
              }
          } 
      } while (!done);
// We actually need the month before:
    ty = last_ty;
    tm = last_tm;
    adj_mth = last_adj_mth;
    if ( adj_mth == 12L && tm == 1L ) // Do we really need tib_y ???
      tib_y = ty - 1L;
    else      
      tib_y = ty;
    zladag[0] = last_zladag0;
    zladag[1] = last_zladag1;    
   
    sprintf ( outbuf, "Final - year: %ld, month: %ld, zla_dag: %ld", tib_y, adj_mth, zladag[0] );
    printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );    
        
    tt_test = jd - last_nm_jd - 2L;
    if ( tt_test <= 0 )
      tt_test = 1L;
    sprintf ( outbuf, "Approximate lunar date: %ld", tt_test );
//  printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );    
    
// Now, let's find our day:
    
    tt = tt_test;
    done = 0;
    
    cur_mth = zladag[0];
    gza_dru (cur_mth);  
    nyi_dru (cur_mth);  
    rilchaf ( cur_mth );    
    last_tt_jd = 0L; // Lose this later
    prv_juldat = 0L;
    have_last_tt = 0;
    do
      {
        sprintf ( outbuf, "Checking lunar day: %ld", tt );
//      printf ( "%s\n", outbuf );
//        fprintf ( fptgt, "%s\n", outbuf );    
        tse_dru ( tt );  
        nyi_lon ( tt );
        add_gen (tsebar, gzadru, tsedru, 7L, gza_f ); // KTC 23
        add_gen (nyibar, nyidru, nyilon, 27L, sun_f ); // KTC 23
        nyi_dag (nyibar);  // Must be done before gza_dag - KTC 31
        gza_dag (tsebar);  // KTC 24            
        spi_zagf (); // This puts Julian day number into juldat.    
        sprintf ( outbuf, "Juldat = %ld, jd = %ld", juldat, jd );
//      printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );            

        if ( juldat == jd )
          {
            done = 1;   // Presumably a normal day
          }
        else if ( juldat - 1L == jd )
          {
            done = 1;   // Presumably a duplicated day is indicated
          }          
        else if ( juldat - 1L == jd ) // Possibly the first of a duplicated lunar date
          {
            if ( last_tt_jd ) // we have passed the day in question
              {
                done = 1;
                ; // set flags for duplicated day.
              }
          }
        else 
          {
            ++tt;
            if ( tt > 30L ) // Big problem!!!
              {
                printf ( "\nGone beyond end of month!\n\n" );
                getch();
                exit (EXIT_FAILURE);
              }   
            for ( i = 0; i < 6; ++i ) prv_gzadag[i] = gzadag[i];
            for ( i = 0; i < 6; ++i ) prv_nyidag[i] = nyidag[i];        
            for ( i = 0; i < 6; ++i ) prv_nyibar[i] = nyibar[i];                                      
            last_tt_jd = juldat; // lose this later
            prv_juldat = juldat;
            have_last_tt = 1;
          }
      } while ( !done );
    sprintf ( outbuf, "Date found. Lunar date: %ld", tt );
//  printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );            
    
    // back up current day data:
    for ( i = 0; i < 6; ++i ) hld_gzadag[i] = gzadag[i];
    for ( i = 0; i < 6; ++i ) hld_nyidag[i] = nyidag[i];        
    for ( i = 0; i < 6; ++i ) hld_nyibar[i] = nyibar[i]; // May not be needed.
    hld_juldat = juldat;
    prv_omit_chk = 0; // Flag to indicate test for previous omitted.
//    if ( !have_last_tt ) // Then get data for previous lunar day, even if zero
//      {
        --tt;
        tse_dru ( tt );  
        nyi_lon ( tt );
        add_gen (tsebar, gzadru, tsedru, 7L, gza_f ); // KTC 23
        add_gen (nyibar, nyidru, nyilon, 27L, sun_f ); // KTC 23
        nyi_dag (nyibar);  // Must be done before gza_dag - KTC 31
        gza_dag (tsebar);              
        spi_zagf (); // This puts Julian day number into juldat.          
        // First, a check. Is it possible that current lunar day and previous both
        // calculate to same solar day? This would be if current lunar day is omitted
        // and entirely contained within the solar day. Maybe if lunar day 1 is omitted?
        // This has not been found in testing, and should not happen
        // The way we should find an omitted is when the following lunar day and
        // current calculate to same solar day.
        if ( hld_gzadag[0] == gzadag[0] ) // This is now error code
          {
            printf ( "Stopped on lunar day: %ld\n", tt );
            getch();
          }
        for ( i = 0; i < 6; ++i ) prv_gzadag[i] = gzadag[i];
        for ( i = 0; i < 6; ++i ) prv_nyidag[i] = nyidag[i];        
        for ( i = 0; i < 6; ++i ) prv_nyibar[i] = nyibar[i];                        
        prv_juldat = juldat;        
        // Check for possible omitted lunar date, prior to current
        if ( gzadag[1] >= 54L ) // 54?? This should get all of them, in Phugpa
          {
            prv_omit_chk = 1; 
            --tt;
            tse_dru ( tt );  
            nyi_lon ( tt );
            add_gen (tsebar, gzadru, tsedru, 7L, gza_f ); // KTC 23
            add_gen (nyibar, nyidru, nyilon, 27L, sun_f ); // KTC 23
            nyi_dag (nyibar);  // Must be done before gza_dag - KTC 31
            gza_dag (tsebar);   
            spi_zagf ();           
            // Probably don't need all this:
            for ( i = 0; i < 6; ++i ) prv2_gzadag[i] = gzadag[i];
//          for ( i = 0; i < 6; ++i ) prv2_nyidag[i] = nyidag[i];        
//          for ( i = 0; i < 6; ++i ) prv2_nyibar[i] = nyibar[i];                                    
            prv2_juldat = juldat;
            ++tt;
          }
        ++tt; // Restore lunar day value.
//      }  // if ( !have_last_tt )
// Now get data for next lunar day
    ++tt;
    tse_dru ( tt );  
    nyi_lon ( tt );
    add_gen (tsebar, gzadru, tsedru, 7L, gza_f ); 
    add_gen (nyibar, nyidru, nyilon, 27L, sun_f ); 
    nyi_dag (nyibar);  
    gza_dag (tsebar);              
    spi_zagf (); 
    for ( i = 0; i < 6; ++i ) nxt_gzadag[i] = gzadag[i];
    for ( i = 0; i < 6; ++i ) nxt_nyidag[i] = nyidag[i];        
    for ( i = 0; i < 6; ++i ) nxt_nyibar[i] = nyibar[i];                        
    nxt_juldat = juldat;

// restore current day data:        
    --tt; // Restore lunar day value.
    tdat->tt = tt;
    for ( i = 0; i < 6; ++i ) gzadag[i] = hld_gzadag[i];
    for ( i = 0; i < 6; ++i ) nyidag[i] = hld_nyidag[i];        
    for ( i = 0; i < 6; ++i ) nyibar[i] = hld_nyibar[i];                      
    juldat = hld_juldat;
// We now have data for the current lunar day and both the one before and following.    
// So, what do we have?
// There are these possibilites:
// 1. We have three consecutive solar days, each with different consecutive lunar dates.
// 2. The lunar date following the current one is omitted. We need to store this information.
//    Can we detect when the previous lunar date is omitted? Should be possible, but
//    only by going back yet another day. Could check for boundary conditions, above.
// 3. Current lunar date is duplicated, current solar day is first of these
// 4. Current lunar date is duplicated, current solar day is second of these ?????

    if ( nxt_juldat == juldat ) // Few tested, all working properly
      {
        // Next lunar date, tt+1 is omitted. We need to store that information    
        sprintf ( outbuf, "Next omitted found: %ld, %ld, %ld", tt+1L, tm, ty );
//      printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );  
        tdat->next_chad = (int)tt+1;
        ++num_next_omit;
//      getch ();
      }
      
      if ( prv_omit_chk ) // We have data to check for previous omitted
        {
          sprintf ( outbuf, "Previous: %ld: %ld, %ld;%ld,%ld", tt-2L, 
                    prv2_juldat, prv2_gzadag[0], prv2_gzadag[1], prv2_gzadag[2] );
//        printf ( "%s\n", outbuf );
          fprintf ( fptgt, "%s\n", outbuf );          
        
          if ( prv2_juldat == prv_juldat )
            {
              sprintf ( outbuf, "Previous omitted found: %ld, %ld, %ld", tt-1L, tm, ty );
//            printf ( "%s\n", outbuf );
              fprintf ( fptgt, "%s\n", outbuf );  
              if ( tt == 1L )
                tdat->prv_chad = 30;
              else
                tdat->prv_chad = (int) tt-1;
              ++num_prev_omit;        
            }

        }
//    if ( prv_juldat == juldat )  --- NEVER HAPPENS.
//      {
//        sprintf ( outbuf, "Omitted found: %ld, %ld, %ld", tt+1L, tm, ty );
//        printf ( "%s\n", outbuf );
//        fprintf ( fptgt, "%s\n", outbuf );            
//        getch ();
//      }      

    if ( juldat - prv_juldat == 2L ) 
      {
        // tt is duplicated
        if ( juldat != jd )
          {
            sprintf ( outbuf, "First of duplicated found: %ld, %ld, %ld", tt, tm, ty );
            tdat->lhag = 1L;
          }
        else 
          {
            sprintf ( outbuf, "Second of duplicated found: %ld, %ld, %ld", tt, tm, ty );
            tdat->lhag = 2L;
          }
//      printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );            
//      getch ();
      }       

    sprintf ( outbuf, "Previous: %ld: %ld, %ld;%ld,%ld", tt-1L, prv_juldat, prv_gzadag[0], 
                      prv_gzadag[1], prv_gzadag[2] );
//  printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );                  
    sprintf ( outbuf, "Current: %ld: %ld, %ld;%ld,%ld", tt, juldat, gzadag[0], gzadag[1], gzadag[2] );
//  printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );                  
    sprintf ( outbuf, "Next: %ld: %ld, %ld;%ld,%ld", tt+1L, nxt_juldat, nxt_gzadag[0], 
                      nxt_gzadag[1], nxt_gzadag[2] );
//  printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );                  
    tdat->juldat = juldat;

// Now we can sort out all the symbolic and other data for the date.
// Later, will calculate Rahu and the planets.
// First, the year:

    tdat->y = (int) tib_y;
    if ( tib_y < 1027 )
      {
        tdat->rab_jung = 0;
        tdat->year_val = 0;
      }
    else
     {
       tdat->rab_jung = (((int) tib_y - 1027) / 60 ) + 1;
       tdat->year_val = ((int) tib_y - 1027) - 60 * ( tdat->rab_jung - 1 );
     }
    sprintf ( outbuf, "Rabjung: %d, Year: %d", tdat->rab_jung, tdat->year_val );
//  printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );                       

    tdat->year_animal = ( (int) tib_y + 1200 - 7 ) % 12;
    tdat->year_element = ( ( (int) tib_y + 1200 ) / 2 - 3 ) % 5;
    tdat->year_gender = ( (int) tib_y + 1200L ) % 2; // 1 = female, odd; 0
    tdat->year_sme_ba = 1 + ( 3007 - (int) tib_y ) % 9;

// Now, the month:

    if ( adj_mth < 0L ) // Intercalary
      tdat->intercal = 1;
    else if ( zladag[1] == zlasho1+2L || zladag[1] == zlasho2+2L ) // Changed, 1/9/2010
      tdat->delaymth = 1;
    if ( adj_mth < 0L )
      tdat->hor_month = (int) -adj_mth;
    else
      tdat->hor_month = (int) adj_mth; 
    tdat->mthanim = tdat->hor_month % 12L;
    Cmonth = tdat->hor_month + 2L;
    if ( Cmonth > 12L )
      Cmonth -= 12L;

    tdat->mthelem = tdat->year_element + 1 + ( tdat->hor_month + 1 ) / 2;
    if ( tdat->year_gender == 0 ) // Male year, even CE number 
      {
        if ( tdat->hor_month == 11 || tdat->hor_month == 12 )
          tdat->mthelem = tdat->year_element + ( tdat->hor_month + 1 ) / 2;
      }
    tdat->mthelem = tdat->mthelem % 5;
    tdat->mthgender = ( tdat->hor_month + 1 ) % 2; // There is no month zero

// The day:

    tdat->tt = (int) tt;           

    for ( i = 0; i < 6; ++i )
      {
        tdat->gd[i] = gzadag[i];
        tdat->nd[i] = nyidag[i];      
      }

// Calculate lunar mansion at sunrise:
// add solar longitude to tithi * factor. Then subtract hours, etc., from gzadag

    for ( i = 0; i < 5; ++i )
      monlong[i] = 0L;
    clear_a_b ();
    monlong[1] = 54L;  // 1/30th of a revolution
    mul_gen ( monlong, monlong, tt, 27L, sun_f );
    add_gen ( monlong, monlong, nyidag, 27L, sun_f );
    listb[0] = 0L;
    for ( i = 1; i < 5; ++i )
      listb[i] = gzadag[i];
    listb[4] = ( sun_f * listb[4] ) / gza_f;
// This gives longitude at daybreak:
    sub_gen ( monlong, monlong, listb, 27L, sun_f );

    tdat->lunman = (int) monlong[0];

    for ( i = 0; i < 6; ++i )
      tdat->ml[i] = monlong[i];

// Now calculate yoga, sbyor ba:

// This is strictly wrong, we should use the sun's longitude at sunrise, 
// but in the Tibetan tradition such an adjustment is not made

    add_gen ( lista, monlong, nyidag, 27L, sun_f );
    tdat->sbyorba = (int) lista[0];

    for ( i = 0; i < 6; ++i )
      tdat->bl[i] = lista[i];

// Now calculate karana, byed pa:

    clear_a_b ();
    sub_gen ( lista, monlong, nyidag, 27L, sun_f ); // At sunrise for moon
    byedpa = (int) lista[0] * 60 + (int) lista[1];
    byedpa = byedpa / 27;     // OK
    if ( byedpa == 0 )
      tdat->byedpax = 7;
    else if ( byedpa == 57 )
      tdat->byedpax = 8;
    else if ( byedpa == 58 )
      tdat->byedpax = 9;
    else if ( byedpa == 59 )
      tdat->byedpax = 10;
    else
      tdat->byedpax = ( byedpa - 1 ) % 7;

    tdat->ZS_m = ( ( nyidag[0] * 60L + nyidag[1] ) * 60L + nyidag[2] ) * 6L + nyidag[3];
    tdat->ZS_m = ( ( nyibar[0] * 60L + nyibar[1] ) * 60L + nyibar[2] ) * 6L + nyibar[3];
    l2bcd ( bcdx0, tdat->ZS_m );
    mulbcdl ( bcdx0, bcdx0, 21600L );
    divbcdl ( bcdx1, bcdx0, 583200L );
    tdat->ZS_m = bcd2l ( bcdx1 );
    tdat->ZS_d = tdat->ZS_m / 60L;
    tdat->ZS_m = tdat->ZS_m % 60L;
    tdat->ZS_s = tdat->ZS_d / 30L;
    tdat->ZS_d = tdat->ZS_d % 30L;

// If Chinese month is number 1, Trigram is Li, index = 1
    tdat->ld_parx = ( (int) Cmonth - 1 ) * 30 + (int) tt;
    tdat->ld_parx = tdat->ld_parx % 8;
// If Chinese month is number 1, "sme ba" is 1
    tdat->ld_smex = ( (int) Cmonth - 1 ) * 30 + (int) tt;
    tdat->ld_smex = tdat->ld_smex % 9;
    if ( tdat->ld_smex == 0 )
      tdat->ld_smex = 9;
// If Chinese month is number 1, Animal is Tiger, index = 11
    tdat->ld_animx = ( (int) Cmonth - 1 ) * 30 + (int) tt + 10;
    tdat->ld_animx = tdat->ld_animx % 12;      

// to here    
  
  } // END - get_tdata ()  

// ROUTINE TO CALCULATE POSITIONS OF RAHU

void do_rahu ( long int m, long int tt ) // KTC 96
  {
    long int t, mth;

    mth = ( m + rahupart ) % 230L;
    t = ( ( m + rahupart ) % 230L ) * 30L + 15L;
    rahutsa[0] = 0L;
    rahutsa[1] = 0L;
    rahutsa[2] = 14L;
    rahutsa[3] = 0L;
    rahutsa[4] = 12L;

    mul_gen ( rahutsa, rahutsa, t, 27L, 23L );
    clear_a_b ();
    lista[0] = 27L;
    sub_gen ( rahudong, lista, rahutsa, 27L, 23L );
    listb[0] = 13L;
    listb[1] = 30L;
    add_gen ( rahujug, rahudong, listb, 27L, 23L );

    rahutsa[0] = 0L;
    rahutsa[1] = 0L;
    rahutsa[2] = 14L;
    rahutsa[3] = 0L;
    rahutsa[4] = 12L;
    mul_gen ( rahutsa, rahutsa, t + 15L, 27L, 23L ); // For new Moon.
    clear_a_b ();
    lista[0] = 27L;
    sub_gen ( rahudong30, lista, rahutsa, 27L, 23L );
    listb[0] = 13L;
    listb[1] = 30L;
    add_gen ( rahujug, rahudong, listb, 27L, 23L );

    sprintf ( printstring,
    "Rahu month: %ld, gdong, 15th: %ld;%ld,%ld,%ld,%ld - 30th: %ld;%ld,%ld,%ld,%ld",
mth, rahudong[0], rahudong[1], rahudong[2], rahudong[3], rahudong[4],
rahudong30[0], rahudong30[1], rahudong30[2], rahudong30[3], rahudong30[4] );
    printf ( "%s\n", printstring );
    if ( printinprog )
      prin_str ( printstring );
  } // END - do_rahu ()

// Function to calculate true month, "zla ba rnam par dag pa"
void zla_dag ( long int y, long int m ) // KTC 15
  {
    long int   yr, a, b; // c;
    yr = y - epch_yr;
    if ( epch == 2 ) // 'khrul sel
      a = 12L * yr + m - 2L;
    else
      a = 12L * yr + m - 3L;
    if ( a >= 0L )
      {
        b = 2L * a + eyr_a;
        zladag[1] = b % 65L;
        zladag[0] = a + b / 65L;
      }
    else
      {
        zladag[1] = 0L;
        zladag[0] = 0L;
      }
  } // END - zla_dag ()

// Adjust month number, if needed
void adj_zla ( void )
  {
    zlapure = 0; // Block this for now
    if ( zlapure )
      {
        if ( zladag[1] == 0L || zladag[1] == 1L )
          {
            if ( !zeromthfg )
              {
                zeromthfg = 1;
//              adj_mth = tm - 1L;
                adj_mth = -tm;
                zladag[0] = zladag[0] - 1L;
              }
            else
              {
                zeromthfg = 0;
                adj_mth = tm;
                zladag[0] = zladag[0] + 1L;
              }
          }
        else
          {
            adj_mth = tm;
            if ( adj_mth == 0L )
              adj_mth = 12L;
          }
        return;
      }
// ELSE, DO THIS:
    if ( zladag[1] == zlasho1 || zladag[1] == zlasho2 )
      adj_mth = -tm;
    else if ( zladag[1] > zlasho2 )
      adj_mth = tm - 1L;
    else if ( zladag[1] == 0L || zladag[1] == 1L )
      {
        if ( !zeromthfg )
          {
            zeromthfg = 1;
            adj_mth = tm - 1L;
            zladag[0] = zladag[0] - 1L;
          }
        else
          {
            zeromthfg = 0;
            adj_mth = tm;
            zladag[0] = zladag[0] + 1L;
          }
      }
    else  // Arrive here if 1 < zladag[1] < zlasho1
      adj_mth = tm;
    if ( adj_mth == 0L )
      adj_mth = 12L;
  } // - adj_zla ()

// Function to calculate lunar anomaly, "ril cha"
void rilchaf ( long int x ) // KTC 21
  {
    long int   a, b;
    b = x + ril_b;
    a = 2L * x + ril_a + b / 126L;
    rilcha[1] = b % 126L;
    rilcha[0] = a % 28L;
  } // END - void rilchaf ( long int x )

// Function to calculate general day, "spyi zhag"
// Calculates Julian for the integer Julian day count at noon UT
// on weekday of "gza' dag". Put into juldat.
void spi_zagf ( void ) // KTC 46
  {
    long int b, c;
    spizag = cur_mth * 30L + tt;
    c = spizag + spz_c;
    spz_frac_c = c % 707L;
    b = spizag + spz_b;
    b = b + c / 707L;
    spz_frac_b = b % 64L;
    spizag = spizag - b / 64L;
    c = ( spizag + spz_f ) % 7L;
    b = gzadag[0];
    if ( c != b )
      {
        if ( c > 4L && b < 2L )
          b += 7L;
        else if ( b > 4L && c < 2L )
          c += 7L;
        spizag = spizag + b - c;
      }
    if ( b - c > 2L )
      printf ( "\nERROR IN GENERAL DAY ROUTINE: %ld\n", b - c );
    juldat = spizag + spz_j;
    jul2date ( juldat );
  } // - spi_zagf ();

long int gregd2jul ( long int D, long int M, long int Y )
{
  long int JD;
  int calndar;  // Gregorian = 1.

// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

  if ( Y > 1582L )
    calndar = 1;
  else if ( Y < 1582L )
    calndar = 0;
  else
    {
      if ( M > 10L )
        calndar = 1;
      else if ( M < 10L )
        calndar = 0;
      else
        {
          if ( D >= 15L )
            calndar = 1;
          else
            calndar = 0;
        }
    }
  if ( calndar )  // Gregorian:
    {
// The following is from ESAA, 1992, p. 604.

  JD = D + ( 1461 * ( Y + 4800L + ( M - 14L ) / 12L ) ) / 4L
         + ( 367L * ( M - 2L - 12L * ( ( M - 14L ) / 12L ) ) ) / 12L
         - ( 3L * ( ( Y + 4900L + ( M - 14 ) / 12L ) / 100L ) ) / 4L - 32075L;

    }
  else  // Julian:
    {
      JD = 367L * Y - ( 7L * ( Y + 5001L + ( M - 9L ) / 7L )) / 4L
           + ( 275L * M ) / 9L + D + 1729777L;
    }

  return ( JD );
} // END - gregd2jul ()

void jul2date ( long int jd )
  {
    long int l, n, j, k, i;

// This algorithm is from photcopied notes, from James Neely.
// Also, checked with ESAA, 1992, p. 604
// Calculates date, at noon on which the Julian date starts.
// Julian 0 starts Greenwich mean noon on 1st Jan 4713 BC, Julian proleptic
// calendar.
// In change from Julian to Gregorian calendars, in 1582, Oct 4th was followed
// by Oct 15th

// First, get day of week:

  doweek = jd - 7L * (( jd + 1 ) / 7L ) + 2L;
  if ( doweek == 7L )
    doweek = 0L;
  if ( doweek > 7L )
    {
      printf ( "ERROR IN DAY OF WEEK ROUTINE:\n" );
//      getch ();
      getch();
    }
  if ( jd >= 2299161L )  // Gregorian calendar:
    { // This has been tested between March 1, 1600 and Jan 31, 2100
    l = jd + 68569L;
    n = ( 4L * l ) / 146097L;
    l = l - ( 146097L * n + 3L ) / 4L;

//  wy = 4000L * ( l + 1L ) / 1461001L;

    l2bcd ( bcda, 4000L );
    mulbcdl ( bcda, bcda, l + 1L );
    divbcdl ( bcda, bcda, 1461001L );
    wy = bcd2l ( bcda );

    l = l - ( 1461L * wy ) / 4L + 31L;
    wm = ( 80L * l ) / 2447L;
    wd = l - ( 2447L * wm ) / 80L;
    l = wm / 11L;
    wm = wm + 2L - 12L * l;
    wy = 100L * ( n - 49L ) + wy + l;
// j = month, k = day, i = year
    }
  else // Julian calendar
    {
      j = jd + 1402L;
      k = ( j - 1L ) / 1461L;
      l = j - 1461L * k;
      n = ( l - 1L ) / 365L - l / 1461L;
      i = l - 365L * n + 30L;
      j = ( 80L * i ) / 2447L;
      wd = i - ( 2447L * j ) / 80L;
      i = j / 11L;
      wm = j + 2L - 12L * i;
      wy = 4L * k + n + i - 4716L;
    }
  } // END - jul2date ()

// Function to calculate month mean Sun, "nyi ma'i dhru ba".
void nyi_dru ( long int x )
  {
    mul_gen ( nyidru, nyidm, x, 27L, sun_f );
    add_gen ( nyidru, nyidru, nyida, 27L, sun_f );
  } // END - nyi_dru ()

// Function to calculate month mean weekday, "gza'i dhru ba".

void gza_dru ( long int x )
  {
    mul_gen ( gzadru, gzadm, x, 7L, gza_f );
    add_gen ( gzadru, gzadru, gzada, 7L, gza_f );
  } // END - gza_dru ()

// Function to calculate lunar day solar longitude, "nyi ma'i longs spyod".
void nyi_lon ( long int x )
  {
    mul_gen ( nyilon, nyilm, x, 27L, sun_f );
  } // END - nyi_lon ()

// Function to calculate lunar day mean weekday, "tshes kyi dhru ba".
void tse_dru ( long int x )
  {
    mul_gen ( tsedru, tsedm, x, 7L, gza_f );
  } // END - tse_dru ()

// Function to calculate true solar longitude, "nyi dag".
void nyi_dag ( long int *a1 )
  {
    long int test, tquo, trem;

    sub_gen ( nyiwor, a1, nyifac, 27L, sun_f ); // KTC 31
    test = 60L * nyiwor[0] + nyiwor[1];
    if ( test < 810L )
      nyidor = 0;
    else
      {
        nyidor = 1;
        sub_gen ( nyiwor, nyiwor, nyihaf, 27L, sun_f);
        test = 60L * nyiwor[0] + nyiwor[1];
      }
    trem = test % 135;
    tquo = test / 135;
    if ( tquo == 0 )
      tquo = 6;

    clear_a_b ();

    lista[2] = ( trem * 60L + nyiwor[2] ) * nyibye[(int)tquo - 1];
    lista[3] = nyiwor[3] * nyibye[(int)tquo - 1];
    lista[4] = nyiwor[4] * nyibye[(int)tquo - 1];

    div_g6 ( lista, 135L, sun_f, 1L );

    clrlst (zerlst);
    add_gen ( lista, zerlst, lista, 27L, sun_f );

    listb[1] = nyidom[(int)tquo - 1];
    if ( tquo == 3L || tquo == 4L || tquo == 5L ) // Then, subtract:
      sub_gen ( listc, listb, lista, 27L, sun_f );
    else
      add_gen ( listc, listb, lista, 27L, sun_f );

    if ( nyidor == 0 )
      sub_gen ( nyidag, a1, listc, 27L, sun_f );
    else
      add_gen ( nyidag, a1, listc, 27L, sun_f );
  } // END - nyi_dag ()

// Function to calculate true weekday, "gza' dag".
void gza_dag ( long int *a1 )
  {
    long int tot, trem, chasha, rilpo;
    long int gzawor[5];

    clrlst (list1);
    clrlst (list2);
    clrlst (zerlst);
    rilpo = rilcha[0] + tt;
    trem = rilpo % 14;
    if ( trem == 0 )
      trem = 14;
    list2[1] = gzadom[(int)trem - 1];
    gza_short_flg = 0; // KTC 26
    if ( gza_short_flg ) // Flag for less accurate weekday adjustment
      {
        chasha = rilcha[1] * gzabye[(int)trem - 1L] * 254520L; //60*6*707
        // Maximum value = 125 * 5 * 254520 = 159075000 - OK
        tot = chasha / 126L;
      }
    else
      {
        l2bcd ( bcda, ( 30L * rilcha[1] + tt ) * gzabye[(int)trem - 1] );
        mulbcdl ( bcda, bcda, 360L * gza_f );
        divbcdl ( bcda, bcda, 3780L );
        tot = bcd2l (bcda);
      }
    if ( tot < 0 ) // KTC 24
      {
        list1[4] = -tot;
        add_gen ( list1, list1, zerlst, 7L, gza_f );
        sub_gen ( list3, list2, list1, 7L, gza_f );
      }
    else
      {
        list1[4] = tot;
        add_gen ( list1, list1, zerlst, 7L, gza_f );
        add_gen ( list3, list2, list1, 7L, gza_f );
      }
    if (( rilpo / 14 ) % 2 == 0 )
      add_gen ( gzawor, a1, list3, 7L, gza_f);
    else
      sub_gen ( gzawor, a1, list3, 7L, gza_f);

    l2bcd ( bcda, gzawor[4] );
    mulbcdl ( bcda, bcda, sun_f );
    divbcdl ( bcda, bcda, gza_f );
    gzawor[4] = bcd2l ( bcda );

//  The above replaces:
//  gzawor[4] = ( sun_f * gzawor[4] ) / gza_f;

    if ( nyidor == 0 )
      sub_gen ( gzadag, gzawor, listc, 7L, sun_f );
    else
      add_gen ( gzadag, gzawor, listc, 7L, sun_f );

// Convert back the lowest fractional part:

    l2bcd ( bcda, gzadag[4] );
    mulbcdl ( bcda, bcda, gza_f );
    divbcdl ( bcda, bcda, sun_f );
    gzadag[4] = bcd2l ( bcda );
  } // END - gza_dag ()
