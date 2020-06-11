/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2005, Øyvind Kolås.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef _BASE_UTIL_H
#define _BASE_UTIL_H

#define LAB_EPSILON       (216.0f / 24389.0f)
#define LAB_KAPPA         (24389.0f / 27.0f)

#include <assert.h>
#include <math.h>

//#include "pow-24.h"
#include <stdint.h> //from pow-24.h

#include <babl.h>

#define BABL_PLANAR_SANITY  \
  {                         \
    assert(src_bands>0);    \
    assert(dst_bands>0);    \
    assert(src);            \
    assert(*src);           \
    assert(dst);            \
    assert(*dst);           \
    assert(n>0);            \
    assert(*src_pitch);     \
  }

#define BABL_PLANAR_STEP          \
  {                               \
    int i;                        \
    for (i=0; i< src_bands; i++)  \
      src[i]+=src_pitch[i];       \
    for (i=0; i< dst_bands; i++)  \
      dst[i]+=dst_pitch[i];       \
  }


static inline double
babl_epsilon_for_zero (double value)
{
 if (value <=  BABL_ALPHA_FLOOR &&
     value >= -BABL_ALPHA_FLOOR)
 {
   return BABL_ALPHA_FLOOR;
 }
 return value;
}

static inline float
babl_epsilon_for_zero_float (float value)
{
 if (value <= BABL_ALPHA_FLOOR_F &&
     value >= -BABL_ALPHA_FLOOR_F)
 {
   return BABL_ALPHA_FLOOR_F;
 }
 return value;
}


#define BABL_USE_SRGB_GAMMA

#ifdef BABL_USE_SRGB_GAMMA

/* srgbtrc 2.4,   1.0 / 1.055,  0.055 / 1.055,     1.0 / 12.92,     0.04045
   labl    3.0,   1.0 / 1.16,   0.16  / 1.16,   2700.0 / 24389.0,   0.08000 */


static inline double
linear_to_gamma_2_2 (double value)
{
  if ( value > LAB_EPSILON )
    return 1.16 * pow (value, (1.0/3.0)) - 0.16;
  return (value * LAB_KAPPA) / 100.0;
}

static inline double
gamma_2_2_to_linear (double value)
{
  if ( value > 0.08 )
    return pow ((value + 0.16) / 1.16, 3.0);
  return (value * 100.0) / LAB_KAPPA;
}

static inline double
babl_linear_to_gamma_2_2 (double value)
{
  if ( value > LAB_EPSILON )
    return 1.16 * pow (value, (1.0/3.0)) - 0.16;
  return (value * LAB_KAPPA) / 100.0;
}
static inline float
babl_linear_to_gamma_2_2f (float value)
{
  if ( value > LAB_EPSILON )
    return 1.16 * pow (value, (1.0/3.0)) - 0.16;
  return (value * LAB_KAPPA) / 100.0;
}


static inline double
babl_gamma_2_2_to_linear (double value)
{
  if ( value > 0.08 )
    return pow ((value + 0.16) / 1.16, 3.0);
  return (value * 100.0) / LAB_KAPPA;
}
static inline float
babl_gamma_2_2_to_linearf (float value)
{
  if ( value > 0.08 )
    return pow ((value + 0.16) / 1.16, 3.0);
  return (value * 100.0) / LAB_KAPPA;
}

#else
  #define linear_to_gamma_2_2(value) (pow((value), (1.0F/2.2F)))
  #define gamma_2_2_to_linear(value) (pow((value), 2.2F))

  #define babl_linear_to_gamma_2_2f(value) (powf((value), (1.0f/2.2f)))
  #define babl_gamma_2_2_to_linearf(value) (powf((value), 2.2f))
#endif

#endif

