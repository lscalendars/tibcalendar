/*
License for Tibastro - Tibetan almanach calculation program

Copyright (c) 2009-2011 Edward Henning
Copyright (c) 2012-2013 Elie Roux <elie.roux@telecom-bretagne.eu>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef UTILS_H
#define UTILS_H

// see comments of get_approx function
#define TRUCATE 0
#define APPROX 1

// first a useful list:
extern long int zerolst[5];

void add_lst_6 ( long int a1[6], long int a2[6], long int a3[6], long int n0,
              long int n4, long int n5 );
void mul_lst ( long int res[5], long int lst[5], long int x, long int n0,
               long int n4 );
void mul_lst_lst ( long int res[5], long int lst1[5], long int lst2[5], long int n0,
               long int n4);
void div_lst_lst ( long int res[5], long int lst1[5], long int lst2[5], long int n4);
void add_lst ( long int a1[5], long int a2[5], long int a3[5], long int n0,
               long int n4);
void sub_lst ( long int a1[5], long int a2[5], long int a3[5], long int n0,
               long int n4 );
void div_lst_6 ( long int a1[6], long int a2[6], long int x, long int n4, long int n5 );
void div_lst ( long int res[6], long int lst[6], long int x, long int n4 );
void clear_lst ( long int l[5]);
void copy_lst ( long int dest[5], long int src[5]);
void mul_lst_6 ( long int res[6], long int lst[6], long int x, long int n4, long int n5 );
void sub_lst_6 ( long int a1[6], long int a2[6], long int a3[6], long int n0, long int n4, long int n5 );
void clear_lst_6 ( long int l[6] );
void copy_lst_6 ( long int dest[6], long int src[6]);

void set_lst(long int lst[5], long int lst0, long int lst1, long int lst2, long int lst3, long int lst4);
void set_lst_6(long int lst[5], long int lst0, long int lst1, long int lst2, long int lst3, long int lst4, long int lst5);

void get_approx (long int lst[5], long int * a0, long int * a1, long int * a2, long int n0, unsigned char trucate);

#endif
