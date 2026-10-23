#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BIN="$ROOT/build/bin"
TMP=${TMPDIR:-/tmp}/gcu-smoke-$$
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
printf 'alpha\n\nbeta\n' > "$TMP/in"
"$BIN/cowf" "$TMP/in" > "$TMP/out"
cmp "$TMP/in" "$TMP/out"
"$BIN/cowf" -num "$TMP/in" > "$TMP/out"
grep -q 'alpha' "$TMP/out"
printf 'a\nb\nc\n' | "$BIN/fcow" > "$TMP/out"
printf 'c\nb\na\n' > "$TMP/expected"
cmp "$TMP/expected" "$TMP/out"
printf 'Gill GCU\n' | "$BIN/b64" > "$TMP/out"
"$BIN/b64" --decode "$TMP/out" > "$TMP/decoded"
printf 'Gill GCU\n' > "$TMP/expected"
cmp "$TMP/expected" "$TMP/decoded"
printf 'a\nb\n' | "$BIN/warp" -wid 1 > "$TMP/out"
"$BIN/wami" > "$TMP/out"
"$BIN/mos" > "$TMP/out"
printf 'GCU smoke tests passed\n'
