/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2005, Øyvind Kolås.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* FIXME: this file should be renamed model-gray.c */

#include "babl.h"
#include "util.h"
#include "rgb-constants.h"
#include "math.h"

static void components  (void);
static void models      (void);
static void conversions (void);

void babl_base_model_gray (void)
{
  components  ();
  models      ();
  conversions ();
}

static void
components (void)
{
  babl_component_new (
   "Y", 
   "id",    BABL_LUMINANCE,
   "luma",
   NULL);

  babl_component_new (
   "Y*A",
   "id",    BABL_LUMINANCE_MUL_ALPHA,
   "luma",
   NULL);

  babl_component_new (
   "Y-g2.2", 
   "id",    BABL_LUMINANCE_GAMMA_2_2,
   "luma",
   NULL);
}

static void
models (void)
{
  babl_model_new (
    "gray",
    "id", BABL_GRAY,
    babl_component_id (BABL_LUMINANCE),
    NULL);

  babl_model_new (
    "gray-g2.2",
    "id", BABL_GRAY_GAMMA_2_2,
    babl_component_id (BABL_LUMINANCE_GAMMA_2_2),
    NULL);

  babl_model_new (
    "graya-g2.2",
    "id", BABL_GRAY_GAMMA_2_2_ALPHA,
    babl_component_id (BABL_LUMINANCE_GAMMA_2_2),
    babl_component_id (BABL_ALPHA),
    NULL);

  babl_model_new (
    "graya",
    "id", BABL_GRAY_ALPHA,
    babl_component_id (BABL_LUMINANCE),
    babl_component_id (BABL_ALPHA),
    NULL);

  babl_model_new (
    "grayA",
    "id", BABL_GRAY_ALPHA_PREMULTIPLIED,
    babl_component_id (BABL_LUMINANCE_MUL_ALPHA),
    babl_component_id (BABL_ALPHA),
    NULL);

}


static void
rgb_to_gray (int    src_bands,
             void **src,
             int   *src_pitch,
             int    dst_bands,
             void **dst,
             int   *dst_pitch,
             int    n)
{
  BABL_PLANAR_SANITY
  while (n--)
    {
      double red, green, blue;
      double luminance, alpha;

      red   = *(double *)src[0];
      green = *(double *)src[1];
      blue  = *(double *)src[2];
      if (src_bands>3)
        alpha = *(double *)src[3];
      else
        alpha = 1.0;

      luminance  = red   * RGB_LUMINANCE_RED +
                   green * RGB_LUMINANCE_GREEN +
                   blue  * RGB_LUMINANCE_BLUE;
      *(double*)dst[0] = luminance;

      if (dst_bands==2)
        *(double*)dst[1] = alpha;

      BABL_PLANAR_STEP
    }
}


static void
rgb_to_gray_2_2 (int    src_bands,
                 void **src,
                 int   *src_pitch,
                 int    dst_bands,
                 void **dst,
                 int   *dst_pitch,
                 int    n)
{
  BABL_PLANAR_SANITY
  while (n--)
    {
      double red, green, blue;
      double luminance, alpha;

      red   = *(double *) src[0];
      green = *(double *) src[1];
      blue  = *(double *) src[2];
      if (src_bands>3)
        alpha = *(double *)src[3];
      else
        alpha = 1.0;

      luminance  = red   * RGB_LUMINANCE_RED +
                   green * RGB_LUMINANCE_GREEN +
                   blue  * RGB_LUMINANCE_BLUE;
      *(double*)dst[0] = linear_to_gamma_2_2 (luminance);

      if (dst_bands==2)
        *(double*)dst[1] = alpha;

      BABL_PLANAR_STEP
    }
}


static void
gray_2_2_to_rgb (int    src_bands,
                 void **src,
                 int   *src_pitch,
                 int    dst_bands,
                 void **dst,
                 int   *dst_pitch,
                 int    n)
{
  BABL_PLANAR_SANITY
  while (n--)
    {
      double luminance;
      double red, green, blue;
      double alpha;

      luminance = gamma_2_2_to_linear (*(double *)src[0]);
      red       = luminance;
      green     = luminance;
      blue      = luminance;
      if (src_bands > 1)
        alpha = *(double *)src[1];
      else
        alpha     = 1.0;

      *(double*)dst[0] = red;
      *(double*)dst[1] = green;
      *(double*)dst[2] = blue;

      if (dst_bands>3)
        *(double*)dst[3] = alpha;

      BABL_PLANAR_STEP
    }
}



