#include "flux_algebra.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int test_count = 0;
#define TEST(name) do { printf("  ✓ %s\n", name); test_count++; } while(0)

/* ── Original tests (11) ───────────────────────────────────────────── */

static void test_major_chord(void) {
    Chord c = chord_major(0);
    assert(c.roots[0] == 0 && c.roots[1] == 4 && c.roots[2] == 7);
    assert(chord_is_major(&c));
    TEST("major chord");
}

static void test_minor_chord(void) {
    Chord c = chord_minor(0);
    assert(c.roots[0] == 0 && c.roots[1] == 3 && c.roots[2] == 7);
    assert(chord_is_minor(&c));
    TEST("minor chord");
}

static void test_transpose(void) {
    Chord c = chord_major(0);
    Chord d = chord_transpose(&c, 2);
    assert(d.roots[0] == 2 && d.roots[1] == 6 && d.roots[2] == 9);
    TEST("transpose");
}

static void test_parallel(void) {
    Chord cmaj = chord_major(0);
    Chord cmin = parallel(&cmaj);
    assert(chord_is_minor(&cmin));
    assert(cmin.roots[0] == 0);
    TEST("parallel (P)");
}

static void test_relative(void) {
    Chord cmaj = chord_major(0);
    Chord amin = relative(&cmaj);
    assert(chord_is_minor(&amin));
    assert(amin.roots[0] == 9);
    TEST("relative (R)");
}

static void test_leading_tone(void) {
    Chord cmaj = chord_major(0);
    Chord bmin = leading_tone(&cmaj);
    assert(chord_is_minor(&bmin));
    assert(bmin.roots[0] == 11);
    TEST("leading tone (L)");
}

static void test_tropical(void) {
    assert(fabs(tropical_add(3.0, 5.0) - 3.0) < 0.001);
    assert(fabs(tropical_mul(3.0, 5.0) - 8.0) < 0.001);
    TEST("tropical semiring");
}

static void test_et_tuning(void) {
    TuningField t = tuning_equal_temperament();
    assert(fabs(tuning_frequency(&t, 69) - 440.0) < 0.01);
    TEST("equal temperament tuning");
}

static void test_just_tuning(void) {
    TuningField t = tuning_just_intonation();
    double cents = tuning_cents_deviation(&t, 64);
    assert(fabs(cents) < 50.0);
    TEST("just intonation");
}

static void test_voice_leading(void) {
    Chord cmaj = chord_major(0);
    Chord dmaj = chord_major(2);
    double dist = voice_leading_distance(&cmaj, &dmaj);
    assert(dist > 0.0);
    TEST("voice leading distance");
}

static void test_dim_aug(void) {
    Chord dim = chord_diminished(0);
    assert(dim.roots[0] == 0 && dim.roots[1] == 3 && dim.roots[2] == 6);
    Chord aug = chord_augmented(0);
    assert(aug.roots[0] == 0 && aug.roots[1] == 4 && aug.roots[2] == 8);
    TEST("diminished and augmented chords");
}

/* ── New tests (target: 22+) ──────────────────────────────────────── */

static void test_chord_is_diminished(void) {
    Chord c = chord_diminished(0);
    assert(chord_is_diminished(&c));
    assert(!chord_is_major(&c));
    assert(!chord_is_minor(&c));
    TEST("chord_is_diminished");
}

static void test_chord_is_augmented(void) {
    Chord c = chord_augmented(0);
    assert(chord_is_augmented(&c));
    assert(!chord_is_major(&c));
    assert(!chord_is_minor(&c));
    TEST("chord_is_augmented");
}

static void test_chord_eq(void) {
    Chord a = chord_major(0);
    Chord b = chord_major(0);
    Chord c = chord_minor(0);
    assert(chord_eq(&a, &b));
    assert(!chord_eq(&a, &c));
    assert(!chord_eq(NULL, &a));
    TEST("chord equality");
}

static void test_chord_contains(void) {
    Chord cmaj = chord_major(0); /* C E G = 0 4 7 */
    assert(chord_contains(&cmaj, 0));
    assert(chord_contains(&cmaj, 4));
    assert(chord_contains(&cmaj, 7));
    assert(!chord_contains(&cmaj, 1));
    assert(!chord_contains(NULL, 0));
    TEST("chord_contains");
}

static void test_tropical_idempotent(void) {
    assert(tropical_is_idempotent(5.0));
    assert(tropical_is_idempotent(-3.0));
    assert(tropical_is_idempotent(0.0));
    TEST("tropical idempotent");
}

static void test_tropical_pow(void) {
    assert(fabs(tropical_pow(3.0, 4) - 12.0) < 0.001);
    assert(fabs(tropical_pow(2.0, 0) - 0.0) < 0.001);
    assert(fabs(tropical_pow(-1.0, 3) - (-3.0)) < 0.001);
    TEST("tropical power");
}

static void test_plr_compose(void) {
    Chord cmaj = chord_major(0);
    /* PLR starting from C major: P→Cm, L→DbMaj, R→Bbm */
    Chord result = plr_compose(&cmaj, "PLR", 3);
    assert(chord_is_minor(&result));
    TEST("PLR compose (PLR on C major → valid triad)");
}

