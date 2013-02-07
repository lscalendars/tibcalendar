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

#include"tests.h"
#include"tbday.h"
#include "tbstructures.h"
#include"utils.h"
#include"system.h"
#include"astrology.h"
#include <stdlib.h>
#include <stdio.h>

int
check_astrology ()
{
  if (test_solar_term () == 0)
    {
      return 0;
    }
  return 1;
}


// would require much more interesting cases, but seems to work fine...
int
test_solar_term ()
{
  int res;
  tib_day *td = new_tib_day ();
  astro_system *sys_p = get_astro_system (PHUGPA);
  astro_system *sys_sl = get_astro_system (SHERAB_LING);
  get_tibday_const(1, td); // we take any tibday
  get_day_astro_data (td, sys_sl, 0);
  // and we just set nyibar, gzadag, duplicated and ommited as we want
  // first a very simple check: nyibar just before a solar term, gzadag to 0
  // first solar term is 23,37,30 - 1;39,0 = 21;58,30
  set_lst(td->nyibar, 21,58,20,0,0);
  set_lst(td->gzadag, 0,0,0,0,0);
  td->ommited = 0;
  td->duplicated = 0;
  get_solar_term(td, sys_sl);
  if (td->astro_data->solar_term != 1)
     {
     free_tib_day (td);
     free (sys_p);
     free (sys_sl);
     return 0;
     }
  set_lst(td->nyibar, 20,50,0,0,0);
  set_lst(td->gzadag, 0,0,0,0,0);
  get_solar_term(td, sys_sl);
  if (td->astro_data->solar_term != 24)
     {
     free_tib_day (td);
     free (sys_p);
     free (sys_sl);
     return 0;
     }
  get_solar_term(td, sys_p);
  if (td->astro_data->solar_term != 0)
     {
     free_tib_day (td);
     free (sys_p);
     free (sys_sl);
     return 0;
     }
  free_tib_day (td);
  free (sys_p);
  free (sys_sl);
  return 1;
}
