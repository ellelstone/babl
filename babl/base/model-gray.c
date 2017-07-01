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
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#include "babl-classes.h"
#include "babl.h"
#include "babl-ids.h"
#include "util.h"
#include "math.h"
#include "babl-base.h"

static void components (void);
static void models (void);
static void conversions (void);
static void formats (void);

/** Set global variables for passing colorant information
 * from GIMP to babl */
const Babl *colorant_babl;
double *colorant_data;

void babl_base_model_gray (void)
{
  components ();
  models ();
  conversions ();
  formats ();
}

static double babl_get_Y (double Y[3])
{
  double *new_colorant_data = babl_get_user_data (colorant_babl);

  Y[0] = new_colorant_data[1];
  Y[1] = new_colorant_data[4];
  Y[2] = new_colorant_data[7];

  return Y[3];
}

static void
components (void)
{
  babl_component_new (
    "Y",
    "id", BABL_LUMINANCE,
    "luma",
    NULL);

  babl_component_new (
    "Ya",
    "id", BABL_LUMINANCE_MUL_ALPHA,
    "luma",
    NULL);

/*  babl_component_new (
    "Y'",
    "id", BABL_LUMA,
    "luma",
    NULL);

  babl_component_new (
    "Y'a",
    "id", BABL_LUMA_MUL_ALPHA,
    "luma",
    NULL);*/
}

static void
models (void)
{
  babl_model_new (
    "id", BABL_GRAY,
    babl_component_from_id (BABL_LUMINANCE),
    NULL);


  babl_model_new (
    "id", BABL_GRAY_ALPHA,
    babl_component_from_id (BABL_LUMINANCE),
    babl_component_from_id (BABL_ALPHA),
    NULL);

  babl_model_new (
    "id", BABL_GRAY_ALPHA_PREMULTIPLIED,
    babl_component_from_id (BABL_LUMINANCE_MUL_ALPHA),
    babl_component_from_id (BABL_ALPHA),
    NULL);

}


static long
rgba_to_graya (char *src,
               char *dst,
               long  n)
{
  double Y[3];
  babl_get_Y (Y);
  //printf("babl model-gray.c 3: \nrY=%.8f gY=%.8f bY:%.8f\n", Y[0], Y[1], Y[2]);
  while (n--)
    {
      double red, green, blue;
      double luminance, alpha;

      red   = ((double *) src)[0];
      green = ((double *) src)[1];
      blue  = ((double *) src)[2];
      alpha = ((double *) src)[3];

      luminance = red * Y[0] +
                  green * Y[1] +
                  blue * Y[2];

      ((double *) dst)[0] = luminance;
      ((double *) dst)[1] = alpha;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 2;
    }
  return n;
}

static long
rgba_to_gray (char *src,
              char *dst,
              long  n)
{
  double Y[3];
  babl_get_Y (Y);

  while (n--)
    {
      double red, green, blue;
      double luminance;

      red   = ((double *) src)[0];
      green = ((double *) src)[1];
      blue  = ((double *) src)[2];

      luminance = red * Y[0] +
                  green * Y[1] +
                  blue * Y[2];

      ((double *) dst)[0] = luminance;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 1;
    }
  return n;
}


/*static long
rgb_to_gray_2_2 (int    src_bands,
                 char **src,
                 int   *src_pitch,
                 int    dst_bands,
                 char **dst,
                 int   *dst_pitch,
                 long   n)
{
  double Y[3];
  babl_get_Y (Y);

  BABL_PLANAR_SANITY
  while (n--)
    {
      double red, green, blue;
      double luminance, alpha;

      red   = *(double *) src[0];
      green = *(double *) src[1];
      blue  = *(double *) src[2];
      if (src_bands > 3)
        alpha = *(double *) src[3];
      else
        alpha = 1.0;

      luminance = red * Y[0] +
                  green * Y[1] +
                  blue * Y[2];
      *(double *) dst[0] = luminance;

      if (dst_bands == 2)
        *(double *) dst[1] = alpha;

      BABL_PLANAR_STEP
    }
  return n;
}*/


