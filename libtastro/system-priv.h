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

#include "system.h"

// private data for the epochs

static const epoch phugpa_epoch= // we take -1000 as a reference
{
    -1000,
    7,
    6,
    61,
    10,
    26,
    5,
    1355847,
    93,
    6663418,
    2080,
    277,
    4,
    511,
    2995,
    48,
    { 5, 35, 36, 4, 160, 0 },
    { 26, 45, 53, 4, 26, 0 },
};

static const epoch tsurphu_epoch= // we take -1000 as a reference
{
    -1000,
    11,
    6,
    75,
    50,
    66,
    5,
    1355847,
    93,
    83343,
    1977,
    268,
    3,
    512,
    2988,
    0, // used only for phugpa
    { 5, 38, 22, 3, 701, 0 },
    { 0, 7, 25, 0, 30, 0 },
};

// for now we take the -1000 version
static const epoch sherab_ling_epoch= // we take -1000 as a reference
{
    -1000,
    45,
    4,
    19,
    416,
    56,
    4,
    1355818L,
    91,
    47477L, // ????  This is subtracted for drag po'i rkang 'dzin.
    -102L,
    86L,
    671L,
    371L,
    3167L,
    16L, // Maybe 18??
    { 5, 38, 22, 3, 701, 0 },
    { 0, 7, 25, 0, 30, 0 },
};
