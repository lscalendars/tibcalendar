#include "system.h"
#include <stdlib.h>
#include <string.h>
#include "system-priv.h"

astro_system *new_system()
{
  astro_system *sys = malloc (sizeof (astro_system));
  epoch *epch = malloc (sizeof (epoch));
  memset (sys, 0, sizeof (astro_system));
  sys->epoch = epch;
  return sys;
}

// For now this is very basic... almost stupid, but this is where the options will come later
astro_system *get_astro_system(unsigned char type)
{
  astro_system *sys = new_system();
  sys->type = type;
  if (type == PHUGPA)
    sys->epoch = &phugpa_epoch;
  else
    sys->epoch = &tsurphu_epoch;
  return sys;
}
