# flux-algebra-c

C port of [flux-algebra](https://github.com/SuperInstance/flux-algebra) — musical algebra for harmonic analysis.

## Features

- **Chord operations**: major/minor/diminished/augmented, transposition
- **PLR group**: Parallel, Leading-tone, Relative (Neo-Riemannian)
- **Tropical semiring**: min-plus algebra over intervals
- **Tuning fields**: equal temperament, just intonation, cents deviation
- **Voice leading**: distance computation between chord pairs

## Build & Test

```bash
make test
```

## API

```c
#include "flux_algebra.h"

Chord cmaj = chord_major(0);          /* C major */
Chord amin = relative(&cmaj);         /* A minor (relative) */
Chord bmin = leading_tone(&cmaj);     /* B minor (leading tone) */
Chord cmin = parallel(&cmaj);         /* C minor (parallel) */

TuningField ji = tuning_just_intonation();
double freq = tuning_frequency(&ji, 64);  /* E4 in just intonation */
double cents = tuning_cents_deviation(&ji, 64);
```

## License

MIT
