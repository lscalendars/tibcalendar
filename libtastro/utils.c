/*********************************************************************************
License for Tibastro - Tibetan astronomical and astrological calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

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

#include<stdio.h>
#include"utils.h"
// TODO: use a normal gmp.h
//#include "mini-gmp.h"
#include <gmp.h>

// Functions to handle list calculations as done by tibetans, see KTC 18-19 for an example

/* here all lists are represented by an array of at least 5 integers (sometimes 6)
 * let's call them a0;a1,a2,a3,a4,a5
 * all the values are mod a certain value, which we call n0,n1,n2,n3,n4,n5
 * to take the notation of KTC: a0;a1,a2,a3,a4,a5 (n0,n1,n2,n3,n4,n5)
 * we consider n1,n2,n3 to be always 60,60,6, but n0,n4 and n5 can vary
 */

long int zerolst[5] = {0,0,0,0,0};

/*
 * function to add two lists together:
 * a1 = a2 + a3
 * n0,n4 and n5 can vary
 */

void add_lst_6 ( long int a1[6], long int a2[6], long int a3[6], long int n0,
              long int n4, long int n5 )
  {
    long int r;
    r = a2[5] + a3[5];
    if ( r < 0 )
      {
        printf ( "\n\nERROR IN ADD_G6:\n");
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX        
        printf ( "List A2: %ld;%ld,%ld,%ld,%ld,%ld\n", a2[0], a2[1], a2[2], a2[3], a2[4], a2[5] );
        printf ( "List A3: %ld;%ld,%ld,%ld,%ld,%ld\n", a3[0], a3[1], a3[2], a3[3], a3[4], a3[5] );
        printf ( "A2[5] = %ld\n", a2[5] );
        printf ( "A3[5] = %ld\n\n", a3[5] );
      }
    a1[5] = r % n5;
    r = a2[4] + a3[4] + r / n5;
    a1[4] = r % n4;
    r = a2[3] + a3[3] + r / n4;
    a1[3] = r % 6;
    r = a2[2] + a3[2] + r / 6;
    a1[2] = r % 60;
    r = a2[1] + a3[1] + r / 60;
    a1[1] = r % 60;
    r = a2[0] + a3[0] + r / 60;
    a1[0] = r % n0;
  }

/*
 * function to divise a list of 6:
 * a = a % x
 * n4 and n5 can vary
 */
void div_lst_6 ( long int res[6], long int lst[6], long int x, long int n4, long int n5 )
  {
    long long int r;
    long long int b; 
    
    r = ((long long) lst[0] % (long long) x) * 60;
    res[0] = lst[0] / x;
    
    b = ((long long) lst[1]) + r;
    res[1] = (long) (b / x);
    r = (b % (long long) x) * 60;
    
    b = ((long long) lst[2]) + r;
    res[2] = (long) (b / x);
    r = (b % (long long) x) * 6;
    
    b = ((long long) lst[3]) + r;
    res[3] = (long) (b / x);
    r = (b % (long long) x) * n4;
    
    b = (((long long) lst[4]) + r);
    res[4] = (long) (b / x);
    r = (b % (long long) x) * n5;
    
    b = ((long long) lst[5]) + r;
    res[5] = (long) (b / x);
}

/*
 * function to divise a list:
 * res = lst % x
 * n4 can vary
 */
void div_lst ( long int res[6], long int lst[6], long int x, long int n4 )
  {
    long int r;
    long int b; 
    
    r = ( lst[0] % x) * 60;
    res[0] = lst[0] / x;
    
    b = (lst[1]) + r;
    res[1] = (b / x);
    r = (b % x) * 60;
    
    b = ( lst[2]) + r;
    res[2] =  (b / x);
    r = (b %  x) * 6;
    
    b = ( lst[3]) + r;
    res[3] = (b / x);
    r = (b % x) * n4;
    
    b = lst[4] + r;
    res[4] = (b / x);
}

/* 
 * function multiplying a list of 5 by an integer
 *  - res is the resulting list
 *  - lst is the list we want to multiply
 *  - x is the integer
 *  - n0 and n4 can vary
 */
