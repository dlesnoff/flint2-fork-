/*
    Copyright (C) 2020 Fredrik Johansson

    This file is part of Calcium.

    Calcium is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#ifndef CA_POLY_H
#define CA_POLY_H

#ifdef CA_POLY_INLINES_C
#define CA_POLY_INLINE
#else
#define CA_POLY_INLINE static __inline__
#endif

#include <stdio.h>
#include "flint/flint.h"
#include "flint/fmpz_poly.h"
#include "flint/fmpq_poly.h"
#include "arb_poly.h"
#include "acb_poly.h"
#include "antic/nf.h"
#include "antic/nf_elem.h"
#include "ca.h"
#include "ca_vec.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Polynomial object */

typedef ca_vec_struct ca_poly_struct;
typedef ca_poly_struct ca_poly_t[1];

/* Vectors of polynomials */

typedef struct
{
    ca_poly_struct * coeffs;
    slong length;
    slong alloc;
}
ca_poly_vec_struct;

typedef ca_poly_vec_struct ca_poly_vec_t[1];

/* Memory management */

void ca_poly_init(ca_poly_t poly, ca_ctx_t ctx);
void ca_poly_init2(ca_poly_t poly, slong len, ca_ctx_t ctx);
void ca_poly_clear(ca_poly_t poly, ca_ctx_t ctx);
void ca_poly_fit_length(ca_poly_t poly, slong len, ca_ctx_t ctx);
void _ca_poly_set_length(ca_poly_t poly, slong len, ca_ctx_t ctx);
void _ca_poly_normalise(ca_poly_t poly, ca_ctx_t ctx);

CA_POLY_INLINE void
ca_poly_swap(ca_poly_t poly1, ca_poly_t poly2, ca_ctx_t ctx)
{
    ca_poly_struct t = *poly1;
    *poly1 = *poly2;
    *poly2 = t;
}

/* Assignment and simple values */

void ca_poly_set_ca(ca_poly_t poly, const ca_t x, ca_ctx_t ctx);
void ca_poly_set_si(ca_poly_t poly, slong x, ca_ctx_t ctx);

CA_POLY_INLINE void
ca_poly_zero(ca_poly_t poly, ca_ctx_t ctx)
{
    _ca_poly_set_length(poly, 0, ctx);
}

CA_POLY_INLINE void
ca_poly_x(ca_poly_t poly, ca_ctx_t ctx)
{
    ca_poly_fit_length(poly, 2, ctx);
    ca_zero(poly->coeffs, ctx);
    ca_one(poly->coeffs + 1, ctx);
    _ca_poly_set_length(poly, 2, ctx);
}

CA_POLY_INLINE void
ca_poly_one(ca_poly_t poly, ca_ctx_t ctx)
{
    ca_poly_set_si(poly, 1, ctx);
}

void ca_poly_set(ca_poly_t res, const ca_poly_t src, ca_ctx_t ctx);
void ca_poly_set_fmpz_poly(ca_poly_t res, const fmpz_poly_t src, ca_ctx_t ctx);
void ca_poly_set_fmpq_poly(ca_poly_t res, const fmpq_poly_t src, ca_ctx_t ctx);

/* Random generation */

void ca_poly_randtest(ca_poly_t poly, flint_rand_t state, slong len, slong depth, slong bits, ca_ctx_t ctx);
void ca_poly_randtest_rational(ca_poly_t poly, flint_rand_t state, slong len, slong bits, ca_ctx_t ctx);

/* Basic operations */

/* todo: document */
int ca_poly_make_monic(ca_poly_t res, const ca_poly_t poly, ca_ctx_t ctx);

/* todo: document */
void _ca_poly_reverse(ca_ptr res, ca_srcptr poly, slong len, slong n, ca_ctx_t ctx);

/* Input and output */

void ca_poly_print(const ca_poly_t poly, ca_ctx_t ctx);
void ca_poly_printn(const ca_poly_t poly, slong digits, ca_ctx_t ctx);

/* Degree and leading coefficient */

int ca_poly_is_proper(const ca_poly_t poly, ca_ctx_t ctx);

/* Comparisons */

/* todo: document */
truth_t _ca_poly_check_equal(ca_srcptr poly1, slong len1, ca_srcptr poly2, slong len2, ca_ctx_t ctx);
truth_t ca_poly_check_equal(const ca_poly_t poly1, const ca_poly_t poly2, ca_ctx_t ctx);

/* todo: document, improve */
CA_POLY_INLINE truth_t
ca_poly_check_is_one(const ca_poly_t poly, ca_ctx_t ctx)
{
    truth_t res;
    ca_poly_t t;
    ca_poly_init(t, ctx);
    ca_poly_one(t, ctx);
    res = ca_poly_check_equal(poly, t, ctx);
    ca_poly_clear(t, ctx);
    return res;
}

/* Arithmetic */

void ca_poly_neg(ca_poly_t res, const ca_poly_t src, ca_ctx_t ctx);

void _ca_poly_add(ca_ptr res, ca_srcptr poly1, slong len1, ca_srcptr poly2, slong len2, ca_ctx_t ctx);
void ca_poly_add(ca_poly_t res, const ca_poly_t poly1, const ca_poly_t poly2, ca_ctx_t ctx);

