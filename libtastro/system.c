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
#include <stdlib.h>
#include <string.h>
#include "system-priv.h"
#include "utils.h" // for copy_list

astro_system *new_system()
{
  astro_system *sys = malloc (sizeof (astro_system));
  //epoch *epch = malloc (sizeof (epoch));
  memset (sys, 0, sizeof (astro_system));
  //sys->epoch = epch;
  return sys;
}

// For now this is very basic... almost stupid, but this is where the options will come later
astro_system *get_astro_system(unsigned char type)
{
  static long int ndc_p[5] = { 2, 10, 58, 1, 17 }; // temporary variable... could certainly be easier...
  static long int ndc_sl[5] = { 2, 10, 58, 2, 3846950L };
  astro_system *sys = new_system();
  sys->type = type;
  if (type == PHUGPA)
    {
      sys->epoch = &phugpa_epoch;
      sys->sun_f = 67L;
      copy_lst(sys->nyi_drup_const, ndc_p); // solar motion in a month
    }
  else if (type == TSURPHU)
    {
      sys->epoch = &tsurphu_epoch;
      sys->sun_f = 67L;
      copy_lst(sys->nyi_drup_const, ndc_p);
    }
  else // SHERAB_LING
    {
      sys->epoch = &sherab_ling_epoch;
      sys->sun_f = 4815377L;
      copy_lst(sys->nyi_drup_const, ndc_sl);
    }
  return sys;
}
