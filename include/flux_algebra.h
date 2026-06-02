#ifndef FLUX_ALGEBRA_H
#define FLUX_ALGEBRA_H

#include <math.h>
#include <stddef.h>

/* ── Chord operations ────────────────────────────────────────────────── */

/** A chord represented as pitch classes (0-11) with up to 4 notes */
typedef struct {
    int roots[4];  /**< Pitch class of each note (0=C, 1=C#, ...) */
    int count;     /**< Number of notes in the chord */
} Chord;

/**
 * Build a major triad from a root pitch class.
 * Root, +4 semitones (M3), +7 semitones (P5)
 */
Chord chord_major(int root);

/**
 * Build a minor triad from a root pitch class.
 * Root, +3 semitones (m3), +7 semitones (P5)
 */
Chord chord_minor(int root);

/**
 * Build a diminished triad from a root pitch class.
 * Root, +3 semitones (m3), +6 semitones (d5)
 */
Chord chord_diminished(int root);

/**
 * Build an augmented triad from a root pitch class.
 * Root, +4 semitones (M3), +8 semitones (A5)
 */
Chord chord_augmented(int root);

/**
 * Check if a chord is a major triad.
 */
int chord_is_major(Chord *c);

/**
 * Check if a chord is a minor triad.
 */
int chord_is_minor(Chord *c);

/**
 * Check if a chord is a diminished triad.
 */
int chord_is_diminished(Chord *c);

/**
 * Check if a chord is an augmented triad.
 */
int chord_is_augmented(Chord *c);

/**
 * Transpose a chord by a number of semitones.
 */
Chord chord_transpose(Chord *c, int semitones);

/**
 * Check if two chords are equal (same pitch classes).
 */
int chord_eq(Chord *a, Chord *b);

/**
 * Check if a chord contains a given pitch class.
 * @param c      Chord to search
 * @param pitch  Pitch class (0-11)
 * @return       1 if found, 0 otherwise
 */
int chord_contains(Chord *c, int pitch);

/* ── PLR group (Neo-Riemannian) ──────────────────────────────────────── */

/**
 * Parallel (P) transformation: major ↔ minor, same root.
 * C major → C minor
 */
Chord parallel(Chord *c);

/**
 * Leading-tone (L) transformation: major ↔ minor via leading tone exchange.
 * C major → E minor (root moves down by half step from 5th)
 * A minor → F major (root moves up by half step)
 */
Chord leading_tone(Chord *c);

/**
 * Relative (R) transformation: major ↔ relative minor and vice versa.
 * C major → A minor
 * A minor → C major
 */
Chord relative(Chord *c);

/**
 * PLR composition: apply a sequence of transformations.
 * @param c     Starting chord
 * @param ops   String of 'P', 'L', 'R' characters (e.g., "PLR")
 * @param n     Length of ops string
 * @return      Resulting chord
 */
Chord plr_compose(Chord *c, const char *ops, int n);

/* ── Tropical semiring ──────────────────────────────────────────────── */

/**
 * Tropical addition (min): a ⊕ b = min(a, b).
 */
double tropical_add(double a, double b);

/**
 * Tropical multiplication (addition): a ⊗ b = a + b.
 */
double tropical_mul(double a, double b);

/**
 * Tropical power: a^(⊗n) = n * a (repeated tropical multiplication).
 * @param a  Base
 * @param n  Exponent (must be >= 0)
 * @return   a added to itself n times
 */
double tropical_pow(double a, int n);

/**
 * Check if tropical arithmetic is idempotent: a ⊕ a = a.
 * Always true for the min-plus semiring.
 */
static inline int tropical_is_idempotent(double a) {
    return tropical_add(a, a) == a;
}

/* ── Tuning fields ──────────────────────────────────────────────────── */

/** A tuning system defining frequency ratios for 12 pitch classes */
typedef struct {
    double base_freq;    /**< Reference frequency (e.g., 261.626 Hz for C4) */
    int base_midi;       /**< MIDI note number of the reference */
    double ratios[12];   /**< Frequency ratio for each pitch class */
} TuningField;

/**
 * Equal temperament tuning (2^(n/12) ratios).
 */
TuningField tuning_equal_temperament(void);

/**
 * Just intonation tuning (integer ratio approximations).
 */
TuningField tuning_just_intonation(void);

/**
 * Compute the frequency for a MIDI note in a given tuning.
 * @param t     Tuning field
 * @param midi  MIDI note number
 * @return      Frequency in Hz
 */
double tuning_frequency(TuningField *t, int midi);

/**
 * Compute cents deviation from equal temperament for a MIDI note.
 * @param t     Tuning field
 * @param midi  MIDI note number
 * @return      Deviation in cents
 */
double tuning_cents_deviation(TuningField *t, int midi);

/* ── Voice leading ──────────────────────────────────────────────────── */

/**
 * Compute the voice leading distance between two chords.
 * Uses wrapped semitone distance (min of up/down interval).
 * @param a  First chord
 * @param b  Second chord
 * @return   Sum of minimal semitone distances per voice
 */
double voice_leading_distance(Chord *a, Chord *b);

/**
 * Find the optimal voice leading by trying all permutations.
 * For triads (3 notes), tests all 6 permutations.
 * @param a  First chord
 * @param b  Second chord
 * @return   Minimal voice leading distance
 */
double voice_leading_optimal(Chord *a, Chord *b);

/* ── Benchmark ──────────────────────────────────────────────────────── */

/**
 * Run a benchmark of PLR transformations.
 * @param iterations  Number of PLR cycles to run
 * @return            Elapsed time in seconds
 */
double benchmark_plr(int iterations);

#endif