/*static long
gray_2_2_to_rgb (int    src_bands,
                 char **src,
                 int   *src_pitch,
                 int    dst_bands,
                 char **dst,
                 int   *dst_pitch,
                 long   n)
{
  BABL_PLANAR_SANITY
  while (n--)
    {
      double luminance;
      double red, green, blue;
      double alpha;

      luminance = *(double *) src[0];
      red       = luminance;
      green     = luminance;
      blue      = luminance;
      if (src_bands > 1)
        alpha = *(double *) src[1];
      else
        alpha = 1.0;

      *(double *) dst[0] = red;
      *(double *) dst[1] = green;
      *(double *) dst[2] = blue;

      if (dst_bands > 3)
        *(double *) dst[3] = alpha;

      BABL_PLANAR_STEP
    }
  return n;
}*/



static long
graya_to_rgba (char *src,
               char *dst,
               long  n)
{
  while (n--)
    {
      double luminance;
      double red, green, blue;
      double alpha;

      luminance = ((double *) src)[0];
      alpha     = ((double *) src)[1];
      red       = luminance;
      green     = luminance;
      blue      = luminance;

      ((double *) dst)[0] = red;
      ((double *) dst)[1] = green;
      ((double *) dst)[2] = blue;
      ((double *) dst)[3] = alpha;

      src += sizeof (double) * 2;
      dst += sizeof (double) * 4;
    }
  return n;
}


static long
gray_to_rgba (char *src,
              char *dst,
              long  n)
{
  double Y[3];
  babl_get_Y (Y);

  while (n--)
    {
      double luminance;
      double red, green, blue;

      luminance = ((double *) src)[0];
      red       = luminance;
      green     = luminance;
      blue      = luminance;

      ((double *) dst)[0] = red;
      ((double *) dst)[1] = green;
      ((double *) dst)[2] = blue;
      ((double *) dst)[3] = 1.0;

      src += sizeof (double) * 1;
      dst += sizeof (double) * 4;
    }
  return n;
}

static long
gray_alpha_premultiplied_to_rgba (int    src_bands,
                                  char **src,
                                  int   *src_pitch,
                                  int    dst_bands,
                                  char **dst,
                                  int   *dst_pitch,
                                  long   n)
{
  BABL_PLANAR_SANITY
  assert (src_bands == 2);
  assert (dst_bands == 4);

  while (n--)
    {
      double luminance = *(double *) src[0];
      double alpha     = *(double *) src[1];

      if (alpha > BABL_ALPHA_THRESHOLD)
        {
          luminance = luminance / alpha;
        }
      else
        {
          luminance = 0.0;
        }

      *(double *) dst[0] = luminance;
      *(double *) dst[1] = luminance;
      *(double *) dst[2] = luminance;
      *(double *) dst[3] = alpha;
      BABL_PLANAR_STEP
    }
  return n;
}


static long
rgba_to_gray_alpha_premultiplied (int    src_bands,
                                  char **src,
                                  int   *src_pitch,
                                  int    dst_bands,
                                  char **dst,
                                  int   *dst_pitch,
                                  long   n)
{
  double Y[3];
  babl_get_Y (Y);

  BABL_PLANAR_SANITY;
  assert (src_bands == 4);
  assert (dst_bands == 2);

  while (n--)
    {
      double red   = *(double *) src[0];
      double green = *(double *) src[1];
      double blue  = *(double *) src[2];
      double alpha = *(double *) src[3];
      double luminance;

      luminance = red * Y[0] +
                  green * Y[1] +
                  blue * Y[2];

      luminance *= alpha;

      *(double *) dst[0] = luminance;
      *(double *) dst[1] = alpha;
      BABL_PLANAR_STEP
    }
  return n;
}

static long
non_premultiplied_to_premultiplied (int    src_bands,
                                    char **src,
                                    int   *src_pitch,
                                    int    dst_bands,
                                    char **dst,
                                    int   *dst_pitch,
                                    long   n)
{
  BABL_PLANAR_SANITY

  while (n--)
    {
      double alpha;
      int    band;

      alpha = *(double *) src[src_bands - 1];
      for (band = 0; band < src_bands - 1; band++)
        {
          *(double *) dst[band] = *(double *) src[band] * alpha;
        }
      *(double *) dst[dst_bands - 1] = alpha;

      BABL_PLANAR_STEP
    }
  return n;
}

