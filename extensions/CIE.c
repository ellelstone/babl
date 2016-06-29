/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2005, 2014 Øyvind Kolås.
 * Copyright (C) 2009, Martin Nordholts
 * Copyright (C) 2014, Elle Stone
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
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "babl/babl.h"
#include "util.h"
#include "extensions/util.h"

/*******begin James Gregson's matrix code *****************************/

/**
 @file solve_2x2_3x3.h
 http://jamesgregson.blogspot.com/2012/09/2x2-and-3x3-matrix-inverses-and-linear.html
 @author James Gregson (james.gregson@gmail.com)
 @brief 2x2 and 3x3 matrix inverses and linear system solvers. Free for all use. Best efforts made at testing although I give no guarantee of correctness. Although not required, please leave this notice intact and pass along any bug-fixes/reports to the email address above.  See the functions test_2x2() and test_3x3() for usage details.
*/

/**
* Elle Stone modified the solve_2x2_3x3.h code to change "const double"
* to "double" for the input matrices. This probably wasn't required as
* the problems I was having compiling the code seem to have been
* related to problems with include files and/or linking issues.
*
* Elle Stone also removed the test_2x2 and test_3x3 matrix functions
* because they use drand, which doesn't compile under Windows.
*/

#ifndef SOLVE_2X2_3X3
#define SOLVE_2x2_3X3

#define SOLVE_SUCCESS 0
#define SOLVE_FAILURE 1
#define SOLVE_EPSILON 1e-8

/**
 @brief Compute the inverse of a 2x2 matrix A and store it in iA, returns SOLVE_FAILURE on failure.
 @param[in] A Input matrix, indexed by row then column, i.e. A[row][column]
 @param[out] iA Output matrix that is the inverse of A, provided a is definite
 @return SOLVE_SUCCESS on success, SOLVE_FAILURE on failure
 */
int invert_2x2( double A[2][2], double iA[2][2] );

/**
 @brief Solves a 2x2 system Ax=b
 @param[in] A input 2x2 matrix
 @param[out] x output solution vector
 @param[in] b input right-hand-side
 @return SOLVE_SUCCESS on success, SOLVE_FAILURE on failure
 */
int solve_2x2( double A[2][2], double x[2], double b[2] );

/**
 @brief Computes the squared residual of a solution x for a linear system Ax=b, for testing purposes.
 @param[in] A input matrix
 @param[in] x input solution vector
 @param[in] b input right-hand-side
 @return squared 2-norm of residual vector
 */
double residual_2x2( double A[2][2], double x[2], double b[2] );

/**
 @brief Compute the inverse of a 3x3 matrix A and store it in iA, returns SOLVE_FAILURE on failure.
 @param[in] A Input matrix, indexed by row then column, i.e. A[row][column]
 @param[out] iA Output matrix that is the inverse of A, provided a is definite
 @return SOLVE_SUCCESS on success, SOLVE_FAILURE on failure
 */
int invert_3x3( double A[3][3], double iA[3][3] );

/**
 @brief Solves a 3x3 system Ax=b
 @param[in] A input 3x3 matrix
 @param[out] x output solution vector
 @param[in] b input right-hand-side
 @return SOLVE_SUCCESS on success, SOLVE_FAILURE on failure
 */
int solve_3x3( double A[3][3], double x[3], double b[3] );

/**
 @brief Computes the squared residual of a solution x for a linear system Ax=b, for testing purposes.
 @param[in] A input matrix
 @param[in] x input solution vector
 @param[in] b input right-hand-side
 @return squared 2-norm of residual vector
 */
double residual_3x3( double A[3][3], double x[3], double b[3] );


#endif

/**
 @file solve_2x2_3x3.c
 @author James Gregson (james.gregson@gmail.com)
 http://jamesgregson.blogspot.com/2012/09/2x2-and-3x3-matrix-inverses-and-linear.html
 @brief Implementation for 2x2 and 3x3 matrix inverses and linear system solution. See solve_2x2_3x3.h for details.
*/

int invert_2x2( double A[2][2], double iA[2][2] ){
 double det;
 det = A[0][0]*A[1][1] - A[0][1]*A[1][0];
 if( fabs(det) < SOLVE_EPSILON )
  return SOLVE_FAILURE;

 iA[0][0] =  A[1][1]/det; iA[0][1] = -A[0][1]/det;
 iA[1][0] = -A[1][0]/det; iA[1][1] =  A[0][0]/det;

 return SOLVE_SUCCESS;
}

int solve_2x2( double A[2][2], double x[2], double b[2] ){
 double iA[2][2];

 if( invert_2x2( A, iA ) )
  return SOLVE_FAILURE;

 x[0] = iA[0][0]*b[0] + iA[0][1]*b[1];
 x[1] = iA[1][0]*b[0] + iA[1][1]*b[1];

 return SOLVE_SUCCESS;
}

double residual_2x2( double A[2][2], double x[2], double b[2] ){
 double r[2];
 r[0] = A[0][0]*x[0] + A[0][1]*x[1] - b[0];
 r[1] = A[1][0]*x[0] + A[1][1]*x[1] - b[1];
 return r[0]*r[0] + r[1]*r[1];
}