static void test_plr_compose_pp(void) {
    Chord cmaj = chord_major(0);
    /* PP = identity */
    Chord result = plr_compose(&cmaj, "PP", 2);
    assert(chord_eq(&result, &cmaj));
    TEST("PLR compose (PP = identity)");
}

static void test_voice_leading_self(void) {
    Chord c = chord_major(0);
    double dist = voice_leading_distance(&c, &c);
    assert(fabs(dist) < 1e-10);
    TEST("voice leading distance (self = 0)");
}

static void test_voice_leading_optimal(void) {
    Chord cmaj = chord_major(0);  /* 0, 4, 7 */
    Chord emin = chord_minor(4);  /* 4, 7, 11 */
    double opt = voice_leading_optimal(&cmaj, &emin);
    assert(opt >= 0.0);
    assert(opt <= voice_leading_distance(&cmaj, &emin) + 0.01);
    TEST("optimal voice leading");
}

static void test_voice_leading_null(void) {
    Chord c = chord_major(0);
    assert(voice_leading_distance(NULL, &c) > 1e8);
    assert(voice_leading_distance(&c, NULL) > 1e8);
    TEST("voice leading null safety");
}

static void test_transpose_identity(void) {
    Chord c = chord_major(0);
    Chord t = chord_transpose(&c, 0);
    assert(chord_eq(&c, &t));
    TEST("transpose by 0 = identity");
}

static void test_transpose_octave(void) {
    Chord c = chord_major(0);
    Chord t = chord_transpose(&c, 12);
    assert(chord_eq(&c, &t));
    TEST("transpose by octave = identity");
}

static void test_et_tuning_octave(void) {
    TuningField t = tuning_equal_temperament();
    double f60 = tuning_frequency(&t, 60);
    double f72 = tuning_frequency(&t, 72);
    assert(fabs(f72 / f60 - 2.0) < 0.01);
    TEST("ET tuning octave ratio = 2");
}

static void test_just_tuning_fifth(void) {
    TuningField t = tuning_just_intonation();
    /* G (7) should be close to 3/2 ratio */
    double ratio = t.ratios[7];
    assert(fabs(ratio - 1.5) < 0.01);
    TEST("just intonation fifth ratio ≈ 3/2");
}

static void test_tuning_null(void) {
    assert(tuning_frequency(NULL, 60) == 0.0);
    assert(tuning_cents_deviation(NULL, 60) == 0.0);
    TEST("tuning null safety");
}

static void test_parallel_roundtrip(void) {
    Chord cmaj = chord_major(0);
    Chord cmin = parallel(&cmaj);
    Chord back = parallel(&cmin);
    assert(chord_eq(&back, &cmaj));
    TEST("parallel roundtrip (P∘P = id)");
}

static void test_relative_roundtrip(void) {
    Chord cmaj = chord_major(0);
    Chord amin = relative(&cmaj);
    Chord back = relative(&amin);
    assert(chord_eq(&back, &cmaj));
    TEST("relative roundtrip (R∘R = id)");
}

static void test_leading_tone_roundtrip(void) {
    Chord cmaj = chord_major(0);
    Chord bmin = leading_tone(&cmaj);
    Chord back = leading_tone(&bmin);
    assert(chord_eq(&back, &cmaj));
    TEST("leading tone roundtrip (L∘L = id)");
}

static void test_all_major_chords(void) {
    for (int r = 0; r < 12; r++) {
        Chord c = chord_major(r);
        assert(chord_is_major(&c));
        assert(!chord_is_minor(&c));
    }
    TEST("all 12 major chords valid");
}

static void test_all_minor_chords(void) {
    for (int r = 0; r < 12; r++) {
        Chord c = chord_minor(r);
        assert(chord_is_minor(&c));
        assert(!chord_is_major(&c));
    }
    TEST("all 12 minor chords valid");
}

static void test_benchmark_plr(void) {
    double elapsed = benchmark_plr(100000);
    assert(elapsed >= 0.0);
    printf("    PLR benchmark: 100k cycles in %.4f s\n", elapsed);
    TEST("PLR benchmark");
}

/* ── Main ────────────────────────────────────────────────────────────── */

int main(void) {
    printf("=== flux-algebra tests ===\n\n");

    /* Original */
    test_major_chord();
    test_minor_chord();
    test_dim_aug();
    test_transpose();
    test_parallel();
    test_relative();
    test_leading_tone();
    test_tropical();
    test_et_tuning();
    test_just_tuning();
    test_voice_leading();

    /* New */
    test_chord_is_diminished();
    test_chord_is_augmented();
    test_chord_eq();
    test_chord_contains();
    test_tropical_idempotent();
    test_tropical_pow();
    test_plr_compose();
    test_plr_compose_pp();
    test_voice_leading_self();
    test_voice_leading_optimal();
    test_voice_leading_null();
    test_transpose_identity();
    test_transpose_octave();
    test_et_tuning_octave();
    test_just_tuning_fifth();
    test_tuning_null();
    test_parallel_roundtrip();
    test_relative_roundtrip();
    test_leading_tone_roundtrip();
    test_all_major_chords();
    test_all_minor_chords();
    test_benchmark_plr();

    printf("\n✅ All %d tests passed!\n", test_count);
    return 0;
}
