#include "flux_algebra.h"
#include <stdlib.h>

/* ── Chord operations ────────────────────────────────────────────────── */

Chord chord_major(int root) {
    Chord c;
    c.roots[0] = root % 12;
    c.roots[1] = (root + 4) % 12;
    c.roots[2] = (root + 7) % 12;
    c.count = 3;
    return c;
}

Chord chord_minor(int root) {
    Chord c;
    c.roots[0] = root % 12;
    c.roots[1] = (root + 3) % 12;
    c.roots[2] = (root + 7) % 12;
    c.count = 3;
    return c;
}

Chord chord_diminished(int root) {
    Chord c;
    c.roots[0] = root % 12;
    c.roots[1] = (root + 3) % 12;
    c.roots[2] = (root + 6) % 12;
    c.count = 3;
    return c;
}

Chord chord_augmented(int root) {
    Chord c;
    c.roots[0] = root % 12;
    c.roots[1] = (root + 4) % 12;
    c.roots[2] = (root + 8) % 12;
    c.count = 3;
    return c;
}

int chord_is_major(Chord *c) {
    if (c->count < 3) return 0;
    int iv1 = (c->roots[1] - c->roots[0] + 12) % 12;
    int iv2 = (c->roots[2] - c->roots[1] + 12) % 12;
    return iv1 == 4 && iv2 == 3;
}

int chord_is_minor(Chord *c) {
    if (c->count < 3) return 0;
    int iv1 = (c->roots[1] - c->roots[0] + 12) % 12;
    int iv2 = (c->roots[2] - c->roots[1] + 12) % 12;
    return iv1 == 3 && iv2 == 4;
}

Chord chord_transpose(Chord *c, int semitones) {
    Chord result = *c;
    for (int i = 0; i < c->count; i++) {
        result.roots[i] = (c->roots[i] + semitones % 12 + 12) % 12;
    }
    return result;
}

/* ── PLR group ───────────────────────────────────────────────────────── */

Chord parallel(Chord *c) {
    if (chord_is_major(c)) return chord_minor(c->roots[0]);
    if (chord_is_minor(c)) return chord_major(c->roots[0]);
    return *c;
}

Chord leading_tone(Chord *c) {
    if (chord_is_major(c)) return chord_minor((c->roots[0] + 11) % 12);
    if (chord_is_minor(c)) return chord_major((c->roots[0] + 1) % 12);
    return *c;
}

Chord relative(Chord *c) {
    if (chord_is_major(c)) return chord_minor((c->roots[0] + 9) % 12);
    if (chord_is_minor(c)) return chord_major((c->roots[0] + 3) % 12);
    return *c;
}

/* ── Tropical semiring ──────────────────────────────────────────────── */

double tropical_add(double a, double b) { return a < b ? a : b; }
double tropical_mul(double a, double b) { return a + b; }

/* ── Tuning fields ──────────────────────────────────────────────────── */

TuningField tuning_equal_temperament(void) {
    TuningField t;
    t.base_freq = 261.626;
    t.base_midi = 60;
    for (int i = 0; i < 12; i++) t.ratios[i] = pow(2.0, i / 12.0);
    return t;
}

TuningField tuning_just_intonation(void) {
    TuningField t;
    t.base_freq = 261.626;
    t.base_midi = 60;
    double ji[] = {1.0, 16.0/15, 9.0/8, 6.0/5, 5.0/4, 4.0/3, 45.0/32, 3.0/2, 8.0/5, 5.0/3, 9.0/5, 15.0/8};
    for (int i = 0; i < 12; i++) t.ratios[i] = ji[i];
    return t;
}

double tuning_frequency(TuningField *t, int midi) {
    int pc = ((midi - t->base_midi) % 12 + 12) % 12;
    int octaves = (midi - t->base_midi) / 12;
    return t->base_freq * t->ratios[pc] * pow(2.0, octaves);
}

double tuning_cents_deviation(TuningField *t, int midi) {
    double ji = tuning_frequency(t, midi);
    double et = 261.626 * pow(2.0, (midi - 60) / 12.0);
    if (et <= 0.0) return 0.0;
    return 1200.0 * log2(ji / et);
}

/* ── Voice leading ──────────────────────────────────────────────────── */

double voice_leading_distance(Chord *a, Chord *b) {
    if (a->count != b->count) return 1e9;
    double dist = 0.0;
    for (int i = 0; i < a->count; i++) {
        int diff = abs(a->roots[i] - b->roots[i]);
        int wrapped = 12 - diff;
        dist += (diff < wrapped) ? diff : wrapped;
    }
    return dist;
}
