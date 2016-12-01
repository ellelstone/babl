/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2012 Øyvind Kolås.
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "babl.h"

#include "base/util.h"

int init (void);

static inline long
conv_rgba8_cairo24_le (unsigned char *src, unsigned char *dst, long samples)
{
  long n = samples;
  while (n--)
    {
      unsigned char red   = *src++;
      unsigned char green = *src++;
      unsigned char blue  = *src++;
      *dst++ = blue;
      *dst++ = green;
      *dst++ = red;
      *dst++ = 255;
      src++;
    }
  return samples;
}

static inline long
conv_rgb8_cairo24_le (unsigned char *src, unsigned char *dst, long samples)
{
  long n = samples;
  while (n--)
    {
      unsigned char red   = *src++;
      unsigned char green = *src++;
      unsigned char blue  = *src++;
      *dst++ = blue;
      *dst++ = green;
      *dst++ = red;
      *dst++ = 255;
    }
  return samples;
}

static inline long
conv_rgbA8_premul_cairo32_le (unsigned char *src, unsigned char *dst, long samples)
{
  long n = samples;
  while (n--)
    {
      unsigned char red    = *src++;
      unsigned char green  = *src++;
      unsigned char blue   = *src++;
      unsigned char alpha  = *src++;

      *dst++ = blue;
      *dst++ = green;
      *dst++ = red;
      *dst++ = alpha;
    }
  return samples;
}

static inline long
conv_rgbA8_cairo32_le (unsigned char *src, unsigned char *dst, long samples)
{
  long n = samples;
  while (n--)
    {
      unsigned char red    = *src++;
      unsigned char green  = *src++;
      unsigned char blue   = *src++;
      unsigned char alpha  = *src++;

#define div_255(a) ((((a)+128)+(((a)+128)>>8))>>8)
      *dst++ = div_255 (blue  * alpha);
      *dst++ = div_255 (green * alpha);
      *dst++ = div_255 (red   * alpha);
#undef div_255
      *dst++ = alpha;
    }
  return samples;
}

static long
conv_rgbafloat_cairo32_le (unsigned char *src,
                           unsigned char *dst,
                           long           samples)
{
  float *fsrc = (float *) src;
  unsigned char *cdst = (unsigned char *) dst;
  int n = samples;

  while (n--)
    {
      float red    = *fsrc++;
      float green  = *fsrc++;
      float blue   = *fsrc++;
      float alpha  = *fsrc++;
      if (alpha >= 1.0)
      {
        int val = blue * 0xff + 0.5f;
        *cdst++ = val >= 0xff ? 0xff : val <= 0 ? 0 : val;
        val = green * 0xff + 0.5f;
        *cdst++ = val >= 0xff ? 0xff : val <= 0 ? 0 : val;
        val = red * 0xff + 0.5f;
        *cdst++ = val >= 0xff ? 0xff : val <= 0 ? 0 : val;
        *cdst++ = 0xff;
      }
      else if (alpha <= 0.0)
      {
        (*(uint32_t*)cdst)=0;
        cdst+=4;
      }
      else
      {
        float balpha = alpha * 0xff;
        int val = blue * balpha + 0.5f;
        *cdst++ = val >= 0xff ? 0xff : val <= 0 ? 0 : val;
        val = green * balpha + 0.5f;
        *cdst++ = val >= 0xff ? 0xff : val <= 0 ? 0 : val;
        val = red * balpha + 0.5f;
        *cdst++ = val >= 0xff ? 0xff : val <= 0 ? 0 : val;
        *cdst++ = balpha + 0.5f;
      }
    }
  return samples;
}

int
init (void)
{
  int   testint  = 23;
  char *testchar = (char*) &testint;
  int   littleendian = (testchar[0] == 23);

  if (littleendian)
    {
      const Babl *f32 = babl_format_new (
        "name", "cairo-ARGB32",
        babl_model ("RaGaBaA"),
        babl_type ("u8"),
        babl_component ("Ba"),
        babl_component ("Ga"),
        babl_component ("Ra"),
        babl_component ("A"),
        NULL
      );

      const Babl *f24 = babl_format_new (
        "name", "cairo-RGB24",
        babl_model ("RGB"),
        babl_type ("u8"),
        babl_component ("B"),
        babl_component ("G"),
        babl_component ("R"),
        babl_component ("PAD"),
        NULL
      );

      babl_conversion_new (babl_format ("RaGaBaA u8"), f32, "linear",
                           conv_rgbA8_premul_cairo32_le, NULL);
      babl_conversion_new (babl_format ("RGBA u8"), f32, "linear",
                           conv_rgbA8_cairo32_le, NULL);

      babl_conversion_new (babl_format ("RGBA float"), f32, "linear",
                           conv_rgbafloat_cairo32_le, NULL);

      babl_conversion_new (babl_format ("RGBA u8"), f24, "linear",
                           conv_rgba8_cairo24_le, NULL);
      babl_conversion_new (babl_format ("RGB u8"), f24, "linear",
                           conv_rgb8_cairo24_le, NULL);
    }
  else
    {
      babl_format_new (
        "name", "cairo-ARGB32",
        babl_model ("RaGaBaA"),
        babl_type ("u8"),
        babl_component ("A"),
        babl_component ("Ra"),
        babl_component ("Ga"),
        babl_component ("Ba"),
        NULL
      );
      babl_format_new (
        "name", "cairo-RGB24",
        babl_model ("RGB"),
        babl_type ("u8"),
        babl_component ("PAD"),
        babl_component ("R"),
        babl_component ("G"),
        babl_component ("B"),
        NULL
      );
    }
  babl_format_new (
    "name", "cairo-A8",
    babl_model ("YA"),
    babl_type ("u8"),
    babl_component ("A"),
    NULL
    );
  
  return 0;
}
