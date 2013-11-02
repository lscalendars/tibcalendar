#include "astronomy.h"
#include "vsop.h"

/*
 * This function computes and returns sun longitude at time t, expressed in
 * Julian millenia. It takes into account the time light takes to travel.
 */
 
coord_t get_sun_long(time_t t) {
  // First thing is to get the distance between sun and moon. This is simply
  // the first spherical coordinate, called R (for ro) in vsop87.
  coord_t r = vsop87_d_earth_3(t);
  // Then we adjust t, reducing it by the time light takes to travel the
  // distance r.
  // Speed of light is exactly 299,792,458 m/s
  // l is in astronomical units, 1 AU = 149,597,870,700m
  // -> the light takes tlt = 149,597,870,700/299,792,458 s to travel one AU
  // -> t is in julian millenia, so tlt should be too : tlt = tlt / (24*3600*365250)
  // -> the light takes tlt*r to traval the Sun-Earth distance
  // -> if we reduce t by tlt*r, we have a good approximation of the time
  //    at which we should compute the longitude
  //
  // Here we take tlt = 0.0000000158125074098206584757228417908680
  t = t - l * 0.0000000158125074098206584757228417908680;
  // now we have our time set. Having the solar longitude is extremely easy
  // when we have earth longitude, it's just the opposite!
  // Longitude is the first variable, called L.
  return vsop87_d_earth_1(t);
}
