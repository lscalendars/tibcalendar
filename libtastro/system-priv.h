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

#include "system.h"

// private data for the epochs

static /*const*/ epoch phugpa_epoch= // we take -1000 as a reference
{
    -1000, // year
    7, // eyr_a
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
    0,
    { 5, 35, 36, 4, 160, 0 },
    { 26, 45, 53, 4, 26, 0 },
};

static /*const*/ epoch tsurphu_epoch= // we take -1000 as a reference
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
    0, // not used for Tsurphu // TODO: make an option for the use of it...
    { 5, 38, 22, 3, 701, 0 },
    { 0, 7, 25, 0, 30, 0 },
};

// for now we take only the -1000 version
static /*const*/ epoch sherab_ling_epoch= // we take -1000 as a reference
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
    37565L,
    { 4,  6, 56, 5, 293, 0 },
    { 22, 49, 17, 1, 2398189L, 0 },
};

// for now we take only the -1000 version
static /*const*/ epoch bhutan_epoch= // we take -1000 as a reference
{
    -1000,
    11,
    6,
    111,
    579,
    23,
    5,
    1355847L,
    93,
    17895L,
    6939L,
    284L,
    5L,
    512L,
    2988L,
    0,
    { 5, 37, 40, 1, 73, 0 },
    { 0, 6, 2, 4, 8, 0 },
};