void mul_lst ( long int res[5], long int lst[5], long int x, long int n0,
               long int n4 )
  {
    long long int r;
    if (x >=0)
      {
        r = (long long int) lst[4] * (long long int) x;
        res[4] = (long) (r % (long long int)n4);
        r = (long long int) lst[3] * (long long int) x + (r / (long long int) n4);
        res[3] = (long) (r % 6);
        r = (long long int) lst[2] * (long long int) x + (r / 6);
        res[2] = (long) (r % 60);
        r = (long long int) lst[1] * (long long int) x + (r / 60);
        res[1] = (long) (r % 60);
        r = (long long int) lst[0] * (long long int) x + (r / 60);
        res[0] = (long) (r % (long long int)n0);
      }
    else
      {
        x = -x;
        r = (long long int) lst[4] * (long long int) x;
        res[4] = (long) (r % (long long int)n4);
        r = (long long int) lst[3] * (long long int) x + (r / (long long int) n4);
        res[3] = (long) (r % 6);
        r = (long long int) lst[2] * (long long int) x + (r / 6);
        res[2] = (long) (r % 60);
        r = (long long int) lst[1] * (long long int) x + (r / 60);
        res[1] = (long) (r % 60);
        r = (long long int) lst[0] * (long long int) x + (r / 60);
        res[0] = (long) (r % (long long int)n0);
        sub_lst ( res, zerolst, res, n0, n4);
      }
  }

/* 
 * function multiplying a list of 6 by an integer
 *  - res is the resulting list
 *  - lst is the list we want to multiply
 *  - x is the integer
 *  - n4 and n5 can vary
 */
void mul_lst_6 ( long int res[6], long int lst[6], long int x, long int n4,
               long int n5 )
  {
    long long int r;
    // we consider that it's called only with positive values of x.
        r = (long long int) lst[5] * (long long int) x;
        res[5] = (long) (r % (long long int)n5);
        r = (long long int) lst[4] * (long long int) x + (r / (long long int) n5);
        res[4] = (long) (r % (long long int)n4);
        r = (long long int) lst[3] * (long long int) x + (r / (long long int) n4);
        res[3] = (long) (r % 6);
        r = (long long int) lst[2] * (long long int) x + (r / 6);
        res[2] = (long) (r % 60);
        r = (long long int) lst[1] * (long long int) x + (r / 60);
        res[1] = (long) (r % 60);
        r = (long long int) lst[0] * (long long int) x + (r / 60);
        res[0] = (long) (r);
  }

/*
 * a very brutal function multiplying two lists by reducing both to lowest fractional part,
 * multiplying the result and making a list out of it again.
 * the function supposes n0 and n4 are the same for the two lists
 * worse case would be to multiply 26;59,59,5,4815376 (4815377) x 26;59,59,5,4815376 (4815377)
 * which is, to lower unit:  2808327866399 x 2808327866399 = 7886705405193159593227201 (just lower than 2^83)
 * which is higher than 2^63 -1 = 9223372036854775808... so this is not OK, for the general case, 
 * we have to use an arbitrary-precision arithmetic library, like GMP (it was the case in EH's code
 * with the BCD.
 * The best solution would be to add mini-gmp.(c|h) in the code, and to have a check
 * wether the gmp library is present or not, and if not, use mini-gmp (a bit slower than
 * normal gmp)
 * 
 * Warning: we assume that n0 is not above 27
 */

