#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
DEST=${1:?usage: install-to-root.sh ROOTFS}
[ -d "$ROOT/build/bin" ] || { echo "build first with ./build.sh" >&2; exit 1; }
mkdir -p "$DEST/bin" "$DEST/lib" "$DEST/usr/share/gcu/shell"
install -m 0755 "$ROOT/build/lib/libgcu.so" "$DEST/lib/libgcu.so"
for bin in "$ROOT"/build/bin/*; do
    [ -f "$bin" ] || continue
    install -m 0755 "$bin" "$DEST/bin/${bin##*/}"
done
if [ -d "$ROOT/share/gcu/shell" ]; then
    for file in "$ROOT"/share/gcu/shell/*; do
        [ -f "$file" ] || continue
        install -m 0644 "$file" "$DEST/usr/share/gcu/shell/${file##*/}"
    done
fi
printf 'Installed %s GCU executables into staging root: %s\n' "$(find "$ROOT/build/bin" -maxdepth 1 -type f | wc -l | tr -d ' ')" "$DEST"
printf 'Runtime library: %s/lib/libgcu.so\n' "$DEST"
printf 'Shell integrations: %s/usr/share/gcu/shell\n' "$DEST"
