/* T3.C */

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
#include "tbplanets.h"
#include "utils.h"
#include "tbday.h"

static long int merbye1[14] =
              {16, 15, 14, 13, 11, 7, 5, 0, 4, 11, 20, 28, 34, 16};
static long int merbye2[14] =
              {28, 20, 11, 4, 0, 5, 7, 11, 13, 14, 15, 16, 16, 34};
static long int merdom1[14] =
              {16, 32, 47, 61, 74, 85, 92, 97, 97, 93, 82, 62, 34, 0};
static long int merdom2[14] =
              {34, 62, 82, 93, 97, 97, 92, 85, 74, 61, 47, 32, 16, 0};
static long int venbye1[14] =
              {25, 25, 24, 24, 22, 22, 18, 15, 8, 6, 30, 99, 73, 25};
static long int venbye2[14] =
              {99, 30, 6, 8, 15, 18, 22, 22, 24, 24, 25, 25, 25, 73};
static long int vendom1[14] =
              {25, 50, 75, 99, 123, 145, 167, 185, 200, 208, 202, 172, 73, 0};
static long int vendom2[14] =
              {73, 172, 202, 208, 200, 185, 167, 145, 123, 99, 75, 50, 25, 0};
static long int marbye1[14] =
              {23, 23, 23, 21, 21, 18, 15, 11, 3, 11, 38, 80, 53, 24};
static long int marbye2[14] =
              {80, 38, 11, 3, 11, 15, 18, 21, 21, 23, 23, 23, 24, 53};
static long int mardom1[14] =
              {24, 47, 70, 93, 114, 135, 153, 168, 179, 182, 171, 133, 53, 0};
static long int mardom2[14] =
              {53, 133, 171, 182, 179, 168, 153, 135, 114, 93, 70, 47, 24, 0};
static long int jupbye1[14] =
              {10, 9, 8, 6, 6, 2, 1, 3, 6, 9, 11, 16, 7, 10};
static long int jupbye2[14] =
              {16, 11, 9, 6, 3, 1, 2, 6, 6, 8, 9, 10, 10, 7};
static long int jupdom1[14] =
              {10, 20, 29, 37, 43, 49, 51, 52, 49, 43, 34, 23, 7, 0};
static long int jupdom2[14] =
              {7, 23, 34, 43, 49, 52, 51, 49, 43, 37, 29, 20, 10, 0};
static long int satbye1[14] =
              {5, 5, 4, 4, 2, 2, 0, 2, 4, 5, 6, 8, 3, 6};
static long int satbye2[14] =
              {8, 6, 5, 4, 2, 0, 2, 2, 4, 4, 5, 5, 6, 3};
static long int satdom1[14] =
              {6, 11, 16, 20, 24, 26, 28, 28, 26, 22, 17, 11, 3, 0};
static long int satdom2[14] =
              {3, 11, 17, 22, 26, 28, 28, 26, 24, 20, 16, 11, 6, 0};

static long int plahaf[6] = {13,30,0,0,0,0};
static long int merfac[5] = {16, 30, 0, 0, 0};
static long int merbye[6] = {7,3,3,7,10,10};
static long int merdom[6] = {10,17,20,17,10,0};
static long int venfac[5] = {6, 0, 0, 0, 0};
static long int venbye[6] = {4,1,1,4,5,5};
static long int vendom[6] = {5,9,10,9,5,0};
static long int marfac[5] = {9, 30, 0, 0, 0};
static long int marbye[6] = {18,7,7,18,25,25};
static long int mardom[6] = {25,43,50,43,25,0};
static long int jupfac[5] = {12, 0, 0, 0, 0};
static long int jupbye[6] = {9,3,3,9,11,11};
static long int jupdom[6] = {11,20,23,20,11,0};
static long int satfac[5] = {18, 0, 0, 0, 0};
static long int satbye[6] = {15,6,6,15,22,22};
static long int satdom[6] = {22,37,43,37,22,0};


