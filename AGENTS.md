# Retrom FBA2012 CPS2 fork

This repository owns the native CPS2 core and its Web artifacts. Read
`retrom-fork.json` and `.github/rpg-runtime/README.md` before changing it.

- Keep `master` as the upstream mirror. Retrom maintenance belongs to
  `retrom/g3fb5b89d2ab7`, based on `3fb5b89d2ab719e45e814e1ad0b5ff721bffdff2`.
- Use short-lived `fix/*`, `feat/*` or `build/*` branches in the named PFB.
- Runtime consumes artifacts; it must not compile or patch this core.
- Run `.github/rpg-runtime/test-checkpoint.sh` and `git diff --check` for
  state changes. Explicitly build a candidate and verify normal game controls,
  saving and fresh-browser restoration before publishing.
- Preserve the original `src/license.txt` verbatim as `LICENSE`; keep component
  notices and the complete modified source in release assets.
- Do not commit ROMs, BIOS, saves, credentials or generated build outputs.
- Release only immutable annotated `retrom-core-g3fb5b89d2ab7-rN` tags from
  the maintenance branch after product validation.
