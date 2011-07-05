#ifndef ASTRO_H
#define ASTRO_H

#include "tbday.h"
void get_year_astro(tib_month *month);
void get_day_infos(tib_day *td);

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
