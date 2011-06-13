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

struct tib_ddat // Tibetan data for a solar day
  {
    int y; // Year, the Gregorian year that mostly matches the Tibetan year
    long jd; // Julian day for the solar day concerned, the target requested
    int w_d;
    int w_m;
    int w_y;
    long juldat; // Julian day determined. Will be different if duplicated lunar day
    int rab_jung; // rabjung, 60 year cycle. 1 refers to 1027 to 1086
    int year_val; // year within the rab_jung cycle. 1027 would be 0
    int year_animal;
    int year_element;
    int year_gender; // 1-female, 0-male
    int year_sme_ba;
    int hor_month;
    int intercal;
    int delaymth;
    int mthanim;
    int mthelem;
    int mthgender;
    int tt;
    int prv_chad;
    int next_chad;
    int lhag;
    int lunman;
    int sbyorba;    
    int byedpax;
    long gd[6];
    long nd[6];
    long ml[6];
    long bl[6];
    long ZS_s;
    long ZS_d;
    long ZS_m;
    int ld_parx;
    int ld_smex;
    int ld_animx;
  };

// T1.C:

void cal_cyc ( void );
void new_yer ( void );

// T2.C:

void get_tpdata ( long, struct tib_ddat * );
void set_grub ( void );
void set_epoch ( void );
void do_rahu ( long int, long int );
void zla_dag ( long int, long int );
void adj_zla ( void );
void rilchaf ( long int );
void spi_zagf ( void );
long int gregd2jul ( long int, long int, long int ); // MOVE ??
void jul2date ( long int ); // MOVE ??
void nyi_dru ( long int );
void gza_dru ( long int );
void nyi_lon ( long int );
void tse_dru ( long int );
void nyi_dag ( long int * );
void gza_dag ( long int * );

// T3.C - Planet routines.

void calc_dalbar ( long int *, long int, long int, long int );
void do_daldag ( long int *, long int *, long int *, long int *,
                 long int *, long int );
void do_plans ( long int );
void marsdag ( void );
void jupdag ( void );
void satdag ( void );
void merdag ( void ); // Mercury myur dag
void vendag ( void ); // Venus myur dag

// T4.C: Routines for printing calendars:

void prn_cal ( void );
int chk_spec ( long int, long int );
int chk_sadag ( long int, long int );
void s_cyc ( void );


// UTILS.C:

void prn_lst6 ( long int * );
void prn_lst7 ( long int * );
void prn_lstd ( long int *, long int, long int );
void mul_g6 ( long int *, long int, long int, long int );
void mul_g7 ( long int *, long int, long int, long int, long int );
void div_g6 ( long int *, long int, long int, long int );
void sub_g6 ( long int *, long int *, long int *, long int, long int, long int
                       );
void add_g6 ( long int *, long int *, long int *, long int, long int, long int
                       );
void add_g7 ( long int *, long int *, long int *, long int, long int, long int,
              long int );
void mul_gen ( long int *, long int *, long int, long int, long int );
long int exp_lst ( long int *, long int ); 
double conv2degs ( long int *, long int, long int );
double solar_long ( long int, double );
double conv2degs1 ( long int *, long int, long int );
void add_gen ( long int *, long int *, long int *, long int, long int );
void sub_gen ( long int *, long int *, long int *, long int, long int );
void prn_lst ( long int * );
void clrlst ( long int * );
void clrlst6 ( long int * );
void clear_a_b ( void );
void testbcd ( void );

void UTET ( int, int, int, int, int, int, double *, double * );
double AINT ( double );
double get_deltaT ( double );
double modfeh ( double, double );

void cls ( void );
int prfldat ( int, int, int, unsigned char * );
void printat ( int, int, char * );
void start_print ( void );
void prin_str ( char * );
void addstrings ( char *, char * );

// TESTS.C:

void set_errfl ( void );
void wrt_errfl ( void );
void clos_errfl ( void );
void srch_cyc ( void );

