# flux-algebra-c

C99 port of [flux-algebra](https://github.com/SuperInstance/flux-algebra) — musical algebra with chord operations, Neo-Riemannian PLR group, tropical semiring, tuning fields, and voice-leading distance.

## What This Gives You

- **Chord operations** — Major/minor/diminished/augmented construction and transposition
- **PLR group** — Parallel, Leading-tone, Relative (Neo-Riemannian transformations)
- **Tropical semiring** — Min-plus algebra over intervals (ℝ ∪ {∞}, min, +)
- **Tuning fields** — Equal temperament, just intonation, cents deviation
- **Voice leading** — Distance computation between chord pairs
- **Zero dependencies** — C99, only `<math.h>`

## Quick Start

```c
#include "flux_algebra.h"

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
```

## License

MIT

Part of the [SuperInstance OpenConstruct](https://github.com/SuperInstance) ecosystem.
