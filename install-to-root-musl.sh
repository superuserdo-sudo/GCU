#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
DEST=${1:?usage: install-to-root-musl.sh ROOTFS}
BIN="$ROOT/build-musl/rootfs/bin"
[ -d "$BIN" ] || { echo "Build first with ./build-musl.sh" >&2; exit 1; }
mkdir -p "$DEST/bin" "$DEST/usr/share/gcu/shell"
count=0
for bin in "$BIN"/*; do
    [ -f "$bin" ] || continue
    install -m 0755 "$bin" "$DEST/bin/${bin##*/}"
    count=$((count + 1))
done
if [ -d "$ROOT/share/gcu/shell" ]; then
    for file in "$ROOT"/share/gcu/shell/*; do
        [ -f "$file" ] || continue
        install -m 0644 "$file" "$DEST/usr/share/gcu/shell/${file##*/}"
    done
fi
printf 'Installed %s GCU musl build executables into staging root: %s\n' "$count" "$DEST"
printf 'No libgcu.so is needed for the static build.\n'
