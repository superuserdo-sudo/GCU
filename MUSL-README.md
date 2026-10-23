# GCU on musl

GCU keeps its C++20 implementation and Gill command names, with a separate
static musl build profile intended for GilluX. It does not use BusyBox.

Start with `docs/musl-build.txt`, then run:

```sh
./build-musl.sh
./test-musl.sh
```

The normal `build.sh` and `build/` output remain separate. A musl-targeting
C++ compiler and its matching C++ standard library are required; a normal
NixOS `clang++` alone is not sufficient. This source archive has not been
compiled here because a musl C++ toolchain/sysroot is not available in the
build environment.
