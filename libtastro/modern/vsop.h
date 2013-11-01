/*
Macros to optimize speed and precision in VSOP calculations.

Copyright (c) 2013 Elie Roux <elie.roux@telecom-bretagne.eu>

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

#if !defined __VSOP_H
#define __VSOP_H

/* First a macro to define if we use long double or double (by default double).
 * We assume T is always double.
 */
#define USE_LONG 0

/* Here we define the type of the numbers we use, and append L or not to the
 * literal through macro _()
 */
#if USE_LONG == 1
typedef double coord_t;
typedef double time_t;
#  define _(x) x
#else
typedef long double time_t;
typedef long double coord_t;
#  define add_suffix(x) x##L
#  define _(x) add_suffix(x)
#endif

/* First thing here is to detect whether to use vectorization or not.
 * Vector extensions have been added to GCC for a long time but we use 4.0, 
 * syntax, so we first set the macro 
 *  USE_DOUBLE_VECTOR
 * to 0 or 1. If we don't use double but long double, we can't use vector
 * extensions (well, there might be architectures that could, but nothing I can
 * test), so it will be set to 0 in this case.
 */
#define USE_DOUBLE_VECTOR 0
#ifdef __GNUC__ // we are on GCC
#  include <features.h>
#  if __GNUC_PREREQ(4,0)  //      If  gcc_version >= 4.7
#    if USE_LONG == 0
#      define USE_DOUBLE_VECTOR 1
#    endif
#  endif
#endif

#if USE_DOUBLE_VECTOR == 1
// this is the vector variable type
typedef double double64x2_t __attribute__ ((vector_size(16)));
#  define initialize() \
     coordtype res = 0;\
     double64x2_t x = {0,0};\
     double64x2_t t = {T,T};\
     double64x2_t tmp = {0,0}
#  define cosvec(x) \
     x[0] = cos(x[0]);\
     x[1] = cos(x[1])
#  define twoops(a1,b1,c1,a2,b2,c2) \
     tmp = {b1, b2} + ({c1,c2} * T);\
     cosvec(tmp);\
     x = x + (tmp * {a1, a1})
#  define oneop(a,b,c) \
     res += a * cos(b+c*t)
#  define end()\
     res += x[0] + x[1];\
     return res
#else // we don't use vectors here, so it's failry simple
#  define initialize() \
     coordtype x = 0
#  define twoops(a1,b1,c1,a2,b2,c2) \
     x += a1 * cos(b1+c1*t);\
     x += a2 * cos(b2+c2*t)
#  define twoops(a,b,c) \
     x += a * cos(b+c*t)
#  define end()\
     return x
#endif


#endif

