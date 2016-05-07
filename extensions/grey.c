/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2013, Daniel Sabo
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
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#include "babl/babl.h"

#include "base/util.h"
#include "extensions/util.h"

static double babl_get_Y (double Y[3])
{
  Y[0] = SRGB_RED_Y;
  Y[1] = SRGB_GREEN_Y;
  Y[2] = SRGB_BLUE_Y;

  if ( colorant_babl != NULL) /* Does this still work once colorant_babl
  has been used the first time? Probably not. Does it need to? */
    {
      double *new_colorant_data = babl_get_user_data (colorant_babl);
      //printf ("babl grey.c 1: babl_get_user_data\n");

      Y[0] = new_colorant_data[1];
      Y[1] = new_colorant_data[4];
      Y[2] = new_colorant_data[7];
      //printf("babl grey.c 2: \nrY=%.8f gY=%.8f bY:%.8f\n\n", Y[0], Y[1], Y[2]);
    }

  return Y[3];
}

static long
conv_rgbaF_linear_y8_linear (unsigned char *src,
                             unsigned char *dst,
                             long           samples)
{
  float *s = (float *) src;
  long   n = samples;
  double Y[3];
  babl_get_Y (Y);
  //printf("babl grey.c 3: \nrY=%.8f gY=%.8f bY:%.8f\n", Y[0], Y[1], Y[2]);
  while (n--)
    {
      float value;
      long int v;
      value  = *s++ * Y[0];
      value += *s++ * Y[1];
      value += *s++ * Y[2];
      s++;

      v = rint (value * 255.0);
      *dst++ = (v < 0) ? 0 : ((v > 255) ? 255 : v);
    }

  return samples;
}

static long
conv_rgbaF_linear_yF_linear (unsigned char *src,
                             unsigned char *dst,
                             long           samples)
{
  float *s = (float *) src;
  float *d = (float *) dst;
  long   n = samples;
  double Y[3];
  babl_get_Y (Y);

  while (n--)
    {
      float value;
      value  = *s++ * Y[0];
      value += *s++ * Y[1];
      value += *s++ * Y[2];
      s++;
      *d++ = value;
    }

  return samples;
}

static long
conv_rgbaF_linear_yaF_linear (unsigned char *src,
                              unsigned char *dst,
                              long           samples)
{
  float *s = (float *) src;
  float *d = (float *) dst;
  long   n = samples;
  double Y[3];
  babl_get_Y (Y);

  while (n--)
    {
      float value;
      value  = *s++ * Y[0];
      value += *s++ * Y[1];
      value += *s++ * Y[2];
      *d++ = value;
      *d++ = *s++;  /* alpha */
    }

  return samples;
}

int init (void);

int
init (void)
{
  babl_conversion_new (babl_format ("RGBA float"),
                       babl_format ("Y u8"),
                       "linear",
                       conv_rgbaF_linear_y8_linear,
                       NULL);

  babl_conversion_new (babl_format ("RGBA float"),
                       babl_format ("Y float"),
                       "linear",
                       conv_rgbaF_linear_yF_linear,
                       NULL);

  babl_conversion_new (babl_format ("RGBA float"),
                       babl_format ("YA float"),
                       "linear",
                       conv_rgbaF_linear_yaF_linear,
                       NULL);

  return 0;
}
