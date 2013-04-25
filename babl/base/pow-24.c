/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2013 Loren Merritt
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

#include <math.h>
#include "util.h"

/* a^b = exp(b*log(a))
 *
 * Extracting the exponent from a float gives us an approximate log.
 * Or better yet, reinterpret the bitpattern of the whole float as an int.
 *
 * However, the output values of 12throot vary by less than a factor of 2
 * over the domain we care about, so we only get log() that way, not exp().
 *
 * Approximate exp() with a low-degree polynomial; not exactly equal to the
 * Taylor series since we're minimizing maximum error over a certain finite
 * domain. It's not worthwhile to use lots of terms, since Newton's method
 * has a better convergence rate once you get reasonably close to the answer.
 */
static inline double
init_newton (double x, double exponent, double c0, double c1, double c2)
{
    int iexp;
    double y = frexp(x, &iexp);
    y = 2*y+(iexp-2);
    c1 *= M_LN2*exponent;
    c2 *= M_LN2*M_LN2*exponent*exponent;
    return y = c0 + c1*y + c2*y*y;
}

/* Returns x^2.4 == (x*(x^(-1/5)))^3, using Newton's method for x^(-1/5).
 * Max absolute error is 4e-8.
 * One more iteration of Newton would bring error down to 4e-15.
 *
 * (The above measurements apply to the input range can that be involved in
 * gamma correction. Accuracy for inputs very close to 0 is somewhat worse,
 * but that will hit the linear part of the gamma curve rather than call these.
 * Out-of-range pixels >1.3 are also somewhat worse.)
 */
double
babl_pow_24 (double x)
{
  double y = init_newton (x, -1./5, 0.9953189663, 0.9594345146, 0.6742970332);
  int i;
  for (i = 0; i < 2; i++)
    y = (1.+1./5)*y - ((1./5)*x*(y*y))*((y*y)*(y*y));
  x *= y;
  return x*x*x;
}

/* Returns x^(1/2.4) == x*(x^(-1/12))^7, using Newton's method for x^(-1/12).
 * Max absolute error is 7e-8
 * One more iteration of Newton would bring error down to 4e-14.
 */
double
babl_pow_1_24 (double x)
{
  double y = init_newton (x, -1./12, 0.9976740658, 0.9885035151, 0.5907708377);
  int i;
  for (i = 0; i < 2; i++)
    {
      double z = (y*y)*(y*y);
      z = (y*z)*(z*z);
      y = (1.+1./12)*y - (1./12)*x*z;
    }
  return ((x*y)*(y*y))*((y*y)*(y*y));
}
