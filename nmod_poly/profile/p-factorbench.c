/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Martin Lee

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <gmp.h>

#include "flint.h"
#include "nmod_poly.h"

int main (void)
{

    double t;
    nmod_poly_t f, g, h;
    for (int i= 15001;i < 16000; i++)
    {
      nmod_poly_init2 (f, 17, i/2+1);
      nmod_poly_init2 (g, 17, i+1);

      nmod_poly_set_coeff_ui (f, i/2, 1);
      nmod_poly_set_coeff_ui (f, 1, 1);
      nmod_poly_set_coeff_ui (f, 0, ((i%17)*(i%17)+3) % 17);

      nmod_poly_set_coeff_ui (g, i, 1);
      nmod_poly_set_coeff_ui (g, i/2+1, 1);
      nmod_poly_set_coeff_ui (g, 1, ((i % 17)+1)%17);
      nmod_poly_set_coeff_ui (g, 0, 15);

      nmod_poly_init (h, 17);
      nmod_poly_gcd (h, f, g);

      if (!nmod_poly_is_one (h))
      {
        printf ("i= %d\n", i);
        nmod_poly_factor_t factors;
        nmod_poly_factor_init (factors);
        t= clock();
        nmod_poly_factor (factors, h);
                    t = (clock() - t) / CLOCKS_PER_SEC;
                printf("factorization %.2lf\n", t);
        nmod_poly_factor_clear (factors);
      }

      nmod_poly_clear (f);
      nmod_poly_clear (g);
      nmod_poly_clear (h);
    }
    return EXIT_SUCCESS;
}
