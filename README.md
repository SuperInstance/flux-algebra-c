# flux-algebra-c

<<<<<<< HEAD
C99 port of [flux-algebra](https://github.com/SuperInstance/flux-algebra) — musical algebra with chord operations, Neo-Riemannian PLR group, tropical semiring, tuning fields, and voice-leading distance.

## What This Gives You

- **Chord operations** — Major/minor/diminished/augmented construction and transposition
- **PLR group** — Parallel, Leading-tone, Relative (Neo-Riemannian transformations)
- **Tropical semiring** — Min-plus algebra over intervals (ℝ ∪ {∞}, min, +)
- **Tuning fields** — Equal temperament, just intonation, cents deviation
- **Voice leading** — Distance computation between chord pairs
- **Zero dependencies** — C99, only `<math.h>`
=======
C port of algebraic music theory — harmonic rings (Z/nZ), Neo-Riemannian PLR group, tropical semiring, tuning fields, and voice leading. Zero dependencies, C99.

## What This Gives You

- **Harmonic ring** — Z/nZ ring operations for pitch-class arithmetic
- **PLR group** — Parallel, Leading-tone, Relative transformations (Neo-Riemannian theory)
- **Tropical semiring** — min-plus algebra for voice-leading distances
- **Tuning fields** — equal temperament and just intonation with cent calculations
- **Voice leading** — minimal voice-leading distance between chords
- **Zero dependencies** — pure C99, one header file
>>>>>>> 54a0f0b (docs: world-class README audit and rewrite)

## Quick Start

```c
#include "flux_algebra.h"

<<<<<<< HEAD
Chord cmaj = chord_major(0);          /* C major */
Chord amin = relative(&cmaj);         /* A minor (relative) */
Chord bmin = leading_tone(&cmaj);     /* B diminished (leading tone) */
Chord cmin = parallel(&cmaj);         /* C minor (parallel) */

/* Tuning systems */
TuningField ji = tuning_just_intonation();
double freq = tuning_frequency(&ji, 64);   /* E4 in just intonation */
double cents = tuning_cents_deviation(&ji, 64);

/* Tropical semiring: min-plus arithmetic */
double result = tropical_add(3.0, 5.0);    /* = 3.0 (min) */
double product = tropical_mul(3.0, 5.0);   /* = 8.0 (+) */

/* Voice-leading distance */
double d = voice_leading_distance(&cmaj, &cmin);
```

## Build & Test

```bash
make            # build static library
make test       # build and run tests
make clean
```

## API Reference

| Function | Description |
|----------|-------------|
| `chord_major(root)` / `chord_minor(root)` | Construct triads |
| `parallel(&chord)` / `relative(&chord)` / `leading_tone(&chord)` | Neo-Riemannian PLR |
| `tuning_equal_temperament()` / `tuning_just_intonation()` | Create tuning fields |
| `tuning_frequency(&field, midi)` | Frequency for MIDI note number |
| `tuning_cents_deviation(&field, midi)` | Cents deviation from ET |
| `tropical_add(a, b)` / `tropical_mul(a, b)` | Tropical semiring operations |
| `voice_leading_distance(&c1, &c2)` | Minimal voice-leading distance |

## How It Fits

- **[flux-algebra-rs](https://github.com/SuperInstance/flux-algebra-rs)** — The Rust original with additional group theory and combinatorics
- **[counterpoint-engine-c](https://github.com/SuperInstance/counterpoint-engine-c)** — Uses chord operations for counterpoint analysis
- **[creative-engine-c](https://github.com/SuperInstance/creative-engine-c)** — Map Lorenz coordinates to harmonic space via tuning fields
- **[constraint-instrument](https://github.com/SuperInstance/constraint-instrument)** — PLR transformations as constraint operations

## Testing

11 tests covering chord construction, PLR transformations, tuning computations, tropical arithmetic, and voice-leading distance.

```bash
make test
```

## Installation

```bash
git clone https://github.com/SuperInstance/flux-algebra-c.git
cd flux-algebra-c
make
=======
/* Chord operations */
Chord cmaj = chord_major(0);    /* C major: {0, 4, 7} */
Chord cmin = chord_minor(0);    /* C minor: {0, 3, 7} */

/* Neo-Riemannian transformations */
Chord par  = parallel(&cmaj);   /* C major → C minor */
Chord rel  = relative(&cmaj);   /* C major → A minor */
Chord lt   = leading_tone(&cmaj); /* C major → C♯ diminished */

/* Tropical semiring */
double d = tropical_add(3.0, 5.0);  /* min(3, 5) = 3 */
double p = tropical_mul(3.0, 5.0);  /* 3 + 5 = 8 */

/* Tuning */
TuningField et = tuning_equal_temperament();
double freq = tuning_frequency(&et, 69);  /* A440 */

/* Voice leading distance */
double dist = voice_leading_distance(&cmaj, &cmin);
>>>>>>> 54a0f0b (docs: world-class README audit and rewrite)
```

## API Reference

### Chords

| Function | Description |
|---|---|
| `chord_major(root)` | Build major triad from MIDI root |
| `chord_minor(root)` | Build minor triad |
| `chord_diminished(root)` | Build diminished triad |
| `chord_augmented(root)` | Build augmented triad |
| `chord_transpose(c, semitones)` | Transpose chord |
| `chord_is_major(c)` / `chord_is_minor(c)` | Quality check |

### PLR Group

| Function | Description |
|---|---|
| `parallel(c)` | Flip mode (major ↔ minor) |
| `leading_tone(c)` | Move leading tone |
| `relative(c)` | Relative major/minor transformation |

### Tropical Semiring

| Function | Description |
|---|---|
| `tropical_add(a, b)` | min(a, b) |
| `tropical_mul(a, b)` | a + b |

### Tuning

| Function | Description |
|---|---|
| `tuning_equal_temperament()` | 12-TET tuning field |
| `tuning_just_intonation()` | Just intonation ratios |
| `tuning_frequency(t, midi)` | MIDI note → Hz |
| `tuning_cents_deviation(t, midi)` | Cents from equal temperament |

### Voice Leading

| Function | Description |
|---|---|
| `voice_leading_distance(a, b)` | Minimal total semitone movement |

## Building

```bash
gcc -std=c99 -c flux_algebra.c -o flux_algebra.o
gcc -std=c99 test_flux_algebra.c flux_algebra.o -o test_flux_algebra
```

## How It Fits

The **C port** of the flux-algebra family:

- [flux-algebra](https://github.com/SuperInstance/flux-algebra) — original Python implementation
- [flux-algebra-rs](https://github.com/SuperInstance/flux-algebra-rs) — Rust port
- [constraint-theory-core](https://github.com/SuperInstance/constraint-theory-core) — uses algebra for constraint checking
- [constraint-substrate](https://github.com/SuperInstance/constraint-substrate) — C bindings for constraint primitives

## Testing

```bash
gcc -std=c99 -Wall -Wextra test_flux_algebra.c flux_algebra.c -lm -o test_flux_algebra
./test_flux_algebra
```

## Installation

Copy `include/flux_algebra.h` and `src/flux_algebra.c` into your project. No build system required.

## License

MIT
<<<<<<< HEAD

Part of the [SuperInstance OpenConstruct](https://github.com/SuperInstance) ecosystem.
=======
>>>>>>> 54a0f0b (docs: world-class README audit and rewrite)
