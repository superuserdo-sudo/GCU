#!/bin/sh
set -eu
name=$1
"$CXX" $CXXFLAGS -I"$ROOT/include" "$ROOT/src/commands/$name.cpp" -L"$LIB" -lgcu -Wl,-rpath,'$ORIGIN/../lib' -o "$BIN/$name"
