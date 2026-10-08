#!/usr/bin/env bash
set -euo pipefail
root=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
mkdir -p "$root/.retrom-build"
work=$(mktemp -d "$root/.retrom-build/checkpoint-test.XXXXXX")
trap 'rm -rf "$work"' EXIT INT TERM
cc -std=gnu99 -D__LIBRETRO__ -DFRONTEND_SUPPORTS_RGB565 \
  -ffunction-sections -fdata-sections \
  -I"$root/src/burn" -I"$root/src/burner/libretro" \
  -I"$root/src/burner/libretro/tchar" \
  -I"$root/src/burner/libretro/libretro-common/include" \
  -I"$root/src/cpu" -I"$root/src/burn/devices" -I"$root/src/burn/snd" \
  -I"$root/src/intf/input" \
  "$root/tests/checkpoint-palette.c" "$root/src/burn/drv/capcom/cps_pal.c" \
  -Wl,--gc-sections -o "$work/checkpoint-test"
"$work/checkpoint-test"