void 
mul_lst_lst ( long int res[5], long int lst1[5], long int lst2[5], long int n0,
               long int n4, unsigned char flag)
{
  long long int lst1_frac;
  long long int r; // a temporary result
  long int tmp;
  mpz_t gmp_r, gmp_lst2_frac, gmp_lst1_frac, gmp_n4;
  
  // now we have to decide if we can work with long long int or if we need to use gmp
    if (n4 < 5206) // sqrt((2^63−1)÷(27×60×60×6)^2) = 5206, the max value authorized if we consider n0 to be 27
    {
  // for this, long long int are enough
  lst1_frac = (long long int) lst1[4] 
        + (long long int) n4 * (long long int) lst1[3]
        + 6LL * (long long int) n4 * (long long int) lst1[2]
        + 60LL * 6LL * (long long int) n4 * (long long int) lst1[1]
        + 60LL * 60LL * 6LL * (long long int) n4 * (long long int) lst1[0];

  // res is the result, basically it's just lst1_frac * lst2_frac (where lst2_frac is computed
  // the same way lst1_frac was
  r =   lst1_frac * ((long long int) lst2[4] 
        + (long long int) n4 * (long long int) lst2[3]
        + 6LL * (long long int) n4 * (long long int) lst2[2]
        + 60LL * 6LL * (long long int) n4 * (long long int) lst2[1]
        + 60LL * 60LL * 6LL * (long long int) n4 * (long long int) lst2[0]);
  
    r = r / (6LL * 60LL * 60LL* (long long int) n4);
  // now something to be more efficient (we mod by a full circle)
  r = r % (6LL * 60LL * 60LL* (long long int) n4 * (long long int) n0);
  
  res[4] = (long int) (r % ((long long int) n4));
  r = r / ((long long int) n4 );
  res[3] = (long int) (r % (6LL));
  r = r / (6LL);
  res[2] = (long int) (r % (60LL));
  r = r / (60LL);
  res[1] = (long int) (r % (60LL));
  res[0] = ((long int) r / (60L)) % n0; // we don't need to mod by n0, we already mod by a full circle
    }
   else
    {
        // We need to know the highest number of bits, for one value it's 26;59,59,5,4815376 (4815377), lower than 2^42
      mpz_init_set_ui(gmp_n4, n4);
      mpz_init2(gmp_lst1_frac, 42);
      mpz_init2(gmp_lst2_frac, 42);
      mpz_init2(gmp_r,83); // highest value imaginable is 83 bits long
      // in order to set them to the correct value, we use a shortcut
      // not that we cannot assign a long long int into a mpz_t
      tmp = lst1[3] + 6L * lst1[2] + 60L * 6L * lst1[1] + 60L * 60L * 6L  * lst1[0]; // here we can use long, no problem
      mpz_set_si(gmp_lst1_frac, tmp);
      mpz_mul(gmp_lst1_frac, gmp_lst1_frac, gmp_n4);
      mpz_add_ui(gmp_lst1_frac, gmp_lst1_frac, (unsigned long int) lst1[4]);
      // now we have gmp_lst1 set, we do the same for gmp_lst2
      tmp = lst2[3] + 6L * lst2[2] + 60L * 6L * lst2[1] + 60L * 60L * 6L  * lst2[0];
      mpz_set_si(gmp_lst2_frac, tmp);
      mpz_mul(gmp_lst2_frac, gmp_lst2_frac, gmp_n4);
      mpz_add_ui(gmp_lst2_frac, gmp_lst2_frac, (unsigned long int) lst2[4]);
      // now gmp_lst1 and gmp_lst2 are set, we multiply them
      mpz_mul (gmp_r, gmp_lst1_frac, gmp_lst2_frac);
      // we divide by the ratio
      mpz_set_si(gmp_lst1_frac, 60L*60L*6L);
      mpz_mul(gmp_lst1_frac, gmp_lst1_frac, gmp_n4);
      mpz_div(gmp_r, gmp_r, gmp_lst1_frac);
      // we mod by a full circle
      mpz_mul_ui(gmp_lst1_frac, gmp_lst1_frac, (unsigned long int) n0);
      mpz_mod(gmp_r, gmp_r, gmp_lst1_frac);
      // now we set res[4], using gmp_lst1 as temporary value
      mpz_fdiv_qr(gmp_r, gmp_lst1_frac, gmp_r, gmp_n4);
      //gmp_r is quotient and gmp_lst1 is mod
      res[4] = mpz_get_si(gmp_lst1_frac);
      // max value is now 27*60*60*6 = 583200 < 2^32, so we can safely come back to just long ints!
      tmp = mpz_get_si(gmp_r);
      res[3] = tmp % 6L;
      tmp = tmp / 6L;
      res[2] =  tmp % 60L;
      tmp = tmp / 60L;
      res[1] = tmp % 60L;
      res[0] = (tmp / 60L); // we don't have to %n0 as we already have mod by a full circle
    }
}

/*
 * A function similar to the previous one, but dividing one list by another
 * by reducing both to lowest fractional part,
 * the function supposes n0 and n4 are the same for the two lists
 * worse case would be to divise 26;59,59,5,4815376 (4815377) by anything, as we have to
 * first multiply it by n0;0,0,0,0, so this makes 2808327866399 x 2808327866400 (just lower than 2^83)
 * which is higher than 2^63 -1 = 9223372036854775808... so same comment as previous function!
 * 
 * Warning: we assume that n0 is not above 27.
 *
 * Warning 2: use with caution, this kind of operation is *never* found in the traditional
 * calculations!
 *
 */

