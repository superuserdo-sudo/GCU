#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
mkdir -p "$TMP/one" "$TMP/two" "$TMP/home with spaces"
for shell in bash dash; do
    if command -v "$shell" >/dev/null 2>&1; then
        HOME="$TMP/home with spaces" "$shell" -c '
            . "$1/share/gcu/shell/pmad.sh"
            pmad "$2/one"
            [ "$PWD" = "$2/one" ] || exit 11
            pmad "$2/two"
            [ "$PWD" = "$2/two" ] || exit 12
            pmad -
            [ "$PWD" = "$2/one" ] || exit 13
            pmad --help >/dev/null
            pmad --version >/dev/null
            pmad
            [ "$PWD" = "$HOME" ] || exit 14
        ' pmad-test "$ROOT" "$TMP"
    fi
done
printf 'pmad shell integration tests passed\n'
