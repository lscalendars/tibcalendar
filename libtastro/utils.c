#include<stdio.h>
#include"utils.h"

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
    long long int b; //TODO: remove, it's useless...
    
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
