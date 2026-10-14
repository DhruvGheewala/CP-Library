#!/usr/bin/env bash
#
# Build/run shim for "CPP17 (Unix).sublime-build" on macOS and Linux.
#
#   cpbuild.sh <compile|run|timeit|sanitize|doctor> [source-file]
#
# Compiler choice happens here, at build time, rather than being baked into the
# .sublime-build file, so a Homebrew GCC upgrade does not require reinstalling.

set -uo pipefail

MODE=${1:-}
SRC=${2:-}

if [ -z "$MODE" ]; then
    echo "usage: cpbuild.sh <compile|run|timeit|sanitize|doctor> [source-file]" >&2
    exit 64
fi

# ---------------------------------------------------------------- compiler pick

# bits/stdc++.h and bits/debug.h need libstdc++ specifically: debug.h refers to
# _Bit_reference, a libstdc++ internal, and <bits/stdc++.h> does not exist in
# libc++ at all. On macOS /usr/bin/g++ is Apple clang wearing a g++ name, so
# matching on the binary name is not enough -- check what it reports itself as.
pick_cxx() {
    local candidate
    for candidate in "${CXX:-}" g++-16 g++-15 g++-14 g++-13 g++-12 g++-11 g++; do
        [ -n "$candidate" ] || continue
        command -v "$candidate" >/dev/null 2>&1 || continue
        "$candidate" --version 2>/dev/null | head -1 | grep -qi 'clang' && continue
        printf '%s\n' "$candidate"
        return 0
    done
    return 1
}

no_compiler() {
    cat >&2 <<'EOF'
No GNU g++ found (Apple clang does not ship <bits/stdc++.h>).

  macOS          brew install gcc
  Debian/Ubuntu  sudo apt install g++
  Fedora         sudo dnf install gcc-c++
  Arch           sudo pacman -S gcc

Already installed under a name not probed above? Export CXX, e.g.
  echo 'export CXX=g++-15' >> ~/.zshrc
EOF
    exit 127
}

CXX=$(pick_cxx) || no_compiler

if [ "$MODE" = doctor ]; then
    echo "compiler : $CXX"
    echo "version  : $("$CXX" --version | head -1)"
    printf '#include <bits/stdc++.h>\nint main(){std::cout<<1;}\n' >"${TMPDIR:-/tmp}/cpdoctor.cpp"
    if "$CXX" -std=c++17 "${TMPDIR:-/tmp}/cpdoctor.cpp" \
              -o "${TMPDIR:-/tmp}/cpdoctor" 2>/dev/null; then
        echo "stdc++.h : ok"
    else
        echo "stdc++.h : MISSING -- this compiler cannot build the library"
        exit 1
    fi
    exit 0
fi

# ------------------------------------------------------------------- locate src

if [ -z "$SRC" ]; then
    echo "cpbuild.sh: mode '$MODE' needs a source file" >&2
    exit 64
fi
if [ ! -f "$SRC" ]; then
    echo "cpbuild.sh: no such file: $SRC" >&2
    exit 66
fi

DIR=$(cd "$(dirname "$SRC")" && pwd)
BASE=$(basename "$SRC")
BASE=${BASE%.*}
BIN="$DIR/$BASE"

# Deliberately NO "-I $DIR". Your sources say #include "bits/stdc++.h", which a
# quoted include already resolves relative to the source file. Adding $DIR to
# the search path also makes the *angle-bracket* <bits/stdc++.h> inside that
# header resolve back to itself; the include guard is already set by then, so it
# expands to nothing and the real libstdc++ header is never read -- every name
# in std:: then comes up undefined. Verified: with -I $DIR the system header is
# silently skipped; without it, it is found.
#
# Warnings are tuned so that a correct build is COMPLETELY silent -- anything
# that does print is then worth reading. Each suppression below is structural,
# i.e. it fires on this library's own design rather than on a mistake of yours:
#
#   unused-value      the debug(...) macro is a comma expression, so -Wall
#                     flags every single call site
#   unused-parameter  the cbasic template's main(argc, argv) uses neither, and
#                     debug.h's to_string(const char*) ignores its argument
#   comment           debug.h ends two banner comments with a backslash
#   unused-variable   debug.h's to_string(vector<bool>) declares an unused n
#   sign-compare      i < v.size() is endemic to contest code
#
# Verified still firing through the suppressions above: -Wshadow,
# -Wreturn-type, -Wparentheses, -Wformat. (-Wuninitialized is unreliable at
# -O2 regardless of these flags -- use the sanitizer variant for that class.)
WARN=(-Wall -Wextra -Wshadow
      -Wno-comment -Wno-unused-parameter -Wno-unused-variable
      -Wno-unused-value -Wno-sign-compare -Wno-unused-result)

compile() {
    printf '[%s] %s\n' "$CXX" "$SRC"
    "$CXX" -std=c++17 -O2 "${WARN[@]}" "$SRC" -o "$BIN" || return $?
    printf -- '-> %s\n' "$BIN"
}

case "$MODE" in
    compile)
        compile
        ;;

    run)
        compile || exit $?
        echo '---'
        # bits/stdc++.h defines DHRUV_GHEEWALA, so the cbase template redirects
        # stdin/stdout/stderr to in.txt / out.txt / debug.txt in this directory.
        # That is why nothing shows up on the panel: read out.txt instead.
        ( cd "$DIR" && "$BIN" )
        echo "--- exit $?"
        ;;

    timeit)
        compile || exit $?
        echo '---'
        ( cd "$DIR" && time "$BIN" )
        ;;

    sanitize)
        # Catches out-of-bounds, UB and bad iterators that -O2 happily ignores.
        # Slower, so it is a separate variant rather than the default.
        printf '[%s +asan/ubsan +_GLIBCXX_DEBUG] %s\n' "$CXX" "$SRC"
        "$CXX" -std=c++17 -O1 -g "${WARN[@]}" \
               -fsanitize=address,undefined \
               -fno-omit-frame-pointer \
               -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \
               "$SRC" -o "$BIN-san" || exit $?
        echo '---'
        ( cd "$DIR" && "$BIN-san" )
        echo "--- exit $?"
        ;;

    *)
        echo "cpbuild.sh: unknown mode '$MODE'" >&2
        exit 64
        ;;
esac
