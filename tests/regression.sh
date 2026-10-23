#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BIN="$ROOT/build/bin"
if [ ! -x "$BIN/dblk" ] || [ ! -x "$BIN/sh256" ]; then echo "GCU regression tests require PROFILE=full"; exit 0; fi
TMP=${TMPDIR:-/tmp}/gcu-regression-$$
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

for cmd in $(cat "$ROOT/commands.txt"); do
    "$BIN/$cmd" --version >/dev/null
    "$BIN/$cmd" --help >/dev/null
done

printf 'abc' | "$BIN/sh256" - | grep -q '^ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad  -$'
printf 'abc' | "$BIN/sh224" - | grep -q '^23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7  -$'
printf 'abc' | "$BIN/sh384" - | grep -q '^cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7  -$'
printf 'abc' | "$BIN/sh512" - | grep -q '^ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f  -$'
printf 'abc' | "$BIN/sh1c" - | grep -q '^a9993e364706816aba3e25717850c26c9cd0d89d  -$'
printf 'abc' | "$BIN/md5c" - | grep -q '^900150983cd24fb0d6963f7d28e17f72  -$'
printf 'abc' | "$BIN/b2ck" - | grep -q '^ba80a53f981c4d0d6a2797b69f12f6e94c212f14685ac4b74b12bb6fdbffa2d17d87c5392aab792dc252d5de4533cc9518d38aa8dbf1925ab92386edd4009923  -$'
printf 'abc' | "$BIN/cksm" - | grep -q '^1219131554 3 -$'
printf 'abc' > "$TMP/hash-input"
"$BIN/sh256" "$TMP/hash-input" > "$TMP/hash-manifest"
"$BIN/sh256" --check "$TMP/hash-manifest" | grep -q 'OK$'

printf 'one:two:three\nx:y\n' | "$BIN/fldx" -d ':' -f '1,3' > "$TMP/cut"
printf 'one:three\nx\n' > "$TMP/expected"
cmp "$TMP/expected" "$TMP/cut"
printf 'b\na\nb\n' | "$BIN/ordf" > "$TMP/sorted"
printf 'a\nb\nb\n' > "$TMP/expected"
cmp "$TMP/expected" "$TMP/sorted"
printf 'x\nx\ny\n' | "$BIN/dupf" -c > "$TMP/uniq"
grep -q '2 x' "$TMP/uniq"
printf 'a\nb\nc\n' | "$BIN/seqn" 3 > "$TMP/seq"
printf '1\n2\n3\n' > "$TMP/expected"
cmp "$TMP/expected" "$TMP/seq"

printf 'hello' > "$TMP/file"
"$BIN/permf" u+x "$TMP/file"
[ -x "$TMP/file" ]
"$BIN/truncf" -s 2 "$TMP/file"
[ "$(wc -c < "$TMP/file")" -eq 2 ]
"$BIN/cop" "$TMP/file" "$TMP/copied"
cmp "$TMP/file" "$TMP/copied"
"$BIN/dblk" if="$TMP/file" of="$TMP/ddcopy" bs=1
cmp "$TMP/file" "$TMP/ddcopy"
"$BIN/mdir" "$TMP/directory"
printf 'erase-me' > "$TMP/wipf-test"
"$BIN/wipf" -n 1 -z "$TMP/wipf-test"
[ "$(wc -c < "$TMP/wipf-test")" -eq 8 ]
"$BIN/rom" -r "$TMP/directory"
[ ! -e "$TMP/directory" ]

printf 'abc' | "$BIN/mout" "$TMP/tee1" "$TMP/tee2" > "$TMP/teeout"
cmp "$TMP/tee1" "$TMP/tee2"
cmp "$TMP/tee1" "$TMP/teeout"
"$ROOT/tests/pmad-shells.sh"
printf 'GCU regression tests passed\n'