int invert_3x3( double A[3][3], double iA[3][3] ){
 double det;

 det = A[0][0]*(A[2][2]*A[1][1]-A[2][1]*A[1][2])
 - A[1][0]*(A[2][2]*A[0][1]-A[2][1]*A[0][2])
 + A[2][0]*(A[1][2]*A[0][1]-A[1][1]*A[0][2]);
 if( fabs(det) < SOLVE_EPSILON )
  return SOLVE_FAILURE;

 iA[0][0] =  (A[2][2]*A[1][1]-A[2][1]*A[1][2])/det;
 iA[0][1] = -(A[2][2]*A[0][1]-A[2][1]*A[0][2])/det;
 iA[0][2] =  (A[1][2]*A[0][1]-A[1][1]*A[0][2])/det;

 iA[1][0] = -(A[2][2]*A[1][0]-A[2][0]*A[1][2])/det;
 iA[1][1] =  (A[2][2]*A[0][0]-A[2][0]*A[0][2])/det;
 iA[1][2] = -(A[1][2]*A[0][0]-A[1][0]*A[0][2])/det;

 iA[2][0] =  (A[2][1]*A[1][0]-A[2][0]*A[1][1])/det;
 iA[2][1] = -(A[2][1]*A[0][0]-A[2][0]*A[0][1])/det;
 iA[2][2] =  (A[1][1]*A[0][0]-A[1][0]*A[0][1])/det;

 return SOLVE_SUCCESS;
}

int solve_3x3( double A[3][3], double x[3], double b[3] ){
 double iA[3][3];

 if( invert_3x3( A, iA ) )
  return SOLVE_FAILURE;

 x[0] = iA[0][0]*b[0] + iA[0][1]*b[1] + iA[0][2]*b[2];
 x[1] = iA[1][0]*b[0] + iA[1][1]*b[1] + iA[1][2]*b[2];
 x[2] = iA[2][0]*b[0] + iA[2][1]*b[1] + iA[2][2]*b[2];

 return SOLVE_SUCCESS;
}

double residual_3x3( double A[3][3], double x[3], double b[3] ){
 double r[3];
 r[0] = A[0][0]*x[0] + A[0][1]*x[1] + A[0][2]*x[2] - b[0];
 r[1] = A[1][0]*x[0] + A[1][1]*x[1] + A[1][2]*x[2] - b[1];
 r[2] = A[2][0]*x[0] + A[2][1]*x[1] + A[2][2]*x[2] - b[2];
 return r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
}

/*********** end James Gregson's matrix code **************************/

#define DEGREES_PER_RADIAN (180 / 3.14159265358979323846)
#define RADIANS_PER_DEGREE (1 / DEGREES_PER_RADIAN)

#define LAB_EPSILON       (216.0 / 24389.0)
#define LAB_KAPPA         (24389.0 / 27.0)

/* The constants below hard-code the D50-adapted sRGB ICC profile
 * reference white, aka the ICC profile D50 illuminant.
 *
 * In a properly ICC profile color-managed application, the profile
 * illuminant values should be retrieved from the image's
 * ICC profile's illuminant.
 *
 * At present, the ICC profile illuminant is always D50. This might
 * change when the next version of the ICC specs is released.
 *
 * As encoded in an actual V2 or V4 ICC profile,
 * the illuminant values are hexadecimal-rounded, as are the following
 * hard-coded D50 ICC profile illuminant values:
 */

#define D50_WHITE_REF_X   0.964202880
#define D50_WHITE_REF_Y   1.000000000
#define D50_WHITE_REF_Z   0.824905400

static double babl_get_colorants (double colorants[3][3]);

static double babl_get_inverse_colorants (double inverse_colorants[3][3]);

static double babl_get_colorants (double colorants[3][3])
{
  double *new_colorant_data = babl_get_user_data (colorant_babl);

  colorants[0][0] = new_colorant_data[0];
  colorants[0][1] = new_colorant_data[3];
  colorants[0][2] = new_colorant_data[6];

  colorants[1][0] = new_colorant_data[1];
  colorants[1][1] = new_colorant_data[4];
  colorants[1][2] = new_colorant_data[7];

  colorants[2][0] = new_colorant_data[2];
  colorants[2][1] = new_colorant_data[5];
  colorants[2][2] = new_colorant_data[8];

  return colorants[3][3];
}

static double babl_get_inverse_colorants (double inverse_colorants[3][3])
{
  double colorants[3][3];
  babl_get_colorants (colorants);

  invert_3x3( colorants, inverse_colorants );

  return inverse_colorants[3][3];
}


static void types (void);
static void components (void);
static void models (void);
static void conversions (void);
static void formats (void);

int init (void);

int init (void)
{
  types ();
  components ();
  models ();
  formats ();
  conversions ();
  return 0;
}

static void  rgbcie_init (void);

static void
rgbxyzrgb_init (void)
{
}

static void
rgbcie_init (void)
{
  static int initialized = 0;

  if (!initialized)
    {
      rgbxyzrgb_init ();
      initialized = 1;
    }
}

static void
components (void)
{
  babl_component_new ("CIE L", NULL);
  babl_component_new ("CIE a", "chroma", NULL);
  babl_component_new ("CIE b", "chroma", NULL);
  babl_component_new ("CIE C(ab)", "chroma", NULL);
  babl_component_new ("CIE H(ab)", "chroma", NULL);
}