static long int rkang_frac = 149209L;

// Periods of the planets, solar days

static long int mercyc = 8797; 
static long int vencyc = 2247;
static long int marcyc = 687;
static long int jupcyc = 4332;
static long int satcyc = 10766;

// least significant radix:

static long int merfrac = 8797;   
static long int venfrac = 749;    
static long int marfrac = 229;    
static long int jupfrac = 361;
static long int satfrac = 5383;

// ROUTINE TO CALCULATE POSITIONS OF RAHU
void
calc_rahup (epoch epch, long int m, long int tt, long int rahudong[5])	// KTC 96
{
  long int t;
  long int rahutsa[5] = { 0, 0, 14, 0, 12 };
  long int lista[5] = { 0, 0, 0, 0, 0 };
  // ( m + epch.rahupart ) % 230 is the rahu month
  t = ((m + epch.rahupart) % 230) * 30 + tt;
  mul_lst (rahutsa, rahutsa, t, 27, 23);
  lista[0] = 27;
  sub_lst (rahudong, lista, rahutsa, 27, 23);
  //lista[0] = 13;
  //lista[1] = 30;
  //add_gen ( rahujug, rahudong, listb, 27, 23 ); // not used...
}				// END - calc_rahup ()

/* Function to compute the mean longitude of a planet
 * lst is the result
 * dat is the particular day (KTC p. 57)
 * cyc is the period of the planet
 * frac is the lowest fractional part (KTC p.58)
 */
void
calc_dalbar (long int lst[5], long int dat, long int cyc, long int frac)
{
  lst[0] = dat * 27;
  lst[1] = 0;
  lst[2] = 0;
  lst[3] = 0;
  lst[4] = 0;
  lst[5] = 0;
  div_lst_6 (lst, lst, cyc, frac, 1);
}

void
do_daldag (long int pdaldag[], long int pdalbar[], long int pfac[],
	   long int pbye[6], long int pdom[6], long int frac)
{
  long int dortst, test, trem, tquo;
  long int lista[6] = { 0, 0, 0, 0, 0, 0 };
  long int listb[6] = { 0, 0, 0, 0, 0, 0 };
  long int listc[6] = { 0, 0, 0, 0, 0, 0 };
  sub_lst (lista, pdalbar, pfac, 27, frac);
  test = lista[0] * 60 + lista[1];
  if (test >= 13 * 60 + 30)
    {
      dortst = 1;		// Subtract half-circle - KTC 59
      test = test - 13 * 60 - 30;
    }
  else
    dortst = 0;			// No subtraction of half-circle
  trem = test % 135;
  tquo = test / 135;
  if (tquo == 0)
    tquo = 6;
  lista[0] = 0;
  lista[1] = trem;
  lista[2] = pdalbar[2];
  lista[3] = pdalbar[3];
  lista[4] = pdalbar[4];
  lista[5] = 0;

  mul_lst_6 (lista, lista, pbye[(int) tquo - 1], frac, 1);
  div_lst_6 (lista, lista, 135, frac, 1);

  listb[1] = pdom[(int) tquo - 1];

// tquo is equal to actual index, except 0, replaced by 6

  if (tquo == 3 || tquo == 4 || tquo == 5)	// Then, subtract:
    sub_lst (listc, listb, lista, 27, frac);
  else
    add_lst (listc, listb, lista, 27, frac);

  if (dortst == 0)
    sub_lst (pdaldag, pdalbar, listc, 27, frac);
  else
    add_lst (pdaldag, pdalbar, listc, 27, frac);
}

#define __DEBUG 1
#if __DEBUG == 1
#include<stdio.h>		// remove after debug"
void
dbg_print_lst (long int l[6])
{
  unsigned char i;
  printf ("%ld;", l[0]);
  for (i = 1; i < 5; i++)
    {
      printf ("%ld,", l[i]);
    }
  printf ("%ld", l[5]);
}
#endif

