/*
 * Tests for flux-algebra-c
 */
#include "flux_algebra.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ASSERT_FEQ(a, b, eps) do { \
    if (fabs((a) - (b)) > (eps)) { \
        fprintf(stderr, "FAIL %s:%d: %.10f != %.10f\n", \
                __FILE__, __LINE__, (double)(a), (double)(b)); \
        return 1; \
    } \
} while(0)

static int test_ring_add(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_add(&hr, 7, 5) == 0);     /* G + E = C mod 12 */
    assert(fa_add(&hr, 11, 3) == 2);    /* B + D# = D */
    printf("  PASS ring_add\n");
    return 0;
}

static int test_ring_subtract(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_subtract(&hr, 0, 7) == 5);  /* C - G = F */
    printf("  PASS ring_subtract\n");
    return 0;
}

static int test_ring_multiply(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_multiply(&hr, 3, 4) == 0);  /* 3×4 = 12 ≡ 0 */
    assert(fa_multiply(&hr, 7, 7) == 1);  /* 7² = 49 ≡ 1 */
    printf("  PASS ring_multiply\n");
    return 0;
}

static int test_negate(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_negate(&hr, 0) == 0);
    assert(fa_negate(&hr, 5) == 7);  /* -5 ≡ 7 mod 12 */
    printf("  PASS negate\n");
    return 0;
}

static int test_invert_around_axis(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_invert(&hr, 4, 0) == 8);  /* invert E around C → Ab */
    printf("  PASS invert_around_axis\n");
    return 0;
}

static int test_transpose(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_transpose(&hr, 0, 7) == 7);  /* C up P5 → G */
    assert(fa_transpose(&hr, 7, 7) == 2);  /* G up P5 → D */
    printf("  PASS transpose\n");
    return 0;
}

static int test_units(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    int units[12];
    int n = fa_units(&hr, units, 12);
    /* Units of Z/12Z: {1, 5, 7, 11} */
    assert(n == 4);
    assert(units[0] == 1);
    assert(units[1] == 5);
    assert(units[2] == 7);
    assert(units[3] == 11);
    printf("  PASS units\n");
    return 0;
}

static int test_ideals(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    FaIdeal ideals[32];
    int n = fa_all_ideals(&hr, ideals, 32);
    assert(n == 6);  /* divisors of 12: 1,2,3,4,6,12 */
    /* Smallest first: {0}, {0,6}, {0,4,8}, {0,3,6,9}, {0,2,4,6,8,10}, Z/12Z */
    assert(ideals[0].count == 1);  /* {0} */
    assert(ideals[1].count == 2);  /* {0,6} */
    assert(ideals[2].count == 3);  /* {0,4,8} */
    printf("  PASS ideals (found %d ideals)\n", n);
    return 0;
}

static int test_ideal_generated_by(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    int gens[] = {0, 4, 7};  /* C major chord */
    FaIdeal ideal = fa_ideal_generated_by(&hr, gens, 3);
    /* gcd(0,4,7,12) = 1, so ideal = Z/12Z */
    assert(ideal.count == 12);
    printf("  PASS ideal_generated_by (C major generates full ring, count=%d)\n", ideal.count);
    return 0;
}

static int test_ideal_tritone(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    int gens[] = {0, 6};  /* tritone pair */
    FaIdeal ideal = fa_ideal_generated_by(&hr, gens, 2);
    /* gcd(0,6,12) = 6, ideal = {0, 6} */
    assert(ideal.count == 2);
    assert(ideal.elements[0] == 0);
    assert(ideal.elements[1] == 6);
    printf("  PASS ideal_tritone\n");
    return 0;
}

static int test_interval_class(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    assert(fa_interval_class(&hr, 0, 4) == 4);   /* C-E = IC 4 */
    assert(fa_interval_class(&hr, 0, 7) == 5);   /* C-G = IC 5 */
    assert(fa_interval_class(&hr, 0, 6) == 6);   /* tritone = IC 6 */
    assert(fa_interval_class(&hr, 0, 11) == 1);  /* C-B = IC 1 */
    printf("  PASS interval_class\n");
    return 0;
}

static int test_pitch_class_set(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    int notes[] = {0, 4, 7, 12, 16};  /* C, E, G, C(+oct), E(+oct) */
    int pcs[12];
    int n = fa_pitch_class_set(&hr, notes, 5, pcs, 12);
    assert(n == 3);
    assert(pcs[0] == 0);
    assert(pcs[1] == 4);
    assert(pcs[2] == 7);
    printf("  PASS pitch_class_set\n");
    return 0;
}