static void
models (void)
{
  babl_model_new (
    "name", "CIE Lab",
    babl_component ("CIE L"),
    babl_component ("CIE a"),
    babl_component ("CIE b"),
    NULL);

  babl_model_new (
    "name", "CIE Lab alpha",
    babl_component ("CIE L"),
    babl_component ("CIE a"),
    babl_component ("CIE b"),
    babl_component ("A"),
    NULL);

  babl_model_new (
    "name", "CIE LCH(ab)",
    babl_component ("CIE L"),
    babl_component ("CIE C(ab)"),
    babl_component ("CIE H(ab)"),
    NULL);

  babl_model_new (
    "name", "CIE LCH(ab) alpha",
    babl_component ("CIE L"),
    babl_component ("CIE C(ab)"),
    babl_component ("CIE H(ab)"),
    babl_component ("A"),
    NULL);
}


/******** begin double RGB/CIE color space conversions ****************/

static inline void  ab_to_CHab    (double  a,
                                   double  b,
                                   double *to_C,
                                   double *to_H);

static inline void  CHab_to_ab    (double  C,
                                   double  H,
                                   double *to_a,
                                   double *to_b);
                                   
static inline void RGB_to_XYZ     (double R,
                                   double G,
                                   double B,
                                   double *to_X,
                                   double *to_Y,
                                   double *to_Z);

static inline void XYZ_to_LAB     (double X,
                                   double Y,
                                   double Z,
                                   double *to_L,
                                   double *to_a,
                                   double *to_b
                                   );

static inline void LAB_to_XYZ     (double L,
                                   double a,
                                   double b,
                                   double *to_X,
                                   double *to_Y,
                                   double *to_Z
                                   );

static inline void XYZ_to_RGB     (double X,
                                   double Y,
                                   double Z,
                                   double *to_R,
                                   double *to_G,
                                   double *to_B);

static inline void
RGB_to_XYZ (double R,
            double G,
            double B,
            double *to_X,
            double *to_Y,
            double *to_Z)
{
  double colorants[3][3];
  babl_get_colorants (colorants);

  /* Convert RGB to XYZ */
  *to_X = colorants[0][0]*R
        + colorants[0][1]*G
        + colorants[0][2]*B;

  *to_Y = colorants[1][0]*R
        + colorants[1][1]*G
        + colorants[1][2]*B;

  *to_Z = colorants[2][0]*R
        + colorants[2][1]*G
        + colorants[2][2]*B;
}

static inline void
XYZ_to_RGB (double X,
            double Y,
            double Z,
            double *to_R,
            double *to_G,
            double *to_B)
{
  double inverse_colorants[3][3];
  babl_get_inverse_colorants (inverse_colorants);
  /* Convert XYZ to RGB */
  *to_R = inverse_colorants[0][0]*X
        + inverse_colorants[0][1]*Y
        + inverse_colorants[0][2]*Z;

  *to_G = inverse_colorants[1][0]*X
        + inverse_colorants[1][1]*Y
        + inverse_colorants[1][2]*Z;

  *to_B = inverse_colorants[2][0]*X
        + inverse_colorants[2][1]*Y
        + inverse_colorants[2][2]*Z;
}

static inline void
XYZ_to_LAB (double X,
            double Y,
            double Z,
            double *to_L,
            double *to_a,
            double *to_b)
{
  double f_x, f_y, f_z;

  double x_r = X / D50_WHITE_REF_X;
  double y_r = Y / D50_WHITE_REF_Y;
  double z_r = Z / D50_WHITE_REF_Z;

  if (x_r > LAB_EPSILON) f_x = pow(x_r, 1.0 / 3.0);
  else ( f_x = ((LAB_KAPPA * x_r) + 16) / 116.0 );

  if (y_r > LAB_EPSILON) f_y = pow(y_r, 1.0 / 3.0);
  else ( f_y = ((LAB_KAPPA * y_r) + 16) / 116.0 );

  if (z_r > LAB_EPSILON) f_z = pow(z_r, 1.0 / 3.0);
  else ( f_z = ((LAB_KAPPA * z_r) + 16) / 116.0 );

  *to_L = (116.0 * f_y) - 16.0;
  *to_a = 500.0 * (f_x - f_y);
  *to_b = 200.0 * (f_y - f_z);
}

static inline void
LAB_to_XYZ (double L,
            double a,
            double b,
            double *to_X,
            double *to_Y,
            double *to_Z)
{
  double fy, fx, fz, fx_cubed, fy_cubed, fz_cubed;
  double xr, yr, zr;

  fy = (L + 16.0) / 116.0;
  fy_cubed = fy*fy*fy;

  fz = fy - (b / 200.0);
  fz_cubed = fz*fz*fz;

  fx = (a / 500.0) + fy;
  fx_cubed = fx*fx*fx;

  if (fx_cubed > LAB_EPSILON) xr = fx_cubed;
  else xr = ((116.0 * fx) - 16) / LAB_KAPPA;

  if ( L > (LAB_KAPPA * LAB_EPSILON) ) yr = fy_cubed;
  else yr = (L / LAB_KAPPA);

  if (fz_cubed > LAB_EPSILON) zr = fz_cubed;
  else zr = ( (116.0 * fz) - 16 ) / LAB_KAPPA;

  *to_X = xr * D50_WHITE_REF_X;
  *to_Y = yr * D50_WHITE_REF_Y;
  *to_Z = zr * D50_WHITE_REF_Z;
}