/* Main function computing all the data for a tib_day
 */
void
get_planets_data (tib_day *td, epoch epch)
{
  int i;
  long int sz = td->gd - epch.spz_j; //TODO: OK
  long int mardalbar[6] = {0,0,0,0,0,0};
  long int mardaldag[6] = {0,0,0,0,0,0};
  long int jupdalbar[6] = {0,0,0,0,0,0};
  long int jupdaldag[6] = {0,0,0,0,0,0};
  long int satdalbar[6] = {0,0,0,0,0,0};
  long int satdaldag[6] = {0,0,0,0,0,0};
  long int merkanbar[6] = {0,0,0,0,0,0};
  long int venkanbar[6] = {0,0,0,0,0,0};
  long int merdaldag[6] = {0,0,0,0,0,0};
  long int vendaldag[6] = {0,0,0,0,0,0};
  long int dragkang[6] = {0,0,0,0,0,0};
  long int nyindhru[6] = {0,0,0,0,0,0};
  long long int dragkres; // TODO: remove after debug

// MARS, DAL DAG. - KTC 57
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 623
  // here (sz + epch.maradd) % 687 is the particular day (KTC p. 57)
  calc_dalbar (mardalbar, (sz + epch.maradd) % marcyc, marcyc, marfrac);
  //martl = conv2degs ( mardalbar, 229, 1 );
  do_daldag (mardaldag, mardalbar, marfac, marbye, mardom, marfrac);

// JUPITER, DAL DAG. - KTC 61
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 625
  calc_dalbar (jupdalbar, (sz + epch.jupadd) % jupcyc, jupcyc, jupfrac);
  //juptl = conv2degs ( jupdalbar, 361, 1 );
  do_daldag (jupdaldag, jupdalbar, jupfac, jupbye, jupdom, jupfrac);

// SATURN, DAL DAG. - KTC 61
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 633
  calc_dalbar (satdalbar, (sz + epch.satadd) % satcyc, satcyc, satfrac);
  //sattl = conv2degs ( satdalbar, 5383, 1 );
  do_daldag (satdaldag, satdalbar, satfac, satbye, satdom, satfrac);

// MERCURY, RKANG BAR, - KTC 85
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 610

  calc_dalbar (merkanbar, (sz*100 + epch.meradd) % mercyc, mercyc, merfrac);
  //mertl = conv2degs ( merkanbar, 8797, 1 );

// VENUS, RKANG BAR, - KTC 77
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 612
  calc_dalbar (venkanbar, (sz*10 + epch.venadd) % vencyc, vencyc, venfrac);
  //ventl = conv2degs ( venkanbar, 749, 1 );

// DRAG GSUM RKANG 'DZIN, ZHI GNYIS DAL BAR, KTC 63

//   dragkres = ((long long) sz * 18382 + (long long) epch.dragkadd) % 6714405LL;

 // printf("dragkres: %lld\n", dragkres);
  dragkang[0] = 27 * (long) (((long long) sz * 18382 + (long long) epch.dragkadd) % 6714405LL);
  dragkang[1] = 0;
  dragkang[2] = 0;
  dragkang[3] = 0;
  dragkang[4] = 0;
  dragkang[5] = 0;

  div_lst_6 (dragkang, dragkang, 6714405L, rkang_frac, 1);

  copy_lst_6(nyindhru, dragkang); // nyindhru ok

// For Tsurphu system only:
  /* TODO: handle...
     if ( epch == TSURPHU )  // For Tsurphu, use normal mean Sun
     {
     rkang_frac = 13;       
     for ( i = 0; i < 5; ++i )
     nyindhru[i] = nyibar[i];
     nyindhru[5] = 0;  
     nyindhru[4] = (long) (( (long long)nyindhru[4] * 149209L ) / 67);         
     }
   */

// MERCURY DAL DAG. KTC 85
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 616

  do_daldag (merdaldag, nyindhru, merfac, merbye, merdom, rkang_frac);

// VENUS DAL DAG. KTC 77/78
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 616

  do_daldag (vendaldag, nyindhru, venfac, venbye, vendom, rkang_frac);

  // then the main computation, setting td->xxxmurdag
  marsdag (mardaldag, dragkang, td->marmurdag);	// KTC 65
  jupdag (jupdaldag, dragkang, td->jupmurdag);	// KTC 74
  satdag (satdaldag, dragkang, td->satmurdag);	// KTC 75
  merdag (merdaldag, merkanbar, td->mermurdag);	// KTC 86    
  vendag (vendaldag, venkanbar, td->venmurdag);	// KTC 82
  // month or adjusted month?
  calc_rahup (epch, td->month->true_month[0], td->tt, td->rahudong);
}				// END - do_plans ()

