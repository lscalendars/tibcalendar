#ifndef ASTRO_H
#define ASTRO_H

#include "tbday.h"
#include "tbstructures.h"

void get_year_astro_data(tib_year *ty);
void get_month_astro_data(tib_month *tm);
void get_day_astro_data(tib_day *td);
void check_sadag (unsigned char m, unsigned char t, tib_day_astro_data *tda);

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

#endif