static long
rgba_to_lab (char *src,
             char *dst,
             long  n)
{
  while (n--)
    {
      double R  = ((double *) src)[0];
      double G  = ((double *) src)[1];
      double B  = ((double *) src)[2];
      double X, Y, Z, L, a, b;
      
      //convert RGB to XYZ
      RGB_to_XYZ (R, G, B, &X, &Y, &Z);
      
      //convert XYZ to Lab
      XYZ_to_LAB (X, Y, Z, &L, &a, &b);

      ((double *) dst)[0] = L;
      ((double *) dst)[1] = a;
      ((double *) dst)[2] = b;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 3;
    }
  return n;
}

static long
lab_to_rgba (char *src,
             char *dst,
             long  n)
{
  while (n--)
    {
      double L = ((double *) src)[0];
      double a = ((double *) src)[1];
      double b = ((double *) src)[2];

      double X, Y, Z, R, G, B;
      
      //convert Lab to XYZ
      LAB_to_XYZ (L, a, b, &X, &Y, &Z);
      
      //convert XYZ to RGB
      XYZ_to_RGB (X, Y, Z, &R, &G, &B);
      ((double *) dst)[0] = R;
      ((double *) dst)[1] = G;
      ((double *) dst)[2] = B;
      ((double *) dst)[3] = 1.0;

      src += sizeof (double) * 3;
      dst += sizeof (double) * 4;
    }
  return n;
}

static long
rgba_to_laba (char *src,
              char *dst,
              long  n)
{
  while (n--)
    {
      double R     = ((double *) src)[0];
      double G     = ((double *) src)[1];
      double B     = ((double *) src)[2];
      double alpha = ((double *) src)[3];
      double X, Y, Z, L, a, b;
      
      //convert RGB to XYZ
      RGB_to_XYZ (R, G, B, &X, &Y, &Z);
      
      //convert XYZ to Lab
      XYZ_to_LAB (X, Y, Z, &L, &a, &b);

      ((double *) dst)[0] = L;
      ((double *) dst)[1] = a;
      ((double *) dst)[2] = b;
      ((double *) dst)[3] = alpha;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 4;
    }
  return n;
}

static long
laba_to_rgba (char *src,
              char *dst,
              long  n)
{
  while (n--)
    {
      double L     = ((double *) src)[0];
      double a     = ((double *) src)[1];
      double b     = ((double *) src)[2];
      double alpha = ((double *) src)[3];

      double X, Y, Z, R, G, B;
      
      //convert Lab to XYZ
      LAB_to_XYZ (L, a, b, &X, &Y, &Z);
      
      //convert XYZ to RGB
      XYZ_to_RGB (X, Y, Z, &R, &G, &B);
      ((double *) dst)[0] = R;
      ((double *) dst)[1] = G;
      ((double *) dst)[2] = B;
      ((double *) dst)[3] = alpha;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 4;
    }
  return n;
}

static inline void
CHab_to_ab (double  C,
            double  H,
            double *to_a,
            double *to_b)
{
  *to_a = cos (H * RADIANS_PER_DEGREE) * C;
  *to_b = sin (H * RADIANS_PER_DEGREE) * C;
}

static inline void
ab_to_CHab (double  a,
            double  b,
            double *to_C,
            double *to_H)
{
  *to_C = sqrt ( (a * a) + (b * b) );
  *to_H = atan2 (b, a) * DEGREES_PER_RADIAN;

  // Keep H within the range 0-360
  if (*to_H < 0.0)
      *to_H += 360;
}

static long
rgba_to_lchab (char *src,
               char *dst,
               long  n)
{
  while (n--)
    {
      double R = ((double *) src)[0];
      double G = ((double *) src)[1];
      double B = ((double *) src)[2];
      double X, Y, Z, L, a, b, C, H;

      //convert RGB to XYZ
      RGB_to_XYZ (R, G, B, &X, &Y, &Z);

      //convert XYZ to Lab
      XYZ_to_LAB (X, Y, Z, &L, &a, &b);

      //convert Lab to LCH(ab)
      ab_to_CHab (a, b, &C, &H);

      ((double *) dst)[0] = L;
      ((double *) dst)[1] = C;
      ((double *) dst)[2] = H;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 3;
    }
  return n;
}

static long
lchab_to_rgba (char *src,
               char *dst,
               long  n)
{
  while (n--)
    {
      double L = ((double *) src)[0];
      double C = ((double *) src)[1];
      double H = ((double *) src)[2];
      double a, b, X, Y, Z, R, G, B;

      //Convert LCH(ab) to Lab
      CHab_to_ab (C, H, &a, &b);

      //Convert LAB to XYZ
      LAB_to_XYZ (L, a, b, &X, &Y, &Z);

      //Convert XYZ to RGB
      XYZ_to_RGB (X, Y, Z, &R, &G, &B);

      ((double *) dst)[0] = R;
      ((double *) dst)[1] = G;
      ((double *) dst)[2] = B;
      ((double *) dst)[3] = 1.0;

      src += sizeof (double) * 3;
      dst += sizeof (double) * 4;
    }
  return n;
}