// IIUC, mardaldag is what we have and marmurdag is what we want
void
marsdag (long int mardaldag[6], long int dragkang[6], long int marmurdag[6])
{
  long int lista[6], listc[6];
  long int listb[6] = {0,0,0,0,0,0}; // listb is the only one that has to be initialized
  long int test, tquo, trem, x;
  long long int z;
  int dortst, conttest, i;
  copy_lst_6(lista, dragkang);

// Adjust the units for later combination:

  z = ((long long) lista[4] * (long long) marfrac);
  lista[4] = (long) (z / (long long) rkang_frac);
  lista[5] = (long) (z % (long long) rkang_frac);

  //clear_a_b ();
  sub_lst_6 (lista, lista, mardaldag, 27, marfrac, rkang_frac);
  
  test = lista[0] * 60 + lista[1];
  // 13 * 60 + 30 = half a circle
  if (test >= 13 * 60 + 30)
    {
      dortst = 1;		// Subtract half-circle
      sub_lst (lista, lista, plahaf, 27, rkang_frac);
    }
  else
    dortst = 0;			// No subtraction of half-circle

  tquo = lista[0];
  trem = lista[1];
// Checked against calculation tables in British Library and:
// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 644

  if (!dortst)			//  = ma dor, rim pa, lugs 'byung.
    {
      if (tquo == 13)
	conttest = 1;
      else
	conttest = 0;

      if (tquo == 0)
	tquo = 14;

      x = marbye1[(int) tquo - 1];	// Multiplier, looking down
      lista[0] = 0;
      mul_lst_6 (lista, lista, x, marfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, marfrac, rkang_frac);

// Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, marfrac, rkang_frac);
      listb[1] = mardom1[(int) tquo - 1];	// Totals, left hand list.
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
      //listb[5] = 0; TODO: should be useless

      // tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)
	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, marfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, marfrac, rkang_frac);
    }
  else				// IF DORTST, rim min, lugs ldog.
    {
      conttest = 0;
      if (lista[0] == 0)
	{
	  if (lista[1] < 30)
	    conttest = 1;
	}
      if (lista[1] >= 30)
	{
	  ++tquo;
	  trem = lista[1] - 30;
	}
      else if (lista[0] != 0)
	trem = lista[1] + 30;

      if (tquo == 0)
	tquo = 14;

      x = (long int) marbye2[(int) tquo - 1];
      lista[0] = 0;
      lista[1] = trem;
      mul_lst_6 (lista, lista, x, marfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, marfrac, rkang_frac);

// Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, marfrac, rkang_frac);
      listb[1] = mardom2[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;

      // tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 4 || tquo == 5 || tquo == 6 || tquo == 7 || tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13 /** || tquo == 14 */ )	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, marfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, marfrac, rkang_frac);
    }
  if (dortst == 1)
    sub_lst_6 (marmurdag, mardaldag, listc, 27, marfrac, rkang_frac);
  else
    add_lst_6 (marmurdag, mardaldag, listc, 27, marfrac, rkang_frac);
}				// END - marsdag ()