void
div_lst_lst (long int res[5], long int lst1[5], long int lst2[5], long int n4)
{
  long long int lst1_frac, lst2_frac, r;
  long int tmp;
  mpz_t gmp_r, gmp_lst2_frac, gmp_lst1_frac, gmp_n4;

  // now we have to decide if we can work with long long int or if we need to use gmp
  if (n4 < 5206)		// in the case of div_lst_lst, it might be a bit more... but it's not important...
    {
      lst1_frac = (long long int) lst1[4]
	+ (long long int) n4 *(long long int) lst1[3]
	+ 6LL * (long long int) n4 *(long long int) lst1[2]
	+ 60LL * 6LL * (long long int) n4 *(long long int) lst1[1]
	+ 60LL * 60LL * 6LL * (long long int) n4 *(long long int) lst1[0];

      // we compute lst2_frac to check division by 0
      lst2_frac = (long long int) lst2[4]
	+ (long long int) n4 *(long long int) lst2[3]
	+ 6LL * (long long int) n4 *(long long int) lst2[2]
	+ 60LL * 6LL * (long long int) n4 *(long long int) lst2[1]
	+ 60LL * 60LL * 6LL * (long long int) n4 *(long long int) lst2[0];

      if (lst2_frac == 0)
	{
	  printf ("error: division by 0 in div_lst_lst\n");
	  return;
	}

      // now we have to multiply by the factor for the lowest fractional part: to take the
      // same example as above, 2,3 / 3,2 : 23/32 = 0 (we work only with integers here!), which is not
      // really helpful... if we first multiply by 10: 230/32 = 7, so it makes 0,7
      r = lst1_frac * 6LL * 60LL * 60LL * ((long long int) n4);

      // now we divide
      r = r / lst2_frac;

      res[4] = (long int) (r % ((long long int) n4));
      r = r / ((long long int) n4);
      res[3] = (long int) (r % (6LL));
      r = r / (6LL);
      res[2] = (long int) (r % (60LL));
      r = r / (60LL);
      res[1] = (long int) (r % (60LL));
      res[0] = ((long int) r / (60L));	// with division, it's not necessary to % by n0
    }
  else
    {
      // beginning is the same as mul_lst_lst
      mpz_init_set_ui (gmp_n4, (unsigned long int) n4);
      mpz_init2 (gmp_lst1_frac, 42);
      mpz_init2 (gmp_lst2_frac, 42);
      mpz_init2 (gmp_r, 83);
      tmp =
	lst1[3] + 6L * lst1[2] + 60L * 6L * lst1[1] +
	60L * 60L * 6L * lst1[0];
      mpz_set_si (gmp_lst1_frac, tmp);
      mpz_mul (gmp_lst1_frac, gmp_lst1_frac, gmp_n4);
      mpz_add_ui (gmp_lst1_frac, gmp_lst1_frac, (unsigned long int) lst1[4]);
      tmp =
	lst2[3] + 6L * lst2[2] + 60L * 6L * lst2[1] +
	60L * 60L * 6L * lst2[0];
      mpz_set_si (gmp_lst2_frac, tmp);
      mpz_mul (gmp_lst2_frac, gmp_lst2_frac, gmp_n4);
      mpz_add_ui (gmp_lst2_frac, gmp_lst2_frac, (unsigned long int) lst2[4]);
      // checking division by 0:
      if (mpz_cmp_si (gmp_lst2_frac, 0L) == 0)
	{
	  printf ("error: division by 0 in div_lst_lst\n");
	  return;
	}
      // now gmp_lst1 and gmp_lst2 are set, we multiply gmp_lst1_frac by 60*60*6*n4
      mpz_mul_ui (gmp_r, gmp_lst1_frac, 6L * 60L * 60L);
      mpz_mul (gmp_r, gmp_r, gmp_n4);
      // and we divide
      mpz_div (gmp_r, gmp_r, gmp_lst2_frac);
      // now we set res[4], using gmp_lst1 as temporary value
      mpz_fdiv_qr (gmp_r, gmp_lst1_frac, gmp_r, gmp_n4);
      //gmp_r is quotient and gmp_lst1 is mod
      res[4] = mpz_get_si (gmp_lst1_frac);
      // max value is now 27*60*60*6 = 583200 < 2^32, so we can safely come back to just long ints!
      tmp = mpz_get_si (gmp_r);
      res[3] = tmp % 6L;
      tmp = tmp / 6L;
      res[2] = tmp % 60L;
      tmp = tmp / 60L;
      res[1] = tmp % 60L;
      res[0] = (tmp / 60L);	// we don't have to %n0 as we already have mod by a full circle
    }
}

/* 
 * function adding two lists of 5 integers:
 *  a1 = a2 + a3
 *  n0 and n4 can vary
 */
