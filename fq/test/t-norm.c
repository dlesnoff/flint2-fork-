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

    Copyright (C) 2012 Sebastian Pancratz 
    Copyright (C) 2012 Andres Goens

******************************************************************************/

#include <stdio.h>

#include "fq.h"
#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("norm... ");
    fflush(stdout);

    flint_randinit(state);

    /* Compare with product of Galois conjugates */
    for (i = 0; i < 2000; i++)
    {
        fq_ctx_t ctx;
        fq_t a, b, c;
        fmpz_t x, y;
        long j;

        fq_ctx_randtest(ctx, state);

        fq_init(a, ctx);
        fq_init(b, ctx);
        fq_init(c, ctx);
        fmpz_init(x);
        fmpz_init(y);

        fq_randtest(a, state, ctx);
        fq_reduce(a, ctx);

        fq_norm(x, a, ctx);

        fq_one(b, ctx);
        for (j = 0; j < fq_ctx_degree(ctx); j++)
        {
            fq_frobenius(c, a, j, ctx);
            fq_mul(b, b, c, ctx);
        }
        fmpz_poly_get_coeff_fmpz(y, b, 0);

        result = fmpz_equal(x, y);
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("a = "), fq_print_pretty(a, ctx), flint_printf("\n");
            flint_printf("b = "), fq_print_pretty(b, ctx), flint_printf("\n");
            flint_printf("x = "), fmpz_print(x), flint_printf("\n");
            flint_printf("y = "), fmpz_print(y), flint_printf("\n");
            for (j = 0; j < fq_ctx_degree(ctx); j++)
            {
                fq_frobenius(c, a, j, ctx);
                flint_printf("sigma^%ld = ", j), fq_print_pretty(c, ctx), flint_printf("\n");
            }
            abort();
        }

        fq_clear(a, ctx);
        fq_clear(b, ctx);
        fq_clear(c, ctx);
        fmpz_clear(x);
        fmpz_clear(y);

        fq_ctx_clear(ctx);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