static int test_prime_form_major_triad(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    int notes[] = {4, 7, 0};  /* E, G, C — out of order */
    int pf[12];
    int n = fa_prime_form(&hr, notes, 3, pf, 12);
    assert(n == 3);
    /* Major triad (0,4,7) and minor triad (0,3,7) are in the same set class.
       Prime form is (0,3,7) — the more left-packed inversion. */
    assert(pf[0] == 0);
    assert(pf[1] == 3);
    assert(pf[2] == 7);
    printf("  PASS prime_form_major_triad (%d,%d,%d)\n", pf[0], pf[1], pf[2]);
    return 0;
}

static int test_interval_vector(void)
{
    FaHarmonicRing hr;
    fa_harmonic_ring_init(&hr, 12);
    int major[] = {0, 4, 7};  /* C major */
    int counts[7];
    fa_interval_vector(&hr, major, 3, counts);
    /* C-E=IC4, C-G=IC5, E-G=IC3 */
    assert(counts[0] == 0);
    assert(counts[3] == 1);
    assert(counts[4] == 1);
    assert(counts[5] == 1);
    printf("  PASS interval_vector (IC3=%d, IC4=%d, IC5=%d)\n",
           counts[3], counts[4], counts[5]);
    return 0;
}

static int test_interval_ring_cents(void)
{
    /* P5 = 3/2 = 702 cents */
    double cents = fa_ir_cents(3, 2);
    ASSERT_FEQ(cents, 701.955, 0.1);
    /* M3 = 5/4 = 386 cents */
    cents = fa_ir_cents(5, 4);
    ASSERT_FEQ(cents, 386.314, 0.1);
    printf("  PASS interval_ring_cents\n");
    return 0;
}

static int test_interval_ring_add(void)
{
    FaIntervalRing ir;
    fa_interval_ring_default(&ir);
    int64_t num, den;
    /* P5 + M3 = (3/2)(5/4) = 15/8 = M7 */
    fa_ir_add(&ir, 3, 2, 5, 4, &num, &den);
    assert(num == 15);
    assert(den == 8);
    printf("  PASS interval_ring_add (3/2 + 5/4 = %lld/%lld)\n",
           (long long)num, (long long)den);
    return 0;
}

static int test_interval_ring_invert(void)
{
    int64_t num, den;
    fa_ir_invert(3, 2, &num, &den);
    assert(num == 2);
    assert(den == 3);
    printf("  PASS interval_ring_invert\n");
    return 0;
}

static int test_gcd(void)
{
    assert(fa_gcd(12, 8) == 4);
    assert(fa_gcd(7, 12) == 1);
    assert(fa_gcd(0, 5) == 5);
    printf("  PASS gcd\n");
    return 0;
}

static int test_mod_inverse(void)
{
    assert(fa_mod_inverse(7, 12) == 7);  /* 7×7 = 49 ≡ 1 mod 12 */
    assert(fa_mod_inverse(5, 12) == 5);  /* 5×5 = 25 ≡ 1 mod 12 */
    assert(fa_mod_inverse(2, 12) == -1); /* not coprime */
    printf("  PASS mod_inverse\n");
    return 0;
}

static int test_element_order(void)
{
    assert(fa_element_order(7, 12) == 12);  /* P5 generates Z/12Z */
    assert(fa_element_order(6, 12) == 2);   /* tritone: order 2 */
    assert(fa_element_order(4, 12) == 3);   /* M3: generates {0,4,8} */
    printf("  PASS element_order\n");
    return 0;
}

/* ─── Main ─────────────────────────────────────────────────── */

typedef int (*test_fn)(void);

int main(void)
{
    test_fn tests[] = {
        test_ring_add,
        test_ring_subtract,
        test_ring_multiply,
        test_negate,
        test_invert_around_axis,
        test_transpose,
        test_units,
        test_ideals,
        test_ideal_generated_by,
        test_ideal_tritone,
        test_interval_class,
        test_pitch_class_set,
        test_prime_form_major_triad,
        test_interval_vector,
        test_interval_ring_cents,
        test_interval_ring_add,
        test_interval_ring_invert,
        test_gcd,
        test_mod_inverse,
        test_element_order,
    };
    int n = sizeof(tests) / sizeof(tests[0]);
    int failures = 0;
    printf("Running %d tests...\n", n);
    for (int i = 0; i < n; i++) {
        if (tests[i]()) failures++;
    }
    printf("\n%s: %d/%d passed\n",
           failures ? "FAIL" : "OK", n - failures, n);
    return failures;
}