void _ca_poly_sub(ca_ptr res, ca_srcptr poly1, slong len1, ca_srcptr poly2, slong len2, ca_ctx_t ctx);
void ca_poly_sub(ca_poly_t res, const ca_poly_t poly1, const ca_poly_t poly2, ca_ctx_t ctx);

void _ca_poly_mul(ca_ptr C, ca_srcptr A, slong lenA, ca_srcptr B, slong lenB, ca_ctx_t ctx);
void ca_poly_mul(ca_poly_t res, const ca_poly_t poly1, const ca_poly_t poly2, ca_ctx_t ctx);

CA_POLY_INLINE void
ca_poly_mul_ca(ca_poly_t res, const ca_poly_t poly, const ca_t c, ca_ctx_t ctx)
{
    ca_poly_fit_length(res, poly->length, ctx);
    _ca_vec_scalar_mul_ca(res->coeffs, poly->coeffs, poly->length, c, ctx);
    _ca_poly_set_length(res, poly->length, ctx);
    _ca_poly_normalise(res, ctx);
}

void _ca_poly_mullow(ca_ptr C, ca_srcptr A, slong lenA, ca_srcptr B, slong lenB, slong n, ca_ctx_t ctx);
void ca_poly_mullow(ca_poly_t res, const ca_poly_t poly1, const ca_poly_t poly2, slong n, ca_ctx_t ctx);

void _ca_poly_divrem_basecase(ca_ptr Q, ca_ptr R, ca_srcptr A, slong lenA, ca_srcptr B, slong lenB, const ca_t invB, ca_ctx_t ctx);
int ca_poly_divrem_basecase(ca_poly_t Q, ca_poly_t R, const ca_poly_t A, const ca_poly_t B, ca_ctx_t ctx);

void _ca_poly_divrem(ca_ptr Q, ca_ptr R, ca_srcptr A, slong lenA, ca_srcptr B, slong lenB, const ca_t invB, ca_ctx_t ctx);
int ca_poly_divrem(ca_poly_t Q, ca_poly_t R, const ca_poly_t A, const ca_poly_t B, ca_ctx_t ctx);

int ca_poly_div(ca_poly_t Q, const ca_poly_t A, const ca_poly_t B, ca_ctx_t ctx);
int ca_poly_rem(ca_poly_t R, const ca_poly_t A, const ca_poly_t B, ca_ctx_t ctx);

/* Integral and derivative */

void _ca_poly_derivative(ca_ptr res, ca_srcptr poly, slong len, ca_ctx_t ctx);
void ca_poly_derivative(ca_poly_t res, const ca_poly_t poly, ca_ctx_t ctx);

void _ca_poly_integral(ca_ptr res, ca_srcptr poly, slong len, ca_ctx_t ctx);
void ca_poly_integral(ca_poly_t res, const ca_poly_t poly, ca_ctx_t ctx);

/* Greatest common divisor */

slong _ca_poly_gcd_euclidean(ca_ptr G, ca_srcptr A, slong lenA, ca_srcptr B, slong lenB, ca_ctx_t ctx);
int ca_poly_gcd_euclidean(ca_poly_t G, const ca_poly_t A, const ca_poly_t B, ca_ctx_t ctx);

slong _ca_poly_gcd(ca_ptr G, ca_srcptr A, slong lenA, ca_srcptr B, slong lenB, ca_ctx_t ctx);
int ca_poly_gcd(ca_poly_t G, const ca_poly_t A, const ca_poly_t B, ca_ctx_t ctx);

/* Vectors of polynomials */

ca_poly_struct * _ca_poly_vec_init(slong len, ca_ctx_t ctx);
void ca_poly_vec_init(ca_poly_vec_t res, slong len, ca_ctx_t ctx);

void _ca_poly_vec_fit_length(ca_poly_vec_t vec, slong len, ca_ctx_t ctx);
void ca_poly_vec_set_length(ca_poly_vec_t vec, slong len, ca_ctx_t ctx);

void _ca_poly_vec_clear(ca_poly_struct * v, slong len, ca_ctx_t ctx);
void ca_poly_vec_clear(ca_poly_vec_t vec, ca_ctx_t ctx);

void ca_poly_vec_append(ca_poly_vec_t vec, const ca_poly_t f, ca_ctx_t ctx);

/* Roots and factorization */

int ca_poly_factor_squarefree(ca_t c, ca_poly_vec_t fac, ulong * exp, const ca_poly_t F, ca_ctx_t ctx);

void _ca_poly_set_roots(ca_ptr poly, ca_srcptr roots, slong n, ca_ctx_t ctx);
void ca_poly_set_roots(ca_poly_t poly, ca_vec_t roots, ca_ctx_t ctx);

int _ca_poly_roots(ca_ptr roots, ca_srcptr poly, slong len, ca_ctx_t ctx);
int ca_poly_roots(ca_vec_t roots, const ca_poly_t poly, ca_ctx_t ctx);

#ifdef __cplusplus
}
#endif

#endif
