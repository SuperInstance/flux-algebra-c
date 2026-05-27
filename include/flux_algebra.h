#ifndef FLUX_ALGEBRA_H
#define FLUX_ALGEBRA_H

#include <math.h>

/* ── Chord operations ────────────────────────────────────────────────── */

typedef struct {
    int roots[4];
    int count;
} Chord;

Chord chord_major(int root);
Chord chord_minor(int root);
Chord chord_diminished(int root);
Chord chord_augmented(int root);
int chord_is_major(Chord *c);
int chord_is_minor(Chord *c);
Chord chord_transpose(Chord *c, int semitones);

/* ── PLR group (Neo-Riemannian) ──────────────────────────────────────── */

Chord parallel(Chord *c);
Chord leading_tone(Chord *c);
Chord relative(Chord *c);

/* ── Tropical semiring ──────────────────────────────────────────────── */

double tropical_add(double a, double b);
double tropical_mul(double a, double b);

/* ── Tuning fields ──────────────────────────────────────────────────── */

typedef struct {
    double base_freq;
    int base_midi;
    double ratios[12];
} TuningField;

TuningField tuning_equal_temperament(void);
TuningField tuning_just_intonation(void);
double tuning_frequency(TuningField *t, int midi);
double tuning_cents_deviation(TuningField *t, int midi);

/* ── Voice leading ──────────────────────────────────────────────────── */

double voice_leading_distance(Chord *a, Chord *b);

#endif
