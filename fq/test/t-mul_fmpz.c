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

    Copyright (C) 2009 William Hart
    Copyright (C) 2013 Andres Goens

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fq.h" 
#include "long_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("mul_fmpz....");
    fflush(stdout);

    flint_randinit(state);
    /* Check aliasing of a, b */
    for (i = 0; i < 2000 * flint_test_multiplier(); i++)
    {
        fq_ctx_t ctx;
        fmpz_t x;
        fq_t a, b;

        fq_ctx_randtest(ctx, state);

        fq_init(a, ctx);
        fq_init(b, ctx);
	fmpz_init(x);

        fq_randtest(a, state, ctx);
        fmpz_randtest_mod_signed(x,state,fq_ctx_prime(ctx));
        fq_mul_fmpz(b, a, x, ctx);
        fq_mul_fmpz(a, a, x, ctx);

        result = (fq_equal(a, b, ctx));
        if (!result)
        {
            flint_printf("fail:\n\n");
            flint_printf("a = "), fq_print_pretty(a, ctx), flint_printf("\n");
            flint_printf("b = "), fq_print_pretty(b, ctx), flint_printf("\n");
	    flint_printf("x = "), fmpz_print(x), flint_printf("\n");
            abort();
        }

        fq_clear(a, ctx);
        fq_clear(b, ctx);
	fmpz_clear(x);
        fq_ctx_clear(ctx);
    }

    /* compare with direct multiplication */
    for (i = 0; i < 2000 * flint_test_multiplier(); i++)
    {
        fq_ctx_t ctx;
        fmpz_t x;
        fq_t a, c;
	fmpz_poly_t b;

        fq_ctx_randtest(ctx, state);

        fq_init(a, ctx);
        fq_init(c, ctx);
	fmpz_init(x);
	fmpz_poly_init(b);

        fq_randtest(a, state, ctx);
        fmpz_randtest_mod_signed(x,state,fq_ctx_prime(ctx));
        fq_mul_fmpz(c, a, x, ctx);
        fmpz_poly_scalar_mul_fmpz(b,a,x);
	fq_reduce(b,ctx);


        result = (fq_equal(c, b, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n\n");
            flint_printf("a = "), fq_print_pretty(a, ctx), flint_printf("\n");
            flint_printf("b = "), fq_print_pretty(b, ctx), flint_printf("\n");
	    flint_printf("x = "), fmpz_print(x), flint_printf("\n");
            abort();
        }

        fq_clear(a, ctx);
        fq_clear(c, ctx);
        fmpz_poly_clear(b);
	fmpz_clear(x);
        fq_ctx_clear(ctx);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    flint_printf("PASS\n");
    return 0;
}