static long
rgba_to_lchaba (char *src,
                char *dst,
                long  n)
{
  while (n--)
    {
      double R = ((double *) src)[0];
      double G = ((double *) src)[1];
      double B = ((double *) src)[2];
      double alpha = ((double *) src)[3];
      double X, Y, Z, L, a, b, C, H;

      //convert RGB to XYZ
      RGB_to_XYZ (R, G, B, &X, &Y, &Z);

      //convert XYZ to Lab
      XYZ_to_LAB (X, Y, Z, &L, &a, &b);

      //convert Lab to LCH(ab)
      ab_to_CHab (a, b, &C, &H);

      ((double *) dst)[0] = L;
      ((double *) dst)[1] = C;
      ((double *) dst)[2] = H;
      ((double *) dst)[3] = alpha;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 4;
    }
  return n;
}

static long
lchaba_to_rgba (char *src,
                char *dst,
                long  n)
{
  while (n--)
    {
      double L     = ((double *) src)[0];
      double C     = ((double *) src)[1];
      double H     = ((double *) src)[2];
      double alpha = ((double *) src)[3];
      double a, b, X, Y, Z, R, G, B;

      //Convert LCH(ab) to Lab
      CHab_to_ab (C, H, &a, &b);

      //Convert Lab to XYZ
      LAB_to_XYZ (L, a, b, &X, &Y, &Z);

      //Convert XYZ to RGB
      XYZ_to_RGB (X, Y, Z, &R, &G, &B);

      ((double *) dst)[0] = R;
      ((double *) dst)[1] = G;
      ((double *) dst)[2] = B;
      ((double *) dst)[3] = alpha;

      src += sizeof (double) * 4;
      dst += sizeof (double) * 4;
    }
  return n;
}


/******** end double RGB/CIE color space conversions ******************/


/******** begin floating point RGB/CIE color space conversions ********/

static inline float
cubef (float f)
{
  return f * f * f;
}

static long
Yaf_to_Laf (float *src,
            float *dst,
            long   samples)
{
  long n = samples;

  while (n--)
    {
      float yr = src[0];
      float a  = src[1];
      float L  = yr > LAB_EPSILON ? 116.0 * cbrtf (yr) - 16 : LAB_KAPPA * yr;

      dst[0] = L;
      dst[1] = a;

      src += 2;
      dst += 2;
    }

  return samples;
}

static long
rgbf_to_Labf (float *src,
              float *dst,
              long   samples)
{
  long n = samples;
  double colorants[3][3];
  babl_get_colorants (colorants);

  while (n--)
    {
      float r = src[0];
      float g = src[1];
      float b = src[2];

      float xr = colorants[0][0] / D50_WHITE_REF_X * r
               + colorants[0][1] / D50_WHITE_REF_X * g
               + colorants[0][2] / D50_WHITE_REF_X * b;

      float yr = colorants[1][0]/ D50_WHITE_REF_Y * r
               + colorants[1][1] / D50_WHITE_REF_Y * g
               + colorants[1][2] / D50_WHITE_REF_Y * b;

      float zr = colorants[2][0] / D50_WHITE_REF_Z * r
               + colorants[2][1] / D50_WHITE_REF_Z * g
               + colorants[2][2] / D50_WHITE_REF_Z * b;

      float fx = xr > LAB_EPSILON ? cbrtf (xr) : (LAB_KAPPA * xr + 16.0f) / 116.0f;
      float fy = yr > LAB_EPSILON ? cbrtf (yr) : (LAB_KAPPA * yr + 16.0f) / 116.0f;
      float fz = zr > LAB_EPSILON ? cbrtf (zr) : (LAB_KAPPA * zr + 16.0f) / 116.0f;

      float L = 116.0f * fy - 16.0f;
      float A = 500.0f * (fx - fy);
      float B = 200.0f * (fy - fz);

      dst[0] = L;
      dst[1] = A;
      dst[2] = B;

      src += 3;
      dst += 3;
    }

  return samples;
}

static long
rgbaf_to_Labaf (float *src,
                float *dst,
                long   samples)
{
  long n = samples;
  double colorants[3][3];
  babl_get_colorants (colorants);

  while (n--)
    {
      float r = src[0];
      float g = src[1];
      float b = src[2];
      float a = src[3];

      float xr = colorants[0][0] / D50_WHITE_REF_X * r
               + colorants[0][1] / D50_WHITE_REF_X * g
               + colorants[0][2] / D50_WHITE_REF_X * b;

      float yr = colorants[1][0]/ D50_WHITE_REF_Y * r
               + colorants[1][1] / D50_WHITE_REF_Y * g
               + colorants[1][2] / D50_WHITE_REF_Y * b;

      float zr = colorants[2][0] / D50_WHITE_REF_Z * r
               + colorants[2][1] / D50_WHITE_REF_Z * g
               + colorants[2][2] / D50_WHITE_REF_Z * b;

      float fx = xr > LAB_EPSILON ? cbrtf (xr) : (LAB_KAPPA * xr + 16.0f) / 116.0f;
      float fy = yr > LAB_EPSILON ? cbrtf (yr) : (LAB_KAPPA * yr + 16.0f) / 116.0f;
      float fz = zr > LAB_EPSILON ? cbrtf (zr) : (LAB_KAPPA * zr + 16.0f) / 116.0f;

      float L = 116.0f * fy - 16.0f;
      float A = 500.0f * (fx - fy);
      float B = 200.0f * (fy - fz);

      dst[0] = L;
      dst[1] = A;
      dst[2] = B;
      dst[3] = a;

      src += 4;
      dst += 4;
    }

  return samples;
}

