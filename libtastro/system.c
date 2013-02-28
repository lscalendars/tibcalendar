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
#include <stdlib.h>
#include <string.h>
#include "system-priv.h"
#include "tbmath.h"		// for copy_list

astro_system *
new_system ()
{
  astro_system *sys = malloc (sizeof (astro_system));
  //epoch *epch = malloc (sizeof (epoch));
  memset (sys, 0, sizeof (astro_system));
  //sys->epoch = epch;
  return sys;
}

// For now this is very basic... almost stupid, but this is where the options will come later
astro_system *
get_astro_system (unsigned char type)
{
  astro_system *sys = new_system ();
  sys->type = type;
  switch (type)
    {
    case PHUGPA:
      sys->epoch = &phugpa_epoch;
      sys->sun_f = 67L;
      sys->zlasho = 48L;
      set_lst (sys->nyi_drup_const, 2, 10, 58, 1, 17);	// solar motion in a month
      set_lst_6 (sys->nyi_long_const, 0, 4, 21, 5, 43, 0);	// same in a lunar day, see KTC p. 23
      break;
    case TSURPHU:
      sys->epoch = &tsurphu_epoch;
      sys->sun_f = 67L;
      sys->zlasho = -1;		// we don't use it in Tsurphu
      set_lst (sys->nyi_drup_const, 2, 10, 58, 1, 17);
      set_lst_6 (sys->nyi_long_const, 0, 4, 21, 5, 43, 0);
      break;
    case SHERAB_LING:
      sys->epoch = &sherab_ling_epoch;
      sys->sun_f = 4815377L;
      sys->zlasho = 16L;	// Maybe 17??
      set_lst (sys->nyi_drup_const, 2, 10, 58, 2, 3846950L);
      set_lst_6 (sys->nyi_long_const, 0, 4, 21, 5, 3338483L, 0);	// code was div_g6 ( nyilm, 30L, sun_f, 1L )
      break;
    case BHUTAN:
      sys->epoch = &bhutan_epoch;
      sys->sun_f = 67L;
      sys->zlasho = 59L;	// (comment from EH's code) Not found this information in the actual texts - calculated; originally had 56
      set_lst (sys->nyi_drup_const, 2, 10, 58, 1, 17);
      set_lst_6 (sys->nyi_long_const, 0, 4, 21, 5, 43, 0);
      break;
    default:
      break;
    }
  return sys;
}
