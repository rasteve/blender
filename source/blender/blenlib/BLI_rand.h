/* SPDX-FileCopyrightText: 2001-2002 NaN Holding BV. All rights reserved.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "BLI_compiler_attrs.h"
#include "BLI_sys_types.h"

/** \file
 * \ingroup bli
 * \brief Random number functions.
 */

/**
 * RNG is an abstract random number generator type that avoids using globals.
 * Always use this instead of the global RNG unless you have a good reason,
 * the global RNG is not thread safe and will not give repeatable results.
 */
struct RNG;
typedef struct RNG RNG;

struct RNG_THREAD_ARRAY;
typedef struct RNG_THREAD_ARRAY RNG_THREAD_ARRAY;

struct RNG *BLI_rng_new(unsigned int seed);
/**
 * A version of #BLI_rng_new that hashes the seed.
 */
struct RNG *BLI_rng_new_srandom(unsigned int seed);
void BLI_rng_free(struct RNG *rng) ATTR_NONNULL(1);

void BLI_rng_seed(struct RNG *rng, unsigned int seed) ATTR_NONNULL(1);
/**
 * Use a hash table to create better seed.
 */
void BLI_rng_srandom(struct RNG *rng, unsigned int seed) ATTR_NONNULL(1);
void BLI_rng_get_char_n(RNG *rng, char *bytes, size_t bytes_len) ATTR_NONNULL(1, 2);
int BLI_rng_get_int(struct RNG *rng) ATTR_WARN_UNUSED_RESULT ATTR_NONNULL(1);
unsigned int BLI_rng_get_uint(struct RNG *rng) ATTR_WARN_UNUSED_RESULT ATTR_NONNULL(1);
/**
 * \return Random value (0..1), but never 1.0.
 */
double BLI_rng_get_double(struct RNG *rng) ATTR_WARN_UNUSED_RESULT ATTR_NONNULL(1);
/**
 * \return Random value (0..1), but never 1.0.
 */
float BLI_rng_get_float(struct RNG *rng) ATTR_WARN_UNUSED_RESULT ATTR_NONNULL(1);
/**
 * Generate a random point inside given tri.
 */
void BLI_rng_get_tri_sample_float_v2(struct RNG *rng,
                                     const float v1[2],
                                     const float v2[2],
                                     const float v3[2],
                                     float r_pt[2]) ATTR_NONNULL();

void BLI_rng_shuffle_array(struct RNG *rng,
                           void *data,
                           unsigned int elem_size_i,
                           unsigned int elem_num) ATTR_NONNULL(1, 2);

void BLI_rng_shuffle_bitmap(struct RNG *rng, unsigned int *bitmap, unsigned int bits_num)
    ATTR_NONNULL(1, 2);

/** Note that skipping is as slow as generating n numbers! */
/**
 * Simulate getting \a n random values.
 *
 * \note Useful when threaded code needs consistent values, independent of task division.
 */
void BLI_rng_skip(struct RNG *rng, int n) ATTR_NONNULL(1);

/** Return a pseudo-random (hash) float from an integer value */
float BLI_hash_frand(unsigned int seed) ATTR_WARN_UNUSED_RESULT;

/**
 * Shuffle an array randomly using the given seed contents.
 * This routine does not use nor modify the state of the BLI random number generator.
 */
void BLI_array_randomize(void *data,
                         unsigned int elem_size,
                         unsigned int elem_num,
                         unsigned int seed);

void BLI_bitmap_randomize(unsigned int *bitmap, unsigned int bits_num, unsigned int seed)
    ATTR_NONNULL(1);

/** array versions for thread safe random generation */
RNG_THREAD_ARRAY *BLI_rng_threaded_new(void);
void BLI_rng_threaded_free(struct RNG_THREAD_ARRAY *rngarr) ATTR_NONNULL(1);
int BLI_rng_thread_rand(RNG_THREAD_ARRAY *rngarr, int thread) ATTR_WARN_UNUSED_RESULT;

/* Low-discrepancy sequences. */

/** Return the _n_th number of the given low-discrepancy sequence. */
void BLI_halton_1d(unsigned int prime, double offset, int n, double *r);
void BLI_halton_2d(const unsigned int prime[2], double offset[2], int n, double *r);
void BLI_halton_3d(const unsigned int prime[3], double offset[3], int n, double *r);
void BLI_hammersley_1d(unsigned int n, double *r);
