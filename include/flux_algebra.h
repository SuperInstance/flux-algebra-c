#ifndef FLUX_ALGEBRA_H
#define FLUX_ALGEBRA_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ─── HarmonicRing — Z/nZ for pitch classes ────────────────── */

#define FA_MAX_IDEALS  32
#define FA_MAX_ELEMENTS 128

typedef struct {
    int modulus;
} FaHarmonicRing;

void fa_harmonic_ring_init(FaHarmonicRing *hr, int modulus);

/* Ring operations */
int  fa_add(const FaHarmonicRing *hr, int a, int b);
int  fa_subtract(const FaHarmonicRing *hr, int a, int b);
int  fa_multiply(const FaHarmonicRing *hr, int a, int b);
int  fa_negate(const FaHarmonicRing *hr, int a);
int  fa_invert(const FaHarmonicRing *hr, int a, int n);
int  fa_transpose(const FaHarmonicRing *hr, int pc, int interval);

/* Elements and units */
int  fa_units(const FaHarmonicRing *hr, int *out, int max_out);

/* Ideals */
typedef struct {
    int elements[FA_MAX_ELEMENTS];
    int count;
} FaIdeal;

int  fa_all_ideals(const FaHarmonicRing *hr, FaIdeal *out, int max_out);
FaIdeal fa_ideal_generated_by(const FaHarmonicRing *hr, const int *gens, int n_gens);

/* Interval class between two pitch classes */
int  fa_interval_class(const FaHarmonicRing *hr, int a, int b);

/* Pitch class set operations */
int  fa_pitch_class_set(const FaHarmonicRing *hr, const int *notes, int n,
                        int *out, int max_out);

/* Prime form computation */
int  fa_prime_form(const FaHarmonicRing *hr, const int *notes, int n,
                   int *out, int max_out);

/* ─── IntervalRing — just-intonation frequency ratios ──────── */

typedef struct {
    int prime_limit[8];
    int n_primes;
} FaIntervalRing;

void fa_interval_ring_init(FaIntervalRing *ir,
                           const int *primes, int n_primes);
void fa_interval_ring_default(FaIntervalRing *ir);  /* (2,3,5,7,11) */

/* Operations on numerator/denominator pairs */
void fa_ir_add(const FaIntervalRing *ir,
               int64_t a_num, int64_t a_den,
               int64_t b_num, int64_t b_den,
               int64_t *out_num, int64_t *out_den);
void fa_ir_subtract(const FaIntervalRing *ir,
                    int64_t a_num, int64_t a_den,
                    int64_t b_num, int64_t b_den,
                    int64_t *out_num, int64_t *out_den);
void fa_ir_multiply(const FaIntervalRing *ir,
                    int64_t a_num, int64_t a_den,
                    int64_t n,
                    int64_t *out_num, int64_t *out_den);
void fa_ir_invert(int64_t num, int64_t den,
                  int64_t *out_num, int64_t *out_den);

/* Convert ratio to cents */
double fa_ir_cents(int64_t num, int64_t den);

/* ─── Group theory operations ──────────────────────────────── */

/* GCD and modular inverse */
int  fa_gcd(int a, int b);
int  fa_mod_inverse(int a, int m);

/* Group order of element in Z/nZ */
int  fa_element_order(int a, int n);

/* Check if two elements generate the same cyclic subgroup */
int  fa_same_subgroup(int a, int b, int n);

/* ─── Interval vector ─────────────────────────────────────── */

/* Compute interval vector (IC distribution) for a pitch-class set.
   Returns counts[0..6] for interval classes 0-6. */
void fa_interval_vector(const FaHarmonicRing *hr,
                        const int *pcs, int n,
                        int counts[7]);

#ifdef __cplusplus
}
#endif

#endif /* FLUX_ALGEBRA_H */
