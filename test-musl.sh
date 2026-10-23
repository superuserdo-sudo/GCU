#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
BIN="$ROOT/build-musl/rootfs/bin"
READELF=${READELF:-readelf}
[ -d "$BIN" ] || { echo "Missing $BIN; run ./build-musl.sh first." >&2; exit 1; }
command -v "$READELF" >/dev/null 2>&1 || { echo "readelf is required for verification." >&2; exit 2; }
count=0
failed=0
for bin in "$BIN"/*; do
    [ -f "$bin" ] || continue
    count=$((count + 1))
    if "$READELF" -l "$bin" 2>/dev/null | grep -q 'INTERP'; then
        echo "FAIL: dynamic interpreter present: ${bin##*/}" >&2
        failed=1
    fi
    if "$READELF" -d "$bin" 2>/dev/null | grep -q '(NEEDED)'; then
        echo "FAIL: shared-library dependency present: ${bin##*/}" >&2
        failed=1
    fi
done
[ "$count" -gt 0 ] || { echo "No command binaries found." >&2; exit 1; }
[ "$failed" -eq 0 ] || exit 1
printf 'PASS: checked %s executables; no ELF interpreter or shared-library dependencies found.\n' "$count"
printf 'Note: ELF checks verify static linkage, not complete runtime correctness.\n'