static void
gray_to_rgb (int    src_bands,
             void **src,
             int   *src_pitch,
             int    dst_bands,
             void **dst,
             int   *dst_pitch,
             int    n)
{
  BABL_PLANAR_SANITY
  while (n--)
    {
      double luminance;
      double red, green, blue;
      double alpha;

      luminance = *(double *)src[0];
      red       = luminance;
      green     = luminance;
      blue      = luminance;
      if (src_bands > 1)
        alpha = *(double *)src[1];
      else
        alpha     = 1.0;

      *(double*)dst[0] = red;
      *(double*)dst[1] = green;
      *(double*)dst[2] = blue;

      if (dst_bands>3)
        *(double*)dst[3] = alpha;

      BABL_PLANAR_STEP
    }
}

static void
gray_alpha_premultiplied_to_rgba (int    src_bands,
                                  void **src,
                                  int   *src_pitch,
                                  int    dst_bands,
                                  void **dst,
                                  int   *dst_pitch,
                                  int    n)
{
  BABL_PLANAR_SANITY
  assert (src_bands == 2);
  assert (dst_bands == 4);

  while (n--)
    {
      double luminance = *(double *)src[0];
      double alpha     = *(double *)src[1];

      if (alpha > 0.001)
        {
          luminance = luminance / alpha;
        }
      else
        {
          luminance = 0.0;
        }
      
      *(double*)dst[0] = luminance;
      *(double*)dst[1] = luminance;
      *(double*)dst[2] = luminance;
      *(double*)dst[3] = alpha;
      BABL_PLANAR_STEP
    }
}


static void
rgba_to_gray_alpha_premultiplied (int    src_bands,
                                  void **src,
                                  int   *src_pitch,
                                  int    dst_bands,
                                  void **dst,
                                  int   *dst_pitch,
                                  int    n)
{
  BABL_PLANAR_SANITY;
  assert (src_bands == 4);
  assert (dst_bands == 2);

  while (n--)
    {
      double red       = *(double *)src[0];
      double green     = *(double *)src[1];
      double blue      = *(double *)src[2];
      double alpha     = *(double *)src[3];
      double luminance;

      luminance  = red   * RGB_LUMINANCE_RED +
                   green * RGB_LUMINANCE_GREEN +
                   blue  * RGB_LUMINANCE_BLUE;

      luminance *= alpha;
      
      *(double*)dst[0] = luminance;
      *(double*)dst[2] = alpha;
      BABL_PLANAR_STEP
    }
}

static void
non_premultiplied_to_premultiplied (int    src_bands,
                                    void **src,
                                    int   *src_pitch,
                                    int    dst_bands,
                                    void **dst,
                                    int   *dst_pitch,
                                    int    n)
{
  BABL_PLANAR_SANITY

  while (n--)
    {
      double alpha;
      int band;

      alpha = *(double *)src[src_bands-1];
      for (band=0; band<src_bands-1;band++)
        {
          *(double*)dst[band] = *(double*) src[band] * alpha;
        }
      *(double*)dst[dst_bands-1] = alpha;

      BABL_PLANAR_STEP
    }
}

static void
premultiplied_to_non_premultiplied (int    src_bands,
                                    void **src,
                                    int   *src_pitch,
                                    int    dst_bands,
                                    void **dst,
                                    int   *dst_pitch,
                                    int    n)
{
  BABL_PLANAR_SANITY

  while (n--)
    {
      double alpha;
      int band;

      alpha = *(double *)src[src_bands-1];
      for (band=0; band<src_bands-1;band++)
        {
          if (alpha>0.001)
            {
              *(double*)dst[band] = *(double*) src[band] / alpha;
            }
          else
            {
              *(double*)dst[band] = 0.001;
            }
        }
      *(double*)dst[dst_bands-1] = alpha;

      BABL_PLANAR_STEP
    }
}

