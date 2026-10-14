# CP-Library

Competitive-programming library and a complete Sublime Text setup: C++ templates,
42 snippets, build systems, a debug header that prints containers, and an
installer that wires it all into Sublime in one command.

---

## Quick start

**macOS / Linux**

```bash
git clone https://github.com/DhruvGheewala/CP-Library.git
cd CP-Library
./install.sh
```

**Windows** (PowerShell)

```powershell
git clone https://github.com/DhruvGheewala/CP-Library.git
cd CP-Library
powershell -ExecutionPolicy Bypass -File .\install.ps1
```

Then:

1. **Restart Sublime Text.** Package Control sees the 10 packages listed in
   `Package Control.sublime-settings` and installs the missing ones itself.
   Watch the status bar.
2. `Tools -> Build System ->` **CPP17 (Unix)** on macOS/Linux, **CPP17** on Windows.
3. Copy `bits/` next to the file you are solving (see [Why `bits/` travels with
   your code](#why-bits-travels-with-your-code)).
4. Open a `.cpp`, type `cbasic`, press <kbd>Tab</kbd>.

For one-click sample fetching from the browser, see
[Automatic test-case fetching](#automatic-test-case-fetching).

Installer flags: `--dry-run` to preview, `--copy` to copy instead of symlink,
`--uninstall` to undo. Windows uses `-DryRun`, `-Copy`, `-Uninstall`.

---

## Prerequisites

| Need | macOS | Linux | Windows |
|---|---|---|---|
| **GNU g++** | `brew install gcc` | `sudo apt install g++` | [MSYS2](https://www.msys2.org/) MinGW-w64, add `bin\` to `PATH` |
| **Sublime Text** | 3 or 4 | 3 or 4 | 3 or 4 |
| **Package Control** | Command Palette -> `Install Package Control` | same | same |

**macOS needs real GCC, not Apple's.** `/usr/bin/g++` on macOS is Apple clang
wearing a `g++` name. It has no `<bits/stdc++.h>` at all, and `bits/debug.h`
uses `_Bit_reference`, a libstdc++ internal. The build shim detects this and
picks `g++-15`, `g++-14`, … automatically after `brew install gcc` — you do not
need to edit any config. Check what it found:

```
Tools -> Build With... -> CPP17 (Unix) - Doctor (check compiler)
```

Compiler under a name the shim does not probe? `export CXX=g++-15` in your shell
profile and it will be used first.

---

## The one idea worth understanding

The same source file behaves differently at home and on the judge, with no edits.

`#include "bits/stdc++.h"` is a **quoted** include, so it resolves relative to
your source file first:

| | Resolves to | Effect |
|---|---|---|
| **Locally**, with `bits/` beside your `.cpp` | this repo's `bits/stdc++.h` | defines `DEBUG` + `DHRUV_GHEEWALA` -> `debug(...)` prints, I/O redirects to files |
| **On Codeforces**, no `bits/` directory | the system `<bits/stdc++.h>` | neither macro defined -> `debug(...)` compiles to `1729`, `fundri` to `108`, normal stdin/stdout |

So you paste the file into the judge unchanged. Nothing to strip out.

### Why `bits/` travels with your code

That resolution is the whole mechanism, so `bits/` must sit in the **same
directory as the file you are compiling**:

```bash
cp -r ~/CP-Library/bits ~/contests/round-900/
```

Do **not** add the source directory to the include path to avoid this. Putting
`-I <srcdir>` on the command line makes the *angle-bracket* `<bits/stdc++.h>`
inside this repo's own header resolve back to that same header; its include
guard is already set by then, so it expands to nothing, the real libstdc++
header is never read, and every name in `std::` comes up undefined. The Unix
build passes no `-I` for exactly this reason.

### Local runs read and write files, not the console

`bits/stdc++.h` defines `DHRUV_GHEEWALA`, which switches the `cbasic` template
to file I/O in the directory of your source file:

| File | Role |
|---|---|
| `in.txt` | **stdin** — put the test case here |
| `out.txt` | **stdout** — your answer lands here |
| `debug.txt` | **stderr** — everything `debug(...)` and `fundri` print |

This is why the build panel looks empty after a run. Create those three files
once per contest directory and keep them open in split panes.

Want console I/O instead? Comment out `#define DHRUV_GHEEWALA` in
`bits/stdc++.h`; `debug.txt` still captures stderr.

---

## Debugging

`bits/debug.h` gives `debug(...)`, which prints any nesting of containers and
pairs with the expression text alongside the value:

```cpp
vector<pair<int,int>> v{{1,2},{3,4}};
map<string,int> m{{"a",1}};
debug(v, m);   // -> debug.txt:  v = {{1, 2}, {3, 4}}, m = {{"a", 1}}
fundri;        // -> a dashed separator line, useful between test cases
```

Any range works, plus `pair`, `vector<bool>`, `bitset`, `queue`, `stack` and
`priority_queue`, nested freely; `std::string` comes out quoted. There is no
`tuple` overload. Printing a `queue`, `stack` or `priority_queue` drains a copy,
so your original is untouched.

A bare string literal has no value half, so `debug("after sort")` prints just
the label — handy as a checkpoint marker.

Three opt-in switches at the top of `bits/stdc++.h`:

| Macro | Effect |
|---|---|
| `__INCLUDE_LINE_NUMBER__` | prefix every `debug(...)` with `Line: N` |
| `__USE_TOURIST_DEBUG__` | switch to tourist's output formatting |
| `__HOLD_SCREEN_ATLAST__` | wait for a keypress before the window closes |

`exit(0)` is overridden to also print the run time, so end `main` with it.

---

## Build variants

<kbd>Ctrl</kbd>/<kbd>Cmd</kbd>+<kbd>B</kbd> runs the default; <kbd>Ctrl</kbd>/<kbd>Cmd</kbd>+<kbd>Shift</kbd>+<kbd>B</kbd> picks a variant.

**CPP17 (Unix)** — macOS, Linux

| Variant | What it does |
|---|---|
| *(default)* | compile only, `-O2 -Wall -Wextra -Wshadow` |
| Run | compile, then run (reads `in.txt`, writes `out.txt`) |
| Timeit | same, plus wall-clock timing |
| Run with sanitizers | `-fsanitize=address,undefined -D_GLIBCXX_DEBUG` — catches out-of-bounds and UB that `-O2` silently tolerates |
| Doctor (check compiler) | print which `g++` was selected and whether it can find `<bits/stdc++.h>` |

**CPP17 / CPP14** — Windows only. These shell out to `start`, `taskkill` and
`powershell`, and the `Timeit` variant needs `timeit.bat` on your `PATH`.

Reach for **Run with sanitizers** the moment a solution is wrong for no visible
reason. It costs a few seconds and finds the class of bug that takes an hour by
hand.

---

## Automatic test-case fetching

[Competitive Companion](https://chromewebstore.google.com/detail/competitive-companion/cjnmckjndlpiamhfimnnjmnckgghkjbl)
parses the problem you are looking at — statement, samples, limits — and POSTs
it to a program listening on your machine. One click, no copy-paste.

**CppFastOlympicCoding has no Competitive Companion listener of its own**, so
`tools/companion-listener.py` bridges the two. It is standard library only; no
`pip install`.

### Setup

1. Install the extension:
   [Chrome / Edge](https://chromewebstore.google.com/detail/competitive-companion/cjnmckjndlpiamhfimnnjmnckgghkjbl)
   · [source and other browsers](https://github.com/jmerle/competitive-companion)
2. Make a directory for the contest and put `bits/` in it:

   ```bash
   mkdir -p ~/cf/round-900 && cp -r ~/CP-Library/bits ~/cf/round-900/
   ```
3. Start the listener there and leave it running:

   ```bash
   python3 ~/CP-Library/tools/companion-listener.py -d ~/cf/round-900
   ```
4. Open the problem in your browser and click the green **+**.

No extension configuration is needed. Competitive Companion broadcasts to all
of its default ports at once, and the listener defaults to **27121**, which is
one of them.

### What you get per problem

```
A.cpp          seeded from the cbasic template (never overwritten if it exists)
A.cpp:tests    the samples, in CppFastOlympicCoding's own format
in.txt         the first sample, so Ctrl/Cmd+B runs immediately
```

Then either run the samples through FastOlympicCoding, or compile and read
`out.txt` the usual way.

Clicking **+** on a contest page parses every problem in one go. In that case
`in.txt` is deliberately **not** written — there is only one `in.txt` per
directory, and a batch would leave it holding whichever problem arrived last.
Use the test runner for batches, which keeps each problem's samples separately.

### Running the samples

| | Key |
|---|---|
| macOS | <kbd>Ctrl</kbd>+<kbd>B</kbd> — FastOlympicCoding's own binding, free because the build system is on <kbd>Cmd</kbd>+<kbd>B</kbd> |
| Windows / Linux | Command Palette -> `FastOlympicCoding: Run` |

On Windows and Linux the library's keymap binds <kbd>Ctrl</kbd>+<kbd>B</kbd> to
the build system, and a `Packages/User` keymap outranks every package, so
FastOlympicCoding's <kbd>Ctrl</kbd>+<kbd>B</kbd> never fires. The Command
Palette always works; rebind in
`Packages/User/Default (Windows).sublime-keymap` if you would rather have the
key.

Other commands the package exposes, all via the Command Palette:
`FastOlympicCoding:` `Run` · `Make Stress` · `Stop Stress` · `Submit` ·
`Setup Contest` · `Open Settings`.

**Make Stress** is the one worth learning: it runs your solution against a brute
force on generated inputs until they disagree. `TestCase-Generator/` has the
generators to feed it.

### Listener options

| Flag | Effect |
|---|---|
| `-d`, `--dir` | where to write (default: current directory) |
| `-p`, `--port` | listen port (default: 27121; try `10043` if something else has it) |
| `--full-names` | `A-Theatre-Square.cpp` instead of `A.cpp` |
| `--force` | overwrite an existing `.cpp` instead of keeping it |

Names come from the problem title: `A. Theatre Square` becomes `A.cpp`. A title
that does not start with a problem index keeps its full name, so LeetCode's
`1551. Minimum Operations` becomes `1551-Minimum-Operations.cpp`.

### Note on the `:tests` filename

The suffix is FastOlympicCoding's own default, so it is kept as-is for
compatibility. On Windows a `:` in a filename means an NTFS alternate data
stream, so `A.cpp:tests` attaches to `A.cpp` rather than appearing as a separate
file. The package reads it back the same way, so this works — it just will not
show up in a directory listing. Change `tests_file_suffix` in the
FastOlympicCoding settings if you want a normal file.

---

## Snippets

Type the trigger, press <kbd>Tab</kbd>. C++ snippets are scoped to `source.c++`,
Python ones to `source.python`.

**Template**

| Trigger | Gives you |
|---|---|
| `cbasic` | full solution template: fast I/O, `solve(tc)`, multi-test toggle, N-dimensional `vec`, `rnd()`, `in_range` |

**Data structures**

| Trigger | | Trigger | |
|---|---|---|---|
| `csegtree` | segment tree | `csegmentree` | generic segment tree |
| `csegment` | iterative segtree, point update | `cfenwick` | Fenwick / BIT |
| `cdsu` | disjoint set union | `csqrtdecomp` | sqrt decomposition |
| `cpbds` | ordered set (PBDS) | `cmodularint` | modular int class |
| `cmatrix` | matrix template | `chash` | custom hash (anti-collision) |

**Graphs**

| Trigger | | Trigger | |
|---|---|---|---|
| `cadjlist` | adjacency list | `cdijkstra` | Dijkstra |
| `cbellman` | Bellman-Ford | `cspfa` | SPFA |
| `ctoposort` | topological sort | `ccycle` | cycle detection |
| `ckruskal` | Kruskal MST | `cprims` | Prim MST |
| `clca` | LCA, binary lifting | `cflow` | Ford-Fulkerson max flow |

**Math & misc**

| Trigger | | Trigger | |
|---|---|---|---|
| `csieve` | sieve — **3 snippets share this trigger** | `cprimefact` | prime factorization |
| `cmodulo` | modular arithmetic — **2 snippets share this** | `cgcd` | extended GCD |
| `cbit` | bit manipulation | `cpragma` | optimization pragmas |
| `cdirection` | 4/8-direction arrays | `c2p` | two-pointer skeleton |
| `cmosstd` | Mo's algorithm | `cmoshilbert` | Mo's, Hilbert curve order |
| `cto_string` | `to_string` helpers | `stampcomplexity` | complexity comment stamp |

**Python**: `pprint`, `pfile`, `pfwrite`, `pycmd`, `exit`.
**Markdown**: `newDay`.

Where a trigger is shared, Sublime shows a picker instead of inserting
immediately — see [Known rough edges](#known-rough-edges).

---

## Keybindings

Installed per platform. macOS keeps build on <kbd>Cmd</kbd>+<kbd>B</kbd>; the
library's own bindings use <kbd>Ctrl</kbd>+<kbd>Alt</kbd> so they do not collide
with macOS shortcuts.

| Keys (Win/Linux) | Keys (macOS) | Action |
|---|---|---|
| <kbd>Ctrl</kbd>+<kbd>B</kbd> | <kbd>Cmd</kbd>+<kbd>B</kbd> | build |
| <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>C</kbd> | same | cancel build |
| <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>O</kbd> | <kbd>Cmd</kbd>+<kbd>Shift</kbd>+<kbd>O</kbd> | open the `cbasic` template for editing |
| <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>P</kbd> | same | toggle sidebar |
| <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>N</kbd> | same | new snippet |
| <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>O</kbd> | same | open the Packages directory |
| <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>I</kbd> | same | CoolFormat quick-format |
| <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>Shift</kbd>+<kbd>C</kbd> | same | copy selection as highlighted HTML |

AceJump keeps its own default binding (<kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>;</kbd>
and friends) — it is not rebound here.

Windows additionally binds <kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>B</kbd> and
<kbd>Ctrl</kbd>+<kbd>Alt</kbd>+<kbd>S</kbd>, which need packages not on Package
Control — see [Known rough edges](#known-rough-edges).

---

## What the installer touches

Sublime does not recurse when looking for snippets or settings, so everything is
flattened into one directory:

```
<Sublime data dir>/Packages/User/
    CPP17 (Unix).sublime-build      cpbuild.sh
    Default (OSX).sublime-keymap    Preferences.sublime-settings
    Package Control.sublime-settings
    FastOlympicCoding (OSX).sublime-settings
    SublimeHighlight.sublime-settings
    CoolFormatConfig.cfconfig
    CPP-Common-Snippets.sublime-completions
    *.sublime-snippet               (42 files)
```

Data directory per platform:

| | Sublime Text 4 | Sublime Text 3 |
|---|---|---|
| macOS | `~/Library/Application Support/Sublime Text` | `…/Sublime Text 3` |
| Linux | `~/.config/sublime-text` | `~/.config/sublime-text-3` |
| Windows | `%APPDATA%\Sublime Text` | `%APPDATA%\Sublime Text 3` |

Portable install, or a path the installer misses:

```bash
SUBLIME_DATA_DIR=/path/to/Data ./install.sh
```

**Files are symlinked, not copied.** Editing a snippet inside Sublime shows up
as a normal `git diff` in this repo. Pass `--copy` if you would rather Sublime
never write into your checkout. Any pre-existing file with different content is
moved to `<name>.bak-<timestamp>` first; `--uninstall` removes the links and
leaves the backups.

Platform-irrelevant files are skipped: the Unix build and `cpbuild.sh` are not
installed on Windows, and the Windows builds, keymap and FastOlympicCoding
settings are not installed on macOS or Linux.

---

## Packages installed

Package Control installs these automatically on first restart:

| Package | Why |
|---|---|
| **CppFastOlympicCoding** | run test cases, stress-test against a brute force, inline lint |
| **CoolFormat** | format C++ |
| **AceJump** | jump to any character |
| **Dracula Color Scheme** | theme used by `Preferences.sublime-settings` |
| **Highlight** | export highlighted code as HTML |
| **Trimmer** | strip trailing whitespace |
| MarkdownPreview, MarkdownTOC, FileIcons, Package Control | editing notes, icons |

---

## Repository layout

| Path | Contents |
|---|---|
| `bits/` | `stdc++.h` wrapper + `debug.h` container-printing debugger — **copy next to your solution** |
| `Sublime/` | build systems, keymaps, settings, `Snippets/`, `cpbuild.sh` |
| `tools/` | `companion-listener.py` — Competitive Companion -> FastOlympicCoding bridge |
| `Templates/` | 15 reference implementations: Fenwick, lazy segtree, DSU, LCA, Mo's, max flow, Gaussian elimination, Rabin-Karp, bridges/articulation points, Diophantine |
| `Famous-Problems/` | 12 classic problems worked out |
| `Leetcode/` | 13 solved LeetCode problems, named by number |
| `TestCase-Generator/` | random tree, directed and undirected graph generators |
| `timeit.bat` | Windows execution timer, by [erekle](https://codeforces.com/profile/erekle) |
| `ProblemsList-DG.md`, `Random.md` | practice log and notes |

`Templates/` and `Famous-Problems/` are reading material, not headers — they are
standalone files with their own `main`, meant to be copied from.

---

## Troubleshooting

| Symptom | Cause and fix |
|---|---|
| `'bits/stdc++.h' file not found` | Apple clang, or `bits/` is not beside your `.cpp`. Run the **Doctor** variant; `brew install gcc`; copy `bits/` across. |
| Build panel is empty after Run | Working as designed — output went to `out.txt`. See [Local runs read and write files](#local-runs-read-and-write-files-not-the-console). |
| Everything in `std::` is undefined | Something put the source directory on the include path and shadowed the real header. Remove `-I <srcdir>`. |
| Packages never installed | Package Control itself is missing. Command Palette -> `Install Package Control`, restart. |
| Snippets do not fire | Wrong syntax mode. C++ snippets need the file set to C++, not plain text. |
| `running scripts is disabled on this system` | PowerShell policy: `powershell -ExecutionPolicy Bypass -File .\install.ps1` |
| Installer cannot find the data directory | Launch Sublime once so it creates the folder, or set `SUBLIME_DATA_DIR`. |
| `sh: Color: command not found` | Old checkout. The `exit()` macro now guards that call behind `#ifdef _WIN32`. |
| Clicking **+** does nothing | The listener is not running, or something else holds the port. Start it, or use `--port 10043`. |
| `Cannot listen on port 27121` | Another parser (VS Code's cph) has it. Pick another default port: `--port 10043`. |
| Samples fetched but <kbd>Ctrl</kbd>+<kbd>B</kbd> ignores them | On Windows/Linux that key is the build system. Command Palette -> `FastOlympicCoding: Run`. |
| `A.cpp:tests` is nowhere to be seen on Windows | It is an NTFS alternate data stream on `A.cpp`, which is how the package stores it. |

---

## Known rough edges

1. **Three snippets answer to `csieve`** (classical/vector, classical/bitset,
   linear) and **two to `cmodulo`** (functions, class). Sublime shows a picker
   rather than inserting directly.
2. **`Ctrl+Alt+B` and `Ctrl+Alt+S` are dead on Windows.** They invoke
   `cfcompiler` and `quote`, from user-defined packages that are not on Package
   Control. The keymap keeps them, commented, for reference.
3. **`stampcomplexity` has no scope**, so it fires in every file type.
4. **FastOlympicCoding on macOS/Linux** hardcodes `g++` in its own
   `compile_cmd`, which `cpbuild.sh` cannot intercept. After `brew install gcc`,
   change `g++` to your GNU binary in
   `FastOlympicCoding (OSX).sublime-settings`.
5. **`CPP14.sublime-build` is Windows-only** and uses `-fconcepts`, a GCC
   extension. Prefer the C++17 builds.

---

## Links

### Progress trackers
* [Codeforces Visualizer](https://cfviz.netlify.app/index.html) — your progress
* [Codeforces Randomizer](https://karimelghamry.github.io/Codeforces-Randomizer/) — a random problem by tag and rating
* [Codeforces Practice Tracker](https://chrome.google.com/webstore/detail/codeforces-practice-track/cnahgdhboflcnojgmeehhhfeoojifonm) — improvement over time
* [CF Predictor](https://chrome.google.com/webstore/detail/cf-predictor/ocfloejijfhhkkdmheodbaanephbnfhn) — live rating change during a contest
* [KONTESTS](https://chrome.google.com/webstore/detail/kontests/agpdemlkalmmeenclchlajdcmbcacoea) — upcoming contests

### Tutorials
* [Guide For Beginner](http://lbv-pc.blogspot.com/p/advice-for-beginners.html)
* [CP-Algorithms](http://cp-algorithms.com/)
* [Ashish Gupta's Quora Profile](https://www.quora.com/profile/Ashish-Gupta-211)
* [Anudeep's Blogs](https://blog.anudeep2011.com/)
* [ZLE's Blogs](https://kauntaofficial.github.io/blog/)
* [Asharam's Blogs](https://procoderforu.com/)
* [Sabouri's Blogs](http://mdotsabouri.blogspot.com/)
* [Stanford CS97SI — Introduction to Programming Contests](http://stanford.edu/class/cs97si/)
* [GeeksforGeeks](https://www.geeksforgeeks.org/)
* [Brilliant — Computer Science](https://brilliant.org/computer-science/)

### Mathematics
* [Number Theory — Wolfram MathWorld](https://mathworld.wolfram.com/topics/NumberTheory.html)
* [Art of Problem Solving](https://artofproblemsolving.com/wiki/)
* [Number Theory — Brilliant](https://brilliant.org/number-theory/)

### Other libraries worth reading
* [Ashishgup1](https://github.com/Ashishgup1/Competitive-Coding)
* [pin3da](https://github.com/pin3da/Programming-contest)
* [Huzaifa242](https://github.com/huzaifa242/Coding-Library-huzaifa242)
* [Interview-Problem-Statements](https://github.com/LifeofAGeek/Interview-Problem-Statements)
* [Coding-Notes](https://github.com/ankitpriyarup/Coding_Notes)

---

Files in this repo are interconnected — `bits/stdc++.h` pulls in `bits/debug.h`,
and the `cbasic` template assumes both. Copy `bits/` as a unit.
