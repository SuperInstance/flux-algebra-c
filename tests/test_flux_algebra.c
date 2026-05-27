#include "flux_algebra.h"
#include <assert.h>
#include <stdio.h>

static void test_major_chord(void) {
    Chord c = chord_major(0);
    assert(c.roots[0] == 0 && c.roots[1] == 4 && c.roots[2] == 7);
    assert(chord_is_major(&c));
    printf("  ✓ major chord\n");
}

static void test_minor_chord(void) {
    Chord c = chord_minor(0);
    assert(c.roots[0] == 0 && c.roots[1] == 3 && c.roots[2] == 7);
    assert(chord_is_minor(&c));
    printf("  ✓ minor chord\n");
}

static void test_transpose(void) {
    Chord c = chord_major(0);
    Chord d = chord_transpose(&c, 2);
    assert(d.roots[0] == 2 && d.roots[1] == 6 && d.roots[2] == 9);
    printf("  ✓ transpose\n");
}

static void test_parallel(void) {
    Chord cmaj = chord_major(0);
    Chord cmin = parallel(&cmaj);
    assert(chord_is_minor(&cmin));
    assert(cmin.roots[0] == 0);
    printf("  ✓ parallel (P)\n");
}

static void test_relative(void) {
    Chord cmaj = chord_major(0);
    Chord amin = relative(&cmaj);
    assert(chord_is_minor(&amin));
    assert(amin.roots[0] == 9);
    printf("  ✓ relative (R)\n");
}

static void test_leading_tone(void) {
    Chord cmaj = chord_major(0);
    Chord bmin = leading_tone(&cmaj);
    assert(chord_is_minor(&bmin));
    assert(bmin.roots[0] == 11);
    printf("  ✓ leading tone (L)\n");
}

static void test_tropical(void) {
    assert(fabs(tropical_add(3.0, 5.0) - 3.0) < 0.001);
    assert(fabs(tropical_mul(3.0, 5.0) - 8.0) < 0.001);
    printf("  ✓ tropical semiring\n");
}

static void test_et_tuning(void) {
    TuningField t = tuning_equal_temperament();
    assert(fabs(tuning_frequency(&t, 69) - 440.0) < 0.01);
    printf("  ✓ equal temperament tuning\n");
}

static void test_just_tuning(void) {
    TuningField t = tuning_just_intonation();
    double cents = tuning_cents_deviation(&t, 64); /* E4 */
    assert(fabs(cents) < 50.0);
    printf("  ✓ just intonation (E4 cents=%.2f)\n", cents);
}

static void test_voice_leading(void) {
    Chord cmaj = chord_major(0);
    Chord dmaj = chord_major(2);
    double dist = voice_leading_distance(&cmaj, &dmaj);
    assert(dist > 0.0);
    printf("  ✓ voice leading distance (C→D = %.1f)\n", dist);
}

static void test_dim_aug(void) {
    Chord dim = chord_diminished(0);
    assert(dim.roots[0] == 0 && dim.roots[1] == 3 && dim.roots[2] == 6);
    Chord aug = chord_augmented(0);
    assert(aug.roots[0] == 0 && aug.roots[1] == 4 && aug.roots[2] == 8);
    printf("  ✓ diminished and augmented chords\n");
}

int main(void) {
    printf("=== flux-algebra tests ===\n\n");
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
    printf("\n✅ All 11 tests passed!\n");
    return 0;
}