static void
conversions (void)
{
  babl_conversion_new (
    "babl-base: gray-g2.2 to rgba",
    "source",      babl_model_id (BABL_GRAY_GAMMA_2_2),
    "destination", babl_model_id (BABL_RGBA),
    "planar",      gray_2_2_to_rgb,
    NULL
  );


  babl_conversion_new (
    "babl-base: rgba to gray-g2.2",
    "source",      babl_model_id (BABL_RGBA),
    "destination", babl_model_id (BABL_GRAY_GAMMA_2_2),
    "planar",      rgb_to_gray_2_2,
    NULL
  );

  babl_conversion_new (
    "babl-base: graya-g2.2 to rgba",
    "source",      babl_model_id (BABL_GRAY_GAMMA_2_2_ALPHA),
    "destination", babl_model_id (BABL_RGBA),
    "planar",      gray_2_2_to_rgb,
    NULL
  );

  babl_conversion_new (
    "babl-base: rgba to graya-g2.2",
    "source",      babl_model_id (BABL_RGBA),
    "destination", babl_model_id (BABL_GRAY_GAMMA_2_2_ALPHA),
    "planar",      rgb_to_gray_2_2,
    NULL
  );

  babl_conversion_new (
    "babl-base: gray to rgba",
    "source",      babl_model_id (BABL_GRAY),
    "destination", babl_model_id (BABL_RGBA),
    "planar",      gray_to_rgb,
    NULL
  );

  babl_conversion_new (
    "babl-base: gray to rgb",
    "source",      babl_model_id (BABL_GRAY),
    "destination", babl_model_id (BABL_RGB),
    "planar",      gray_to_rgb,
    NULL
  );

  babl_conversion_new (
    "babl-base: gray-alpha to rgba",
    "source",      babl_model_id (BABL_GRAY_ALPHA),
    "destination", babl_model_id (BABL_RGBA),
    "planar",      gray_to_rgb,
    NULL
  );

  babl_conversion_new (
    "babl-base: gray-alpha to rgb",
    "source",      babl_model_id (BABL_GRAY_ALPHA),
    "destination", babl_model_id (BABL_RGB),
    "planar",      gray_to_rgb,
    NULL
  );

  babl_conversion_new (
    "babl-base: rgba to gray-alpha",
    "source",      babl_model_id (BABL_RGBA),
    "destination", babl_model_id (BABL_GRAY_ALPHA),
    "planar",      rgb_to_gray,
    NULL
  );

  babl_conversion_new (
    "babl-base: rgba to gray",
    "source",      babl_model_id (BABL_RGBA),
    "destination", babl_model_id (BABL_GRAY),
    "planar",      rgb_to_gray,
    NULL
  );

  babl_conversion_new (
    "babl-base: rgb to gray-alpha",
    "source",      babl_model_id (BABL_RGB),
    "destination", babl_model_id (BABL_GRAY_ALPHA),
    "planar",      rgb_to_gray,
    NULL
  );

  babl_conversion_new (
    "babl-base: rgb to gray",
    "source",      babl_model_id (BABL_RGB),
    "destination", babl_model_id (BABL_GRAY),
    "planar",      rgb_to_gray,
    NULL
  );

  babl_conversion_new (
    "babl-base: gray-alpha to gray-alpha-premultiplied",
    "source",      babl_model_id (BABL_GRAY_ALPHA),
    "destination", babl_model_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    "planar",      non_premultiplied_to_premultiplied,
    NULL
  );

  babl_conversion_new (
    "babl-base: gray-alpha-premultuplied to gray-alpha",
    "source",      babl_model_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    "destination", babl_model_id (BABL_GRAY_ALPHA),
    "planar",      premultiplied_to_non_premultiplied,
    NULL
  );

    babl_conversion_new (
    "babl-base: gray-alpha-premultiplied to rgba",
    "source",      babl_model_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    "destination", babl_model_id (BABL_RGBA),
    "planar",      gray_alpha_premultiplied_to_rgba,
    NULL
  );

  babl_conversion_new (
    "babl-base: rgba to gray-alpha-premultiplied",
    "source",      babl_model_id (BABL_RGBA),
    "destination", babl_model_id (BABL_GRAY_ALPHA_PREMULTIPLIED),
    "planar",      rgba_to_gray_alpha_premultiplied,
    NULL
  );
}