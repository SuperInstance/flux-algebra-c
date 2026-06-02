#define _POSIX_C_SOURCE 199309L
#include "flux_algebra.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
    if (!c || c->count < 3) return 0;
    int iv1 = (c->roots[1] - c->roots[0] + 12) % 12;
    int iv2 = (c->roots[2] - c->roots[1] + 12) % 12;
    return iv1 == 4 && iv2 == 3;
}

int chord_is_minor(Chord *c) {
    if (!c || c->count < 3) return 0;
    int iv1 = (c->roots[1] - c->roots[0] + 12) % 12;
    int iv2 = (c->roots[2] - c->roots[1] + 12) % 12;
    return iv1 == 3 && iv2 == 4;
}

int chord_is_diminished(Chord *c) {
    if (!c || c->count < 3) return 0;
    int iv1 = (c->roots[1] - c->roots[0] + 12) % 12;
    int iv2 = (c->roots[2] - c->roots[1] + 12) % 12;
    return iv1 == 3 && iv2 == 3;
}

int chord_is_augmented(Chord *c) {
    if (!c || c->count < 3) return 0;
    int iv1 = (c->roots[1] - c->roots[0] + 12) % 12;
    int iv2 = (c->roots[2] - c->roots[1] + 12) % 12;
    return iv1 == 4 && iv2 == 4;
}

Chord chord_transpose(Chord *c, int semitones) {
    Chord result;
    if (!c) { result.count = 0; return result; }
    result = *c;
    for (int i = 0; i < c->count; i++) {
        result.roots[i] = (c->roots[i] + semitones % 12 + 12) % 12;
    }
    return result;
}

int chord_eq(Chord *a, Chord *b) {
    if (!a || !b) return 0;
    if (a->count != b->count) return 0;
    for (int i = 0; i < a->count; i++) {
        if (a->roots[i] != b->roots[i]) return 0;
    }
    return 1;
}

int chord_contains(Chord *c, int pitch) {
    if (!c) return 0;
    pitch = pitch % 12;
    for (int i = 0; i < c->count; i++) {
        if (c->roots[i] == pitch) return 1;
    }
    return 0;
}

/* ── PLR group ───────────────────────────────────────────────────────── */

Chord parallel(Chord *c) {
    if (!c) return chord_major(0);
    if (chord_is_major(c)) return chord_minor(c->roots[0]);
    if (chord_is_minor(c)) return chord_major(c->roots[0]);
    return *c;
}

Chord leading_tone(Chord *c) {
    if (!c) return chord_major(0);
    if (chord_is_major(c)) return chord_minor((c->roots[0] + 11) % 12);
    if (chord_is_minor(c)) return chord_major((c->roots[0] + 1) % 12);
    return *c;
}

Chord relative(Chord *c) {
    if (!c) return chord_major(0);
    if (chord_is_major(c)) return chord_minor((c->roots[0] + 9) % 12);
    if (chord_is_minor(c)) return chord_major((c->roots[0] + 3) % 12);
    return *c;
}

Chord plr_compose(Chord *c, const char *ops, int n) {
    Chord result = *c;
    for (int i = 0; i < n; i++) {
        switch (ops[i]) {
            case 'P': case 'p': result = parallel(&result); break;
            case 'L': case 'l': result = leading_tone(&result); break;
            case 'R': case 'r': result = relative(&result); break;
            default: break;
        }
    }
    return result;
}

/* ── Tropical semiring ──────────────────────────────────────────────── */

double tropical_add(double a, double b) { return a < b ? a : b; }
double tropical_mul(double a, double b) { return a + b; }

double tropical_pow(double a, int n) {
    if (n <= 0) return 0.0;
    return (double)n * a;
}

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
    if (!t) return 0.0;
    int pc = ((midi - t->base_midi) % 12 + 12) % 12;
    int octaves = (midi - t->base_midi) / 12;
    return t->base_freq * t->ratios[pc] * pow(2.0, octaves);
}

double tuning_cents_deviation(TuningField *t, int midi) {
    if (!t) return 0.0;
    double ji = tuning_frequency(t, midi);
    double et = 261.626 * pow(2.0, (midi - 60) / 12.0);
    if (et <= 0.0) return 0.0;
    return 1200.0 * log2(ji / et);
}

/* ── Voice leading ──────────────────────────────────────────────────── */

double voice_leading_distance(Chord *a, Chord *b) {
    if (!a || !b || a->count != b->count) return 1e9;
    double dist = 0.0;
    for (int i = 0; i < a->count; i++) {
        int diff = abs(a->roots[i] - b->roots[i]);
        int wrapped = 12 - diff;
        dist += (diff < wrapped) ? (double)diff : (double)wrapped;
    }
    return dist;
}

static void swap_int(int *a, int *b) {
    int tmp = *a; *a = *b; *b = tmp;
}

double voice_leading_optimal(Chord *a, Chord *b) {
    if (!a || !b || a->count != b->count || a->count > 4) return 1e9;
    /* Try all permutations of b */
    int perm[4];
    for (int i = 0; i < a->count; i++) perm[i] = b->roots[i];
    double best = 1e9;
    int n = a->count;

    /* For small n, enumerate permutations */
    if (n == 1) {
        int diff = abs(a->roots[0] - perm[0]);
        int wrapped = 12 - diff;
        return (diff < wrapped) ? (double)diff : (double)wrapped;
    }
    if (n == 2) {
        /* 2 permutations */
        for (int flip = 0; flip < 2; flip++) {
            double d = 0;
            for (int i = 0; i < n; i++) {
                int diff = abs(a->roots[i] - perm[i]);
                int wrapped = 12 - diff;
                d += (diff < wrapped) ? (double)diff : (double)wrapped;
            }
            if (d < best) best = d;
            swap_int(&perm[0], &perm[1]);
        }
    } else if (n == 3) {
        /* 6 permutations */
        int p[3] = {b->roots[0], b->roots[1], b->roots[2]};
        int orders[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
        for (int o = 0; o < 6; o++) {
            double d = 0;
            for (int i = 0; i < 3; i++) {
                int diff = abs(a->roots[i] - p[orders[o][i]]);
                int wrapped = 12 - diff;
                d += (diff < wrapped) ? (double)diff : (double)wrapped;
            }
            if (d < best) best = d;
        }
    }
    return best;
}

/* ── Benchmark ──────────────────────────────────────────────────────── */

double benchmark_plr(int iterations) {
    Chord c = chord_major(0);
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < iterations; i++) {
        c = parallel(&c);
        c = leading_tone(&c);
        c = relative(&c);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    (void)c;
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}
