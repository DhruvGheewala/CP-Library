#!/usr/bin/env bash
#
# One-shot Sublime Text setup for CP-Library (macOS / Linux).
#
#   ./install.sh              symlink configs into Packages/User
#   ./install.sh --copy       copy instead of symlink
#   ./install.sh --dry-run    print what would happen, touch nothing
#   ./install.sh --uninstall  remove what this script installed
#
# Symlinks are the default so that editing a snippet in Sublime shows up as a
# normal git change in this repo.

set -euo pipefail

REPO=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
MODE=link
STAMP=$(date +%Y%m%d-%H%M%S)

for arg in "$@"; do
    case "$arg" in
        --copy)      MODE=copy ;;
        --dry-run)   MODE=dry ;;
        --uninstall) MODE=uninstall ;;
        -h|--help)   sed -n '2,12p' "${BASH_SOURCE[0]}" | sed 's|^# \?||'; exit 0 ;;
        *)           echo "unknown option: $arg" >&2; exit 64 ;;
    esac
done

say()  { printf '%s\n' "$*"; }
step() { printf '\n== %s\n' "$*"; }

# ------------------------------------------------- 1. find Packages/User

# Newest Sublime first. ST4 dropped the version number from the directory name.
case "$(uname -s)" in
    Darwin) CANDIDATES=(
                "$HOME/Library/Application Support/Sublime Text"
                "$HOME/Library/Application Support/Sublime Text 3"
            ) ;;
    Linux)  CANDIDATES=(
                "${XDG_CONFIG_HOME:-$HOME/.config}/sublime-text"
                "${XDG_CONFIG_HOME:-$HOME/.config}/sublime-text-3"
            ) ;;
    *)      say "Unsupported platform: $(uname -s). On Windows run install.ps1."
            exit 1 ;;
esac

DATA_DIR=""
if [ -n "${SUBLIME_DATA_DIR:-}" ]; then
    # Explicit override wins, and is the escape hatch for portable installs.
    DATA_DIR=$SUBLIME_DATA_DIR
    if [ ! -d "$DATA_DIR/Packages" ]; then
        say "SUBLIME_DATA_DIR=$DATA_DIR has no Packages/ subdirectory."
        exit 1
    fi
else
    for c in "${CANDIDATES[@]}"; do
        if [ -d "$c/Packages" ]; then DATA_DIR=$c; break; fi
    done
fi

if [ -z "$DATA_DIR" ]; then
    say "Could not find a Sublime Text data directory. Looked in:"
    printf '  %s\n' "${CANDIDATES[@]}"
    say ""
    say "Launch Sublime Text once (it creates the directory on first run), then"
    say "re-run this script. Portable install? Pass the path explicitly:"
    say "  SUBLIME_DATA_DIR=/path/to/Data ./install.sh"
    exit 1
fi

USER_DIR="$DATA_DIR/Packages/User"
step "Sublime data directory"
say "  $DATA_DIR"

# ------------------------------------------------- 2. collect what to install

# Everything Sublime expects to sit flat in Packages/User, including snippets:
# Sublime does not recurse for .sublime-snippet, so Sublime/Snippets/* is
# flattened into Packages/User rather than copied as a subdirectory.
FILES=()
while IFS= read -r f; do FILES+=("$f"); done < <(
    find "$REPO/Sublime" -maxdepth 1 -type f \
         \( -name '*.sublime-build'       \
         -o -name '*.sublime-settings'    \
         -o -name '*.sublime-keymap'      \
         -o -name '*.sublime-completions' \
         -o -name '*.cfconfig'            \
         -o -name 'cpbuild.sh' \) | sort
    find "$REPO/Sublime/Snippets" -maxdepth 1 -type f -name '*.sublime-snippet' | sort
)

