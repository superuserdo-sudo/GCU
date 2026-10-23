#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PREFIX=${1:-/usr/local}
DEST="$PREFIX/share/gcu/shell"
[ -d "$ROOT/share/gcu/shell" ] || { echo "missing shell integration files" >&2; exit 1; }
mkdir -p "$DEST"
for file in "$ROOT"/share/gcu/shell/*; do
    [ -f "$file" ] || continue
    install -m 0644 "$file" "$DEST/${file##*/}"
done
printf 'Installed pmad shell integrations to %s\n' "$DEST"
printf '%s\n' 'POSIX shells (sh/bash/zsh/ksh/ash/dash): source pmad.sh' 'fish: source pmad.fish' 'csh/tcsh: source pmad.csh' 'Nushell: source pmad.nu' 'PowerShell: . .\pmad.ps1'
