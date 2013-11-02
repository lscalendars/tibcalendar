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

#include <math.h>

/* First a macro to define if we use long double or double (by default double).
 * We assume T is always double.
 */
#define USE_LONG 0

/* Here we define the type of the numbers we use, and append L or not to the
 * literal through macro _()
 */
#if USE_LONG == 0
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
#  if __GNUC_PREREQ(4,0)  //      If  gcc_version >= 4.0
#    if USE_LONG == 0
#      define USE_DOUBLE_VECTOR 1
#    endif
#  endif
#endif

#if USE_DOUBLE_VECTOR == 1
// this is the vector variable type
typedef double double64x2_t __attribute__ ((vector_size(16)));
#  define initialize() \
     coord_t res = 0;\
     double64x2_t x = (double64x2_t){0, 0};\
     double64x2_t T = (double64x2_t){t, t};\
     double64x2_t tmp = (double64x2_t){0, 0}
#  define cosvec(tmp) \
     tmp[0] = cos(tmp[0]);\
     tmp[1] = cos(tmp[1])
#  define twoops(a1,b1,c1,a2,b2,c2) \
     tmp = (double64x2_t){b1, b2} + ((double64x2_t){c1, c2} * T);\
     cosvec(tmp);\
     x = x + (tmp * (double64x2_t){a1, a2})
#  define oneop(a,b,c) \
     res += a * cos(b+c*t)
#  define end()\
     res += x[0] + x[1];\
     return res
#else // we don't use vectors here, so it's failry simple
#  define initialize() \
     coord_t x = 0
#  if USE_LONG == 0
#    define vsopcos cos
#  else
#    define vsopcos cosl
#  endif
#  define twoops(a1,b1,c1,a2,b2,c2) \
     x += a1 * vsopcos(b1+c1*t);\
     x += a2 * vsopcos(b2+c2*t)
#  define twoops(a,b,c) \
     x += a * vsopcos(b+c*t)
#  define end()\
     return x
#endif

/* Next are the declarations of the main functions:
 */

coord_t vsop87_c_earth_1 (time_t t);
coord_t vsop87_c_earth_2 (time_t t);
coord_t vsop87_c_earth_3 (time_t t);

#endif

