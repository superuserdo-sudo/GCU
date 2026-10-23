GCU — Gill Core Utilities
Version 0.2.0

GCU is a compact, C++20/POSIX command suite for GilluX and Gill. Its command names are Gill-specific; GNU Coreutils is used as a behavioral reference, not as copied source code.

DESIGN
- Each command is a separate executable with a fixed command identity. These are not symlinks to a basename-dispatched multicall executable.
- The implementations are linked against one shared library, libgcu.so, so command wrappers do not each contain a duplicate copy of the whole implementation.
- The default build has 110 command executables and is about 2.5 MiB in this development environment. Included binaries are built for the current x86_64 Linux environment; rebuild for a different architecture or libc/ABI. Actual installed size depends on compiler, C++ runtime, and filesystem block allocation.
- A minimal profile builds only the selected everyday commands. Use it for small root filesystems and add commands as needed.
- The implementation uses C++ standard-library and POSIX/Linux APIs. No OpenSSL or other third-party cryptographic library is required.
- Source files contain no embedded shell-outs to GNU Coreutils for command behavior.

BUILD
Requirements: clang++ (or another compatible C++20 compiler), clang (or a compatible C compiler for tiny command entry points), a POSIX/Linux environment, and standard development headers.

Full command set:
  JOBS=8 ./build.sh

Minimal command set:
  PROFILE=minimal JOBS=8 ./build.sh

CMake alternative (full):
  cmake -S . -B /tmp/gcu-build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
  cmake --build /tmp/gcu-build --parallel 8

CMake minimal profile:
  cmake -S . -B /tmp/gcu-build -DGCU_MINIMAL=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
  cmake --build /tmp/gcu-build --parallel 8

Use another command list:
  COMMANDS_FILE=/path/to/commands.txt JOBS=8 ./build.sh

Executables:
  build/bin/
Shared runtime library:
  build/lib/libgcu.so

Each executable supports --version and --help. Help prints command-specific usage and a short description; consult docs/command-reference.txt, docs/command-expansions.txt, docs/architecture.txt, and docs/option-status.txt for usage, naming, architecture, and known gaps.

TESTING
  ./tests/smoke.sh
  ./tests/regression.sh

The tests cover text output, encoding/decoding, checksums against known vectors, basic file operations, numeric tools, permissions, and command help/version. They are regression tests, not proof of complete GNU Coreutils compatibility.

SHELL INTEGRATION
Install shell integration files (default prefix /usr/local):
  ./install-shell-integration.sh
Then source the matching file in the current shell or add it to your shell startup configuration. See docs/pmad-shell-integration.txt.

STAGING INSTALL
Build first, then install to a disposable root filesystem or image directory:
  ./install-to-root.sh /path/to/staging-root

This creates /path/to/staging-root/bin and /path/to/staging-root/lib. The executables use an origin-relative runtime path to find ../lib/libgcu.so. Test in a VM or image before using these commands in a real system. Do not overwrite a running host's /bin during development.

IMPORTANT LIMITATIONS
- GCU 0.2.0 is a development implementation, not a drop-in, fully GNU-compatible replacement. Some options and edge cases remain incomplete; see docs/option-status.txt.
- SHA/MD5/BLAKE2 and CRC implementations are dependency-free. MD5 and SHA-1 are legacy integrity formats, not suitable for security-sensitive authentication. Checksums alone do not authenticate files.
- Checksum tools currently read each input into memory; avoid them on huge files until streaming digest support is added.
- SELinux context tools require system integration and are not implemented in this build.
- pmad is provided as shell integration for POSIX shells, fish, csh/tcsh, Nushell, and PowerShell. A standalone executable cannot change its parent shell's directory; see docs/pmad-shell-integration.txt.
- wipf attempts to overwrite regular-file contents but cannot guarantee secure erasure on SSDs, journaled/copy-on-write filesystems, snapshots, or storage with remapping.
- Some system utilities depend on Linux-specific facilities such as utmpx, /proc, and statvfs.

Project reference:
  https://www.gnu.org/software/coreutils/manual/coreutils.html