static long
Labf_to_rgbf (float *src,
                float *dst,
                long   samples)
{
  long n = samples;
  double inverse_colorants[3][3];
  babl_get_inverse_colorants (inverse_colorants);

  while (n--)
    {
      float L = src[0];
      float A = src[1];
      float B = src[2];

      float fy = (L + 16.0f) / 116.0f;
      float fx = fy + A / 500.0f;
      float fz = fy - B / 200.0f;

      float yr = L > LAB_KAPPA * LAB_EPSILON ? cubef (fy) : L / LAB_KAPPA;
      float xr = cubef (fx) > LAB_EPSILON ? cubef (fx) : (fx * 116.0f - 16.0f) / LAB_KAPPA;
      float zr = cubef (fz) > LAB_EPSILON ? cubef (fz) : (fz * 116.0f - 16.0f) / LAB_KAPPA;

      float r =  inverse_colorants[0][0] * D50_WHITE_REF_X * xr
               + inverse_colorants[0][1] * D50_WHITE_REF_Y * yr
               + inverse_colorants[0][2] * D50_WHITE_REF_Z * zr;

      float g =  inverse_colorants[1][0] * D50_WHITE_REF_X * xr
               + inverse_colorants[1][1] * D50_WHITE_REF_Y * yr
               + inverse_colorants[1][2] * D50_WHITE_REF_Z * zr;

      float b =  inverse_colorants[2][0] * D50_WHITE_REF_X * xr
               + inverse_colorants[2][1] * D50_WHITE_REF_Y * yr
               + inverse_colorants[2][2] * D50_WHITE_REF_Z * zr;

      dst[0] = r;
      dst[1] = g;
      dst[2] = b;

      src += 3;
      dst += 3;
    }

  return samples;
}

static long
Labaf_to_rgbaf (float *src,
                float *dst,
                long   samples)
{
  long n = samples;
  double inverse_colorants[3][3];
  babl_get_inverse_colorants (inverse_colorants);

  while (n--)
    {
      float L = src[0];
      float A = src[1];
      float B = src[2];
      float a = src[3];

      float fy = (L + 16.0f) / 116.0f;
      float fx = fy + A / 500.0f;
      float fz = fy - B / 200.0f;

      float yr = L > LAB_KAPPA * LAB_EPSILON ? cubef (fy) : L / LAB_KAPPA;
      float xr = cubef (fx) > LAB_EPSILON ? cubef (fx) : (fx * 116.0f - 16.0f) / LAB_KAPPA;
      float zr = cubef (fz) > LAB_EPSILON ? cubef (fz) : (fz * 116.0f - 16.0f) / LAB_KAPPA;

      float r =  inverse_colorants[0][0] * D50_WHITE_REF_X * xr
               + inverse_colorants[0][1] * D50_WHITE_REF_Y * yr
               + inverse_colorants[0][2] * D50_WHITE_REF_Z * zr;

      float g =  inverse_colorants[1][0] * D50_WHITE_REF_X * xr
               + inverse_colorants[1][1] * D50_WHITE_REF_Y * yr
               + inverse_colorants[1][2] * D50_WHITE_REF_Z * zr;

      float b =  inverse_colorants[2][0] * D50_WHITE_REF_X * xr
               + inverse_colorants[2][1] * D50_WHITE_REF_Y * yr
               + inverse_colorants[2][2] * D50_WHITE_REF_Z * zr;

      dst[0] = r;
      dst[1] = g;
      dst[2] = b;
      dst[3] = a;

      src += 4;
      dst += 4;
    }

  return samples;
}