void
jupdag (long int jupdaldag[6], long int dragkang[6], long int jupmurdag[6])
{
  long int lista[6], listc[6];
  long int listb[6] = {0,0,0,0,0,0};
  long int test, tquo, trem, x;
  long long int z;
  int dortst, conttest, i;
  copy_lst_6(lista, dragkang);

// Adjust the units for later combination: 

  z = ((long long) lista[4] * (long long) jupfrac);
  lista[4] = (long) (z / (long long) rkang_frac);
  lista[5] = (long) (z % (long long) rkang_frac);

  sub_lst_6 (lista, lista, jupdaldag, 27, jupfrac, rkang_frac);

  test = lista[0] * 60 + lista[1];
  if (test >= 13L * 60 + 30)
    {
      dortst = 1;
      sub_lst (lista, lista, plahaf, 27, rkang_frac);
    }
  else
    dortst = 0;

  tquo = lista[0];
  trem = lista[1];

// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 645
  if (!dortst)			//  = ma dor, rim pa, lugs 'byung.
    {
      if (tquo == 13)
	conttest = 1;
      else
	conttest = 0;
      if (tquo == 0)
	tquo = 14;
      x = jupbye1[(int) tquo - 1];
      lista[0] = 0;
      mul_lst_6 (lista, lista, x, jupfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, jupfrac, rkang_frac);

// Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, jupfrac, rkang_frac);

      listb[1] = jupdom1[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
      listb[5] = 0;

// tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, jupfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, jupfrac, rkang_frac);
    }
  else				// If DORTST, rim min, lugs ldog.
    {
      conttest = 0;
      if (lista[0] == 0)
	{
	  if (lista[1] < 30)
	    conttest = 1;
	}
      if (lista[1] >= 30)
	{
	  ++tquo;
	  trem = lista[1] - 30;
	}
      else if (lista[0] != 0)
	trem = lista[1] + 30;
      if (tquo == 0)
	tquo = 14;

      x = jupbye2[(int) tquo - 1];
      lista[0] = 0;
      lista[1] = trem;
      mul_lst_6 (lista, lista, x, jupfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, jupfrac, rkang_frac);

// Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, jupfrac, rkang_frac);
      listb[1] = jupdom2[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;

// tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 6 || tquo == 7 || tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, jupfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, jupfrac, rkang_frac);

    }
  if (dortst == 1)
    sub_lst_6 (jupmurdag, jupdaldag, listc, 27, jupfrac, rkang_frac);
  else
    add_lst_6 (jupmurdag, jupdaldag, listc, 27, jupfrac, rkang_frac);
}				// END - jupdag ()

void
satdag (long int satdaldag[6], long int dragkang[6], long int satmurdag[6])
{
  long int lista[6], listc[6];
  long int listb[6] = {0,0,0,0,0,0};
  long int test, tquo, trem, x;
  long long int z;
  int dortst, conttest, i;
  copy_lst_6(lista, dragkang);

// Adjust the units for later combination: 

  z = ((long long) lista[4] * (long long) satfrac);
  lista[4] = (long) (z / (long long) rkang_frac);
  lista[5] = (long) (z % (long long) rkang_frac);

  sub_lst_6 (lista, lista, satdaldag, 27, satfrac, rkang_frac);

  test = lista[0] * 60 + lista[1];
  if (test >= 13 * 60 + 30)
    {
      dortst = 1;
      sub_lst (lista, lista, plahaf, 27, rkang_frac);
    }
  else
    dortst = 0;

  tquo = lista[0];
  trem = lista[1];

// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 647
  if (!dortst)			//  = ma dor, rim pa, lugs 'byung.
    {
      if (tquo == 13)
	conttest = 1;
      else
	conttest = 0;
      if (tquo == 0)
	tquo = 14;
      x = satbye1[(int) tquo - 1];
      lista[0] = 0;
      mul_lst_6 (lista, lista, x, satfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, satfrac, rkang_frac);

// Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, satfrac, rkang_frac);

      listb[1] = satdom1[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
      listb[5] = 0;

      // tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:

	sub_lst_6 (listc, listb, lista, 27, satfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, satfrac, rkang_frac);
    }
  else				// If DORTST, rim min, lugs ldog.
    {
      conttest = 0;
      if (lista[0] == 0)
	{
	  if (lista[1] < 30)
	    conttest = 1;
	}
      if (lista[1] >= 30)
	{
	  ++tquo;
	  trem = lista[1] - 30;
	}
      else if (lista[0] != 0)
	trem = lista[1] + 30;
      if (tquo == 0)
	tquo = 14;

      x = satbye2[(int) tquo - 1];
      lista[0] = 0;
      lista[1] = trem;
      mul_lst_6 (lista, lista, x, satfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, satfrac, rkang_frac);

// Now divide all the way through by 60:
      div_lst_6 (lista, lista, 60, satfrac, rkang_frac);

      listb[1] = satdom2[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
// tquo is equal to actual index, except 0, replaced by 14
      if (tquo == 6 || tquo == 7 || tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, satfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, satfrac, rkang_frac);
    }
  if (dortst == 1)
    sub_lst_6 (satmurdag, satdaldag, listc, 27, satfrac, rkang_frac);
  else
    add_lst_6 (satmurdag, satdaldag, listc, 27, satfrac, rkang_frac);
}				// END - satdag ()

void
merdag (long int merdaldag[6], long int merkanbar[6], long int mermurdag[6])	// Mercury myur dag
{
  long int lista[6], listc[6], dald[6];
  long int listb[6] = {0,0,0,0,0,0};
  long int test, tquo, trem, x;
  long long int z;
  int dortst, conttest, i;

  copy_lst_6(dald, merdaldag);

// Adjust the units for later combination: 

  z = ((long long) dald[4] * (long long) merfrac);
  dald[4] = (long) (z / (long long) rkang_frac);
  dald[5] = (long) (z % (long long) rkang_frac);

  //clear_a_b ();
  sub_lst_6 (lista, merkanbar, dald, 27, merfrac, rkang_frac);

  test = lista[0] * 60 + lista[1];
  if (test >= 13 * 60 + 30)
    {
      dortst = 1;
      sub_lst (lista, lista, plahaf, 27, rkang_frac);
    }
  else
    dortst = 0;
  tquo = lista[0];
  trem = lista[1];

// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 644
  if (!dortst)			//  = ma dor, rim pa, lugs 'byung.
    {
      if (tquo == 13)
	conttest = 1;
      else
	conttest = 0;
      if (tquo == 0)
	tquo = 14;
      x = merbye1[(int) tquo - 1];
      lista[0] = 0;
      mul_lst_6 (lista, lista, x, merfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, merfrac, rkang_frac);

// Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, merfrac, rkang_frac);
      listb[1] = merdom1[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
      listb[5] = 0;

      // tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, merfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, merfrac, rkang_frac);
    }
  else				// If DORTST, rim min, lugs ldog.
    {
      conttest = 0;
      if (lista[0] == 0)
	{
	  if (lista[1] < 30)
	    conttest = 1;
	}
      if (lista[1] >= 30)
	{
	  ++tquo;
	  trem = lista[1] - 30;
	}
      else if (lista[0] != 0)
	trem = lista[1] + 30;

      if (tquo == 0)
	tquo = 14;

      x = merbye2[(int) tquo - 1];
      //clear_lst_6 (lista);
      lista[0] = 0;
      lista[1] = trem;
      mul_lst_6 (lista, lista, x, merfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, merfrac, rkang_frac);
// Now divide all the way through by 60:
      div_lst_6 (lista, lista, 60, merfrac, rkang_frac);
      listb[1] = merdom2[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;

      // tquo is equal to actual index, except 0, replaced by 14

      if (tquo == 5 || tquo == 6 || tquo == 7 || tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, merfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, merfrac, rkang_frac);
    }

  if (dortst == 1)
    sub_lst_6 (mermurdag, dald, listc, 27, merfrac, rkang_frac);
  else
    add_lst_6 (mermurdag, dald, listc, 27, merfrac, rkang_frac);
}				// END - merdag ()

void
vendag (long int vendaldag[6], long int venkanbar[6], long int venmurdag[6])	// Venus myur dag
{
  long int lista[6], listc[6], dald[6];
  long int listb[6] = {0,0,0,0,0,0};
  long int test, tquo, trem, x;
  long long int z;
  int dortst, conttest, i;

  copy_lst_6(dald, vendaldag);

// Adjust the units for later combination: 

  z = ((long long) dald[4] * (long long) venfrac);
  dald[4] = (long) (z / (long long) rkang_frac);
  dald[5] = (long) (z % (long long) rkang_frac);

  //clear_a_b ();
  sub_lst_6 (lista, venkanbar, dald, 27, venfrac, rkang_frac);

  test = lista[0] * 60 + lista[1];
  if (test >= 13 * 60 + 30)
    {
      dortst = 1;
      sub_lst (lista, lista, plahaf, 27, rkang_frac);
    }
  else
    dortst = 0;
  tquo = lista[0];
  trem = lista[1];

// Checked against tables by "dbyangs can grub pa'i rdo rje", p. 645
  if (!dortst)			//  = ma dor, rim pa, lugs 'byung.
    {
      if (tquo == 13)
	conttest = 1;
      else
	conttest = 0;

      if (tquo == 0)
	tquo = 14;

      x = venbye1[(int) tquo - 1];
      lista[0] = 0;
      mul_lst_6 (lista, lista, x, venfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, venfrac, rkang_frac);

      // Now divide all the way through by 60:

      div_lst_6 (lista, lista, 60, venfrac, rkang_frac);

      listb[1] = vendom1[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
      listb[5] = 0;
      // tquo is equal to actual index, except 0, replaced by 14
      if (tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)
	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, venfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, venfrac, rkang_frac);
    }
  else				// If DORTST, rim min, lugs ldog.
    {
      conttest = 0;
      if (lista[0] == 0)
	{
	  if (lista[1] < 30)
	    conttest = 1;
	}
      if (lista[1] >= 30)
	{
	  ++tquo;
	  trem = lista[1] - 30;
	}
      else if (lista[0] != 0)
	trem = lista[1] + 30;
      if (tquo == 0)
	tquo = 14;
      x = venbye2[(int) tquo - 1];
      //clear_lst_6 (lista);
      lista[0] = 0;
      lista[1] = trem;
      mul_lst_6 (lista, lista, x, venfrac, rkang_frac);
      if (conttest)
	mul_lst_6 (lista, lista, 2, venfrac, rkang_frac);
// Now divide all the way through by 60:
      div_lst_6 (lista, lista, 60, venfrac, rkang_frac);
      listb[1] = vendom2[(int) tquo - 1];
      listb[0] = listb[1] / 60;
      listb[1] = listb[1] % 60;
// tquo is equal to actual index, except 0, replaced by 14
      if (tquo == 4 || tquo == 5 || tquo == 6 || tquo == 7 || tquo == 8 || tquo == 9 || tquo == 10 || tquo == 11 || tquo == 12 || tquo == 13)	// Then, subtract:
	sub_lst_6 (listc, listb, lista, 27, venfrac, rkang_frac);
      else
	add_lst_6 (listc, listb, lista, 27, venfrac, rkang_frac);
    }
  if (dortst == 1)
    sub_lst_6 (venmurdag, dald, listc, 27, venfrac, rkang_frac);
  else
    add_lst_6 (venmurdag, dald, listc, 27, venfrac, rkang_frac);
}				// END - vendag ()
