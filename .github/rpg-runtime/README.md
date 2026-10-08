# Retrom FBA2012 CPS2 Web build

The core baseline is `3fb5b89d2ab719e45e814e1ad0b5ff721bffdff2` from
EmulatorJS/fbalpha2012_cps2. The frontend linker is EmulatorJS/RetroArch
`6dd4353937ef48b6ec0bfbdbb15d1c5992d86927`; `retrom-fork.json` owns these pins.

CPS2 palette DMA latches colors separately from video RAM. Instant checkpoints
must save `CpsSavePal` so a fresh instance can rebuild the same palette. Run
`.github/rpg-runtime/test-checkpoint.sh` to exercise the actual native RAM scan
and palette converter, including a fresh allocation and different DMA source
RAM. The unchanged baseline fails the latch round-trip assertion.

Build with Retrom's explicit `pfb-core-build CORE=fbalpha2012_cps2`, or invoke
`.github/rpg-runtime/build-candidate.sh /absolute/empty/output`. The recipe
snapshots source, runs the native regression, compiles inside the pinned
Emscripten image and verifies that the source did not change during the build.
No game or external BIOS is included. The candidate keeps the existing
EmulatorJS 4.2.3 loader contract and single-threaded core behavior.

`LICENSE` is a byte-for-byte copy of upstream `src/license.txt`, including
FB Alpha and Final Burn's original restrictions. Component notices remain at
their original paths in the complete source archive. Do not relabel this
source as GPL or remove its original terms.

After actual Retrom review/play/save/fresh-browser restore validation, annotated
`retrom-core-g3fb5b89d2ab7-rN` tags build and publish the same closed asset set,
including source, license and immutable release metadata.