static void
conversions (void)
{
  babl_conversion_new (
    babl_model ("RGBA"),
    babl_model ("CIE Lab"),
    "linear", rgba_to_lab,
    NULL
  );
  babl_conversion_new (
    babl_model ("CIE Lab"),
    babl_model ("RGBA"),
    "linear", lab_to_rgba,
    NULL
  );
  babl_conversion_new (
    babl_model ("RGBA"),
    babl_model ("CIE Lab alpha"),
    "linear", rgba_to_laba,
    NULL
  );
  babl_conversion_new (
    babl_model ("CIE Lab alpha"),
    babl_model ("RGBA"),
    "linear", laba_to_rgba,
    NULL
  );
  babl_conversion_new (
    babl_format ("RGB float"),
    babl_format ("CIE Lab float"),
    "linear", rgbf_to_Labf,
    NULL
  );
  babl_conversion_new (
    babl_format ("CIE Lab float"),
    babl_format ("RGB float"),
    "linear", Labf_to_rgbf,
    NULL
  );
  babl_conversion_new (
    babl_format ("RGBA float"),
    babl_format ("CIE Lab alpha float"),
    "linear", rgbaf_to_Labaf,
    NULL
  );
  babl_conversion_new (
    babl_format ("CIE Lab alpha float"),
    babl_format ("RGBA float"),
    "linear", Labaf_to_rgbaf,
    NULL
  );
  babl_conversion_new (
    babl_format ("YA float"),
    babl_format ("CIE L alpha float"),
    "linear", Yaf_to_Laf,
    NULL
  );
  babl_conversion_new (
    babl_model ("RGBA"),
    babl_model ("CIE LCH(ab)"),
    "linear", rgba_to_lchab,
    NULL
  );
  babl_conversion_new (
    babl_model ("CIE LCH(ab)"),
    babl_model ("RGBA"),
    "linear", lchab_to_rgba,
    NULL
  );
  babl_conversion_new (
    babl_model ("RGBA"),
    babl_model ("CIE LCH(ab) alpha"),
    "linear", rgba_to_lchaba,
    NULL
  );
  babl_conversion_new (
    babl_model ("CIE LCH(ab) alpha"),
    babl_model ("RGBA"),
    "linear", lchaba_to_rgba,
    NULL
  );

  rgbcie_init ();
}

static void
formats (void)
{
  babl_format_new (
    "name", "CIE Lab float",
    babl_model ("CIE Lab"),

    babl_type ("float"),
    babl_component ("CIE L"),
    babl_component ("CIE a"),
    babl_component ("CIE b"),
    NULL);

  babl_format_new (
    "name", "CIE Lab alpha float",
    babl_model ("CIE Lab alpha"),

    babl_type ("float"),
    babl_component ("CIE L"),
    babl_component ("CIE a"),
    babl_component ("CIE b"),
    babl_component ("A"),
    NULL);

  babl_format_new (
    "name", "CIE L alpha float",
    babl_model ("CIE Lab alpha"),
    babl_type ("float"),
    babl_component ("CIE L"),
    babl_component ("A"),
    NULL);

  babl_format_new (
    "name", "CIE Lab u8",
    babl_model ("CIE Lab"),

    babl_type ("CIE u8 L"),
    babl_component ("CIE L"),
    babl_type ("CIE u8 ab"),
    babl_component ("CIE a"),
    babl_type ("CIE u8 ab"),
    babl_component ("CIE b"),
    NULL);

  babl_format_new (
    "name", "CIE Lab u16",
    babl_model ("CIE Lab"),

    babl_type ("CIE u16 L"),
    babl_component ("CIE L"),
    babl_type ("CIE u16 ab"),
    babl_component ("CIE a"),
    babl_type ("CIE u16 ab"),
    babl_component ("CIE b"),
    NULL);

  babl_format_new (
    "name", "CIE LCH(ab) float",
    babl_model ("CIE LCH(ab)"),

    babl_type ("float"),
    babl_component ("CIE L"),
    babl_component ("CIE C(ab)"),
    babl_component ("CIE H(ab)"),
    NULL);

  babl_format_new (
    "name", "CIE LCH(ab) alpha float",
    babl_model ("CIE LCH(ab) alpha"),

    babl_type ("float"),
    babl_component ("CIE L"),
    babl_component ("CIE C(ab)"),
    babl_component ("CIE H(ab)"),
    babl_component ("A"),
    NULL);
}


/******** end floating point RGB/CIE color space conversions **********/


/******** begin  integer RGB/CIE color space conversions **************/

static inline long
convert_double_u8_scaled (double        min_val,
                          double        max_val,
                          unsigned char min,
                          unsigned char max,
                          char         *src,
                          char         *dst,
                          int           src_pitch,
                          int           dst_pitch,
                          long          n)
{
  while (n--)
    {
      double        dval = *(double *) src;
      unsigned char u8val;

      if (dval < min_val)
        u8val = min;
      else if (dval > max_val)
        u8val = max;
      else
        u8val = rint ((dval - min_val) / (max_val - min_val) * (max - min) + min);

      *(unsigned char *) dst = u8val;
      src                   += src_pitch;
      dst                   += dst_pitch;
    }
  return n;
}

static inline long
convert_u8_double_scaled (double        min_val,
                          double        max_val,
                          unsigned char min,
                          unsigned char max,
                          char         *src,
                          char         *dst,
                          int           src_pitch,
                          int           dst_pitch,
                          long          n)
{
  while (n--)
    {
      int    u8val = *(unsigned char *) src;
      double dval;

      if (u8val < min)
        dval = min_val;
      else if (u8val > max)
        dval = max_val;
      else
        dval = (u8val - min) / (double) (max - min) * (max_val - min_val) + min_val;

      (*(double *) dst) = dval;

      dst += dst_pitch;
      src += src_pitch;
    }
  return n;
}

#define MAKE_CONVERSIONS(name, min_val, max_val, min, max) \
  static long \
  convert_ ## name ## _double (char *src, \
                               char *dst, \
                               int src_pitch, \
                               int dst_pitch, \
                               long n)        \
  { \
    return convert_u8_double_scaled (min_val, max_val, min, max, \
                                     src, dst, src_pitch, dst_pitch, n); \
  }                                                               \
  static long \
  convert_double_ ## name (char *src, \
                           char *dst, \
                           int src_pitch, \
                           int dst_pitch, \
                           long n)        \
  { \
    return convert_double_u8_scaled (min_val, max_val, min, max, \
                                     src, dst, src_pitch, dst_pitch, n); \
  }