void add_lst ( long int a1[5], long int a2[5], long int a3[5], long int n0,
               long int n4)
  {
    long int r;
    r = a2[4] + a3[4];
    if ( r < 0 )
      {
        // error
        return;
      }
    a1[4] = r % n4;
    r = a2[3] + a3[3] + r / n4;
    a1[3] = r % 6;
    r = a2[2] + a3[2] + r / 6;
    a1[2] = r % 60;
    r = a2[1] + a3[1] + r / 60;
    a1[1] = r % 60;
    r = a2[0] + a3[0] + r / 60;
    a1[0] = r % n0;
  }

/* 
 * function adding two lists of 2 integers: TODO: really useful?
 *  a1 = a2 + a3
 *  n1 can vary
 */
void add_lst_2 ( long int a1[2], long int a2[2], long int a3[2], long int n1)
  {
    long int r;
    r = a2[1] + a3[1];
    if ( r < 0 )
      {
        // error
        return;
      }
    a1[1] = r % n1;
    a1[0] = r = a2[0] + a3[0] + r / n1;
  }

/* 
 * function substracting two lists of 5 integers:
 *  a1 = a2 - a3
 *  n0 and n4 can vary
 */
void sub_lst ( long int a1[5], long int a2[5], long int a3[5], long int n0,
               long int n4 )
  {
  long int a4[5];
  int  i;
    for ( i = 0; i < 5; ++i )
      a4[i] = a2[i];
    a1[4] = a4[4] - a3[4];
    if ( a1[4] < 0 )
      {
        a1[4] = a1[4] + n4;
        a4[3] = a4[3] - 1;
      }
    a1[3] = a4[3] - a3[3];
    if ( a1[3] < 0 )
      {
        a1[3] = a1[3] + 6;
        a4[2] = a4[2] - 1;
      }
    a1[2] = a4[2] - a3[2];
    if ( a1[2] < 0 )
      {
        a1[2] = a1[2] + 60;
        a4[1] = a4[1] - 1;
      }
    a1[1] = a4[1] - a3[1];
    if ( a1[1] < 0 )
      {
        a1[1] = a1[1] + 60;
        a4[0] = a4[0] - 1;
      }
    a1[0] = a4[0] - a3[0];
    if ( a1[0] < 0 )
      a1[0] = a1[0] + n0;
  }
  
/* 
 * function adding two lists of 6 integers:
 *  a1 = a2 - a3
 *  n0, n4 and n5 can vary
 */
void sub_lst_6 ( long int a1[6], long int a2[6], long int a3[6], long int n0,
               long int n4, long int n5 )
  {
  long int tmp[6];
    copy_lst_6(tmp, a2);

    a1[5] = tmp[5] - a3[5];
    if ( a1[5] < 0L )
      {
        a1[5] = a1[5] + n5;
        tmp[4] = tmp[4] - 1;
      }

    a1[4] = tmp[4] - a3[4];
    if ( a1[4] < 0L )
      {
        a1[4] = a1[4] + n4;
        tmp[3] = tmp[3] - 1;
      }

    a1[3] = tmp[3] - a3[3];
    if ( a1[3] < 0L )
      {
        a1[3] = a1[3] + 6;
        tmp[2] = tmp[2] - 1;
      }
    a1[2] = tmp[2] - a3[2];
    if ( a1[2] < 0L )
      {
        a1[2] = a1[2] + 60;
        tmp[1] = tmp[1] - 1;
      }
    a1[1] = tmp[1] - a3[1];
    if ( a1[1] < 0L )
      {
        a1[1] = a1[1] + 60;
        tmp[0] = tmp[0] - 1;
      }
    a1[0] = tmp[0] - a3[0];
    if ( a1[0] < 0L )
      a1[0] = a1[0] + n0;
  }
  
// a simple function to clear a list of 5 elements
inline void clear_lst ( long int l[5] )
  {
    unsigned char n;
    for ( n = 0; n < 5; ++n )
      l[n] = 0;
  }
  
// a simple function to clear a list of 6 elements
inline void clear_lst_6 ( long int l[6] )
  {
    unsigned char n;
    for ( n = 0; n < 6; ++n )
      l[n] = 0;
  }
  
inline void copy_lst(long int dest[5], long src[5])
  {
    unsigned char n;
    for ( n = 0; n < 5; ++n )
      dest[n] = src[n];  
  }
  
inline void copy_lst_6(long int dest[6], long src[6])
  {
    unsigned char n;
    for ( n = 0; n < 6; ++n )
      dest[n] = src[n];  
  }
