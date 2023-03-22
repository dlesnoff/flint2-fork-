/*
    Copyright (C) 2023 Albin Ahlbäck

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#ifndef LIMB_TYPES_H
#define LIMB_TYPES_H

#include "flint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FLINT_MAX_FACTORS_IN_LIMB 15

typedef struct
{
   int num;
   int exp[FLINT_MAX_FACTORS_IN_LIMB];
   ulong p[FLINT_MAX_FACTORS_IN_LIMB];
}
n_factor_t;

#ifdef __cplusplus
}
#endif

#endif /* LIMB_TYPES_H */