/* source ICC.1:2004-10 */

MAKE_CONVERSIONS (u8_l, 0.0, 100.0, 0x00, 0xff)
MAKE_CONVERSIONS (u8_ab, -128.0, 127.0, 0x00, 0xff)

#undef MAKE_CONVERSIONS

static void
types_u8 (void)
{
  babl_type_new (
    "CIE u8 L",
    "integer",
    "unsigned",
    "bits", 8,
    "min_val", 0.0,
    "max_val", 100.0,
    NULL
  );

  babl_type_new (
    "CIE u8 ab",
    "integer",
    "unsigned",
    "bits", 8,
    "min_val", -128.0,
    "max_val", 127.0,
    NULL
  );

  babl_conversion_new (
    babl_type ("CIE u8 L"),
    babl_type ("double"),
    "plane", convert_u8_l_double,
    NULL
  );
  babl_conversion_new (
    babl_type ("double"),
    babl_type ("CIE u8 L"),
    "plane", convert_double_u8_l,
    NULL
  );

  babl_conversion_new (
    babl_type ("CIE u8 ab"),
    babl_type ("double"),
    "plane", convert_u8_ab_double,
    NULL
  );
  babl_conversion_new (
    babl_type ("double"),
    babl_type ("CIE u8 ab"),
    "plane", convert_double_u8_ab,
    NULL
  );
}

static inline long
convert_double_u16_scaled (double         min_val,
                           double         max_val,
                           unsigned short min,
                           unsigned short max,
                           char          *src,
                           char          *dst,
                           int            src_pitch,
                           int            dst_pitch,
                           long           n)
{
  while (n--)
    {
      double         dval = *(double *) src;
      unsigned short u16val;

      if (dval < min_val)
        u16val = min;
      else if (dval > max_val)
        u16val = max;
      else
        u16val = rint ((dval - min_val) / (max_val - min_val) * (max - min) + min);

      *(unsigned short *) dst = u16val;
      dst                    += dst_pitch;
      src                    += src_pitch;
    }
  return n;
}

static inline long
convert_u16_double_scaled (double         min_val,
                           double         max_val,
                           unsigned short min,
                           unsigned short max,
                           char          *src,
                           char          *dst,
                           int            src_pitch,
                           int            dst_pitch,
                           long           n)
{
  while (n--)
    {
      int    u16val = *(unsigned short *) src;
      double dval;

      if (u16val < min)
        dval = min_val;
      else if (u16val > max)
        dval = max_val;
      else
        dval = (u16val - min) / (double) (max - min) * (max_val - min_val) + min_val;

      (*(double *) dst) = dval;
      dst              += dst_pitch;
      src              += src_pitch;
    }
  return n;
}

#define MAKE_CONVERSIONS(name, min_val, max_val, min, max)      \
  static long \
  convert_ ## name ## _double (char *src, \
                               char *dst, \
                               int src_pitch, \
                               int dst_pitch, \
                               long n)        \
  { \
    return convert_u16_double_scaled (min_val, max_val, min, max, \
                                      src, dst, src_pitch, dst_pitch, n); \
  }                                                               \
  static long \
  convert_double_ ## name (char *src, \
                           char *dst, \
                           int src_pitch, \
                           int dst_pitch, \
                           long n)        \
  { \
    return convert_double_u16_scaled (min_val, max_val, min, max, \
                                      src, dst, src_pitch, dst_pitch, n); \
  }

MAKE_CONVERSIONS (u16_l, 0.0, 100.0, 0x00, 0xffff)
MAKE_CONVERSIONS (u16_ab, -128.0, 127.0, 0x00, 0xffff)

#undef MAKE_CONVERSIONS

static void
types_u16 (void)
{
  babl_type_new (
    "CIE u16 L",
    "integer",
    "unsigned",
    "bits", 16,
    "min_val", 0.0,
    "max_val", 100.0,
    NULL
  );

  babl_type_new (
    "CIE u16 ab",
    "id", "CIE u8 ab",
    "integer",
    "unsigned",
    "bits", 16,
    "min_val", -128.0,
    "max_val", 127.0,
    NULL
  );


  babl_conversion_new (
    babl_type ("CIE u16 L"),
    babl_type ("double"),
    "plane", convert_u16_l_double,
    NULL
  );
  babl_conversion_new (
    babl_type ("double"),
    babl_type ("CIE u16 L"),
    "plane", convert_double_u16_l,
    NULL
  );

  babl_conversion_new (
    babl_type ("CIE u16 ab"),
    babl_type ("double"),
    "plane", convert_u16_ab_double,
    NULL
  );
  babl_conversion_new (
    babl_type ("double"),
    babl_type ("CIE u16 ab"),
    "plane", convert_double_u16_ab,
    NULL
  );
}

static void
types (void)
{
  types_u8 ();
  types_u16 ();
}

/******** end  integer RGB/CIE color space conversions ****************/