# The Windows-only build systems and keymap would only clutter the command
# palette on a Unix box, so leave them out.
SKIP=(
    "CPP14.sublime-build"
    "CPP17.sublime-build"
    "Default (Windows).sublime-keymap"
    "FastOlympicCoding (Windows).sublime-settings"
)
case "$(uname -s)" in
    Darwin) SKIP+=("Default (Linux).sublime-keymap"
                   "FastOlympicCoding (Linux).sublime-settings") ;;
    Linux)  SKIP+=("Default (OSX).sublime-keymap"
                   "FastOlympicCoding (OSX).sublime-settings") ;;
esac

skipped() {
    local base=$1 s
    for s in "${SKIP[@]}"; do [ "$base" = "$s" ] && return 0; done
    return 1
}

# ------------------------------------------------- 3. uninstall path

if [ "$MODE" = uninstall ]; then
    step "Removing installed files from Packages/User"
    removed=0
    for src in "${FILES[@]}"; do
        dst="$USER_DIR/$(basename "$src")"
        if [ -L "$dst" ] && [ "$(readlink "$dst")" = "$src" ]; then
            rm "$dst"; say "  unlinked $(basename "$dst")"; removed=$((removed+1))
        elif [ -f "$dst" ] && cmp -s "$src" "$dst"; then
            rm "$dst"; say "  removed  $(basename "$dst")"; removed=$((removed+1))
        fi
    done
    say ""
    say "Removed $removed file(s). Backups named *.bak-* were left alone."
    exit 0
fi

# ------------------------------------------------- 4. install

step "Installing into Packages/User"
[ "$MODE" = dry ] || mkdir -p "$USER_DIR"

installed=0 backed_up=0
for src in "${FILES[@]}"; do
    base=$(basename "$src")
    skipped "$base" && continue
    dst="$USER_DIR/$base"

    # Never clobber a real file the user wrote; move it aside first.
    if [ -e "$dst" ] && [ ! -L "$dst" ]; then
        if cmp -s "$src" "$dst"; then
            :
        else
            if [ "$MODE" = dry ]; then
                say "  would back up $base -> $base.bak-$STAMP"
            else
                mv "$dst" "$dst.bak-$STAMP"
                say "  backed up $base -> $base.bak-$STAMP"
            fi
            backed_up=$((backed_up+1))
        fi
    fi

    case "$MODE" in
        dry)  say "  would link $base" ;;
        link) rm -f "$dst"; ln -s "$src" "$dst" ;;
        copy) rm -f "$dst"; cp "$src" "$dst" ;;
    esac
    installed=$((installed+1))
done

[ "$MODE" = dry ] || chmod +x "$REPO/Sublime/cpbuild.sh"
say "  $installed file(s), $backed_up backup(s)"

if [ "$MODE" = dry ]; then
    step "Dry run -- nothing was changed"
    exit 0
fi

# ------------------------------------------------- 5. compiler check

step "Compiler"
if bash "$REPO/Sublime/cpbuild.sh" doctor; then
    COMPILER_OK=yes
else
    COMPILER_OK=no
fi

# ------------------------------------------------- 6. what is left to do

step "Next steps"
cat <<EOF
1. Restart Sublime Text. Package Control reads the installed_packages list in
   Package Control.sublime-settings and downloads the 10 missing packages by
   itself -- watch the status bar.

   No Package Control yet? Command Palette (ctrl/cmd+shift+p) ->
   "Install Package Control", then restart again.

2. Tools -> Build System -> "CPP17 (Unix)".

3. Copy bits/ next to whatever you are solving -- the template does
   #include "bits/stdc++.h", which resolves relative to your source file:
     cp -r "$REPO/bits" ~/my-contest/

4. In a .cpp file type  cbasic  then Tab to drop in the template.
EOF

if [ "$COMPILER_OK" = no ]; then
    cat <<'EOF'

   Compiler is not usable yet (see above) -- fix that before step 4, or the
   build will fail on <bits/stdc++.h>.
EOF
fi

say ""
say "Done. Configs are symlinked, so edits in Sublime show up as git changes here."
