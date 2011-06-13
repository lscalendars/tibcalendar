/* TCN.C */

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
#include "tcn.h"
#include "tc.def"

FILE *repfil;
struct tib_ddat td;

extern int num_next_omit;
extern int num_prev_omit;

void chk_bcd ( void );

void cal_cyc ( void ) // Main routine
  {
    int more;
    long cur_jd;
    long dd, mm, yy;
    char chr;
    restart: cls ();
    printf ( "%s\n", e_str[epch] );
    printf ("Enter the starting date, month and year (dd mm yyyy): " );
    scanf ("%ld %ld %ld", &dd, &mm, &yy);
    printf ("\n");
// Convert to Julian:
    cur_jd = gregd2jul ( dd, mm, yy );
    sprintf ( outbuf, "Target western date:   %ld:%ld:%ld, Julian: %ld", dd, mm, yy, cur_jd );
    printf ( "%s\n", outbuf );
    fprintf ( fptgt, "%s\n", outbuf );                       
    more = 1;
    while (more)
      {
        printf ("\n\n");      

               
        get_tpdata ( cur_jd, &td );

        sprintf ( outbuf, "Returned western date: %ld:%ld:%ld, Julian: %ld", td.w_d, td.w_m, 
                  td.w_y, td.juldat );
        printf ( "%s\n", outbuf );

        fprintf ( fptgt, "%s\n", outbuf );                       
        sprintf ( outbuf, "Rabjung: %d, Year: %d, %s-%s-%s, sme-ba: %d", 
                  td.rab_jung, td.year_val, cycelem1[td.year_element], cycgendE[td.year_gender],
                  cycanim[td.year_animal], td.year_sme_ba );
        printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );       
        
// Month data:        
        if ( td.intercal ) 
          sprintf ( outbuf, "Month: %d, Intercalary, %s-%s-%s", td.hor_month, cycelem1[td.mthelem], 
                    cycgendE[td.mthgender], cycanim[td.mthanim] );
        else if ( td.delaymth ) 
          sprintf ( outbuf, "Month: %d, Delayed, %s-%s-%s", td.hor_month, cycelem1[td.mthelem], 
                    cycgendE[td.mthgender], cycanim[td.mthanim] );        
        else
          sprintf ( outbuf, "Month: %d, %s-%s-%s", td.hor_month, cycelem1[td.mthelem], 
                    cycgendE[td.mthgender], cycanim[td.mthanim] );
        printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );                
        
// Day data:        

        if ( td.lhag == 1 )        
          sprintf ( outbuf, "Lunar date: %d, First of duplicated", td.tt );
        else if ( td.lhag == 2 )                
          sprintf ( outbuf, "Lunar date: %d, Second of duplicated", td.tt );        
        else if ( td.prv_chad )                
          sprintf ( outbuf, "Lunar date: %d, Previous, %d, omitted", td.tt, td.prv_chad );        
        else if ( td.next_chad )                
          sprintf ( outbuf, "Lunar date: %d, Next, %d, omitted", td.tt, td.next_chad );        
        else 
          sprintf ( outbuf, "Lunar date: %d", td.tt );        
        printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );        

        sprintf ( outbuf,
          "Lunar day: %ld;%ld,%ld Moon: %ld;%ld,%ld Sun: %ld;%ld,%ld Yoga: %ld;%ld,%ld Sign: %ld;%ld,%ld",
                   td.gd[0], td.gd[1], td.gd[2], 
                   td.ml[0], td.ml[1], td.ml[2],
                   td.nd[0], td.nd[1], td.nd[2],
                   td.bl[0], td.bl[1], td.bl[2],
                   td.ZS_s, td.ZS_d, td.ZS_m );

        printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );        
        
        sprintf ( outbuf, "%s, %s, %s, %s %ld", yogaT[td.sbyorba], byedT[td.byedpax],
                  cycanim[td.ld_animx], cycpar[td.ld_parx], td.ld_smex );        
        
        printf ( "%s\n", outbuf );
        fprintf ( fptgt, "%s\n", outbuf );                
                      

        printf ( "\nRestart, Next, Exit\n\n" );
        
        chr = getch ();
        if ( chr == 'R' || chr == 'r' )
          goto restart;
        else if ( chr == 'N' || chr == 'n' )
          {
            ++cur_jd;
          }
        else if ( chr == 'E' || chr == 'e' )          
          more = 0;
      }
  } // END - cal_cyc ()

void main (void)
  {
    int    i, finish, xit;
    long x;
    char   chr;
    set_epoch ();  // T2.C
    finish = 0;
    fptgt = fopen ( "result.txt", "w" );
    if ( fptgt == NULL )
      {
        printf ( "Error opening target file!\n" );
        getch ();
      }                
    while ( finish == 0 )
      {
        cls ();
        printat (1, 22, "TIBETAN CALENDAR TEST SOFTWARE\n\n");
        printat (2, 31, "      Version 0.1\n\n");
        printat (5, 20, "Use numeric keys to select from the following:\n\n");
        printat (11, 30, "   1. \n");
        printat (13, 30, "   2. \n");
        printat (15, 30, "   3. Convert date.\n");
        printat (17, 30, "   4. Run test.\n");
        printat (19, 30, "   5. \n");
        printat (21, 30, "   9. Finish.\n");
        xit = 0;

        do
          {
            chr = getch ();
            switch ( chr )
              {
                case '1':       
                  set_epoch ();
                  xit = 1;
                  break;
                case '3':       
                  cal_cyc ();  // Main calendar cycle
                  xit = 1;
                  break;
                case '4':     // Run test routine
                  // For testing:
//                x = 2455500L;
//                x = 1356200L;
//		  x = 2000487L; for 10 - catches problem with duplicated followed by omit
                  x = 2096269L;
                  for ( i = 0; i < 300; ++i )
                    {
                      get_tpdata ( x, &td );
                      x = x + 1L;
                    }
                  sprintf ( outbuf, "Omitteds: Previous: %d, next: %d", 
                            num_prev_omit, num_next_omit );
                  printf ( "%s\n", outbuf );
                  fprintf ( fptgt, "%s\n", outbuf );  
                  getch ();
                  xit = 1;
                  break;
                case '9':      // FINISH
                  finish = 1;
                  xit = 1;
                  break;
                default:
                  ;
                  break;
              }
          } while ( xit == 0 );
      }
    chk_bcd ();
  } // END - main ()

