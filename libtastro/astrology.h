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

#ifndef ASTRO_H
#define ASTRO_H

#include "tbday.h"
#include "tbstructures.h"
#include "system.h"

void get_year_astro_data(tib_year *ty);
void get_month_astro_data(tib_month *tm, astro_system *asys);
void get_day_astro_data(tib_day *td, astro_system *asys, unsigned char updateflg);
void check_sadag (unsigned char m, unsigned char t, tib_day_astro_data *tda);
void check_anniversary (unsigned char m, unsigned char t, tib_day *td, astro_system *asys );
unsigned char get_lunar_mansion_element (unsigned char lm);
unsigned char get_dow_element (unsigned char dow);

// The four elements
#define FOUR_EARTH 0
#define FOUR_WATER 1
#define FOUR_FIRE 2
#define FOUR_WIND 3

// The five elements
#define FIRE 0
#define EARTH 1
#define IRON 2
#define WATER 3
#define WOOD 4

// The two genders
#define MALE 0
#define FEMALE 1

// The twelve animals
#define RABBIT 0
#define DRAGON 1
#define SNAKE 2
#define HORSE 3
#define SHEEP 4
#define MONKEY 5
#define BIRD 6
#define DOG 7
#define PIG 8
#define MOUSE 9
#define OX 10
#define TIGER 11

// The five sanskrit vowels (see KTC p. 156)
#define VOW_A 0
#define VOW_I 1
#define VOW_R 2
#define VOW_U 3
#define VOW_L 4

#endif
