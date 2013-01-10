/*********************************************************************************
License for Tibastro - Tibetan astronomical and astrological calculation program

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

#include <string.h>		// for memset
#include <stdlib.h> // for malloc, free
#include "tbstructures.h"

tib_month *
new_tib_month()
{
  tib_month *tm = malloc (sizeof (tib_month));
  memset (tm, 0, sizeof (tib_month));
  tm->year = new_tib_year();
  return tm;
}

tib_year *
new_tib_year()
{
  tib_year *ty = malloc (sizeof (tib_year));
  memset (ty, 0, sizeof (tib_year));
  return ty;
}

tib_year_astro_data *
new_tib_year_astro_data()
{
  tib_year_astro_data *tya = malloc (sizeof (tib_year_astro_data));
  memset (tya, 0, sizeof (tib_year_astro_data));
  return tya;
}

tib_month_astro_data *
new_tib_month_astro_data()
{
  tib_month_astro_data *tma = malloc (sizeof (tib_month_astro_data));
  memset (tma, 0, sizeof (tib_month_astro_data));
  return tma;
}

tib_day_astro_data *
new_tib_day_astro_data()
{
  tib_day_astro_data *tda = malloc (sizeof (tib_day_astro_data));
  memset (tda, 0, sizeof (tib_day_astro_data));
  return tda;
}

tib_day *
new_tib_day()
{
  tib_day *td = malloc (sizeof (tib_day));
  memset (td, 0, sizeof (tib_day));
  td->month = new_tib_month();
  return td;
}

tib_planet_data *
new_tib_planet_data()
{
  tib_planet_data *pd = malloc (sizeof (tib_planet_data));
  memset (pd, 0, sizeof (tib_planet_data));
  return pd;
}

// frees the entire tib_year
void
free_tib_year(tib_year *ty)
{
  if (ty->astro_data)
    free(ty->astro_data);
  free(ty);
}

// frees the entire tib_month (with year)
void
free_tib_month(tib_month *tm)
{
  free_tib_year(tm->year);
  if (tm->astro_data)
    free(tm->astro_data);
  free(tm);
}

// frees the entire tib_day (with month and year)
void
free_tib_day(tib_day *td)
{
  free_tib_month(td->month);
  if (td->astro_data)
    free(td->astro_data);
  if (td->planet_data)
    free(td->planet_data);
  free(td);
}