static long
premultiplied_to_non_premultiplied (int    src_bands,
                                    char **src,
                                    int   *src_pitch,
                                    int    dst_bands,
                                    char **dst,
                                    int   *dst_pitch,
                                    long   n)
{
  BABL_PLANAR_SANITY

  while (n--)
    {
      double alpha;
      int    band;

      alpha = *(double *) src[src_bands - 1];
      for (band = 0; band < src_bands - 1; band++)
        {
          if (alpha > BABL_ALPHA_THRESHOLD)
            {
              *(double *) dst[band] = *(double *) src[band] / alpha;
            }
          else
            {
              *(double *) dst[band] = 0.0;
            }
        }
      *(double *) dst[dst_bands - 1] = alpha;

      BABL_PLANAR_STEP
    }
  return n;
}

static void
conversions (void)
{
  babl_conversion_new (
    babl_model_from_id (BABL_GRAY),
    babl_model_from_id (BABL_RGBA),
    "linear", gray_to_rgba,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_GRAY_ALPHA),
    babl_model_from_id (BABL_RGBA),
    "linear", graya_to_rgba,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_RGBA),
    babl_model_from_id (BABL_GRAY_ALPHA),
    "linear", rgba_to_graya,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_RGBA),
    babl_model_from_id (BABL_GRAY),
    "linear", rgba_to_gray,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_GRAY_ALPHA),
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    "planar", non_premultiplied_to_premultiplied,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    babl_model_from_id (BABL_GRAY_ALPHA),
    "planar", premultiplied_to_non_premultiplied,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    babl_model_from_id (BABL_RGBA),
    "planar", gray_alpha_premultiplied_to_rgba,
    NULL
  );

  babl_conversion_new (
    babl_model_from_id (BABL_RGBA),
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    "planar", rgba_to_gray_alpha_premultiplied,
    NULL
  );
}

static void
formats (void)
{
  babl_format_new (
    babl_model_from_id (BABL_GRAY_ALPHA),
    babl_type_from_id (BABL_HALF),
    babl_component_from_id (BABL_LUMINANCE),
    babl_component_from_id (BABL_ALPHA),
    NULL);
  babl_format_new (
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    babl_type_from_id (BABL_HALF),
    babl_component_from_id (BABL_LUMINANCE_MUL_ALPHA),
    babl_component_from_id (BABL_ALPHA),
    NULL);
  babl_format_new (
    babl_model_from_id (BABL_GRAY),
    babl_type_from_id (BABL_HALF),
    babl_component_from_id (BABL_LUMINANCE),
    NULL);

  /***********/

  babl_format_new (
    babl_model_from_id (BABL_GRAY_ALPHA),
    babl_type ("u15"),
    babl_component_from_id (BABL_LUMINANCE),
    babl_component_from_id (BABL_ALPHA),
    NULL);
  babl_format_new (
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    babl_type ("u15"),
    babl_component_from_id (BABL_LUMINANCE_MUL_ALPHA),
    babl_component_from_id (BABL_ALPHA),
    NULL);
  babl_format_new (
    babl_model_from_id (BABL_GRAY),
    babl_type ("u15"),
    babl_component_from_id (BABL_LUMINANCE),
    NULL);

  babl_format_new (
    babl_model_from_id (BABL_GRAY_ALPHA),
    babl_type_from_id (BABL_U32),
    babl_component_from_id (BABL_LUMINANCE),
    babl_component_from_id (BABL_ALPHA),
    NULL);
  babl_format_new (
    babl_model_from_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    babl_type_from_id (BABL_U32),
    babl_component_from_id (BABL_LUMINANCE_MUL_ALPHA),
    babl_component_from_id (BABL_ALPHA),
    NULL);
  babl_format_new (
    babl_model_from_id (BABL_GRAY),
    babl_type_from_id (BABL_U32),
    babl_component_from_id (BABL_LUMINANCE),
    NULL);

}
