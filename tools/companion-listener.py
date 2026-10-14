#!/usr/bin/env python3
"""
Receive parsed problems from the Competitive Companion browser extension and
lay them out for this library's Sublime Text setup.

    python3 tools/companion-listener.py                 # into the current directory
    python3 tools/companion-listener.py -d ~/cf/round-900
    python3 tools/companion-listener.py --port 10043

For each problem clicked in the browser it writes three things:

    A.cpp          seeded from the cbasic snippet, if it does not already exist
    A.cpp:tests    the sample tests, in CppFastOlympicCoding's own format
    in.txt         the first sample's input, so Ctrl/Cmd+B runs immediately

Standard library only -- no pip install.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
import xml.etree.ElementTree as ET
from http.server import BaseHTTPRequestHandler, HTTPServer

# Competitive Companion POSTs to http://localhost:<port>/ on every port in its
# default list at once, so anything in that list needs no extension config.
# 27121 is nominally "Competitive Programming Helper" (the VS Code extension);
# pick a different one with --port if you also run that.
#   1327 cpbooster · 4244 Hightail · 6174 Mind Sport · 10042 acmX
#   10043 Caide · 10045 CP Editor · 27121 Competitive Programming Helper
DEFAULT_PORT = 27121

# CppFastOlympicCoding reads tests from <source filename> + this suffix, in the
# same directory, unless its tests_relative_dir setting says otherwise.
# Source: FastOlympicCoding/settings.py, get_tests_file_path().
TESTS_SUFFIX = ":tests"

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SNIPPET = os.path.join(
    REPO, "Sublime", "Snippets", "Template-cbase-Snippet.sublime-snippet"
)


def load_template() -> str:
    """Pull the cbasic template out of the snippet and fill in its tab stops."""
    try:
        content = ET.parse(SNIPPET).getroot().findtext("content") or ""
    except (OSError, ET.ParseError) as exc:
        print(f"  ! could not read the cbasic snippet ({exc}); using a bare stub")
        return '#include "bits/stdc++.h"\nusing namespace std;\n\nint main() {\n    \n}\n'

    # ${1:false} is the multi-testcase flag. Competitive Companion does not tell
    # us whether the problem is multi-test, so keep the snippet's own default
    # rather than guessing.
    content = re.sub(r"\$\{1:([^}]*)\}", r"\1", content)
    content = re.sub(r"\$\{2:([^}]*)\}", r"\1", content)
    content = re.sub(r"\$\{\d+:([^}]*)\}", r"\1", content)
    content = content.replace("$0", "")
    return content.lstrip("\n")


def problem_filename(name: str, short: bool) -> str:
    """'A. Theatre Square' -> 'A.cpp' (short) or 'A-Theatre-Square.cpp'."""
    name = name.strip()
    if short:
        # Codeforces/AtCoder style index: a letter, optionally with digits,
        # followed by . or ) and a space.
        m = re.match(r"^([A-Za-z]\d*)[.)]\s", name)
        if m:
            return m.group(1).upper() + ".cpp"
    safe = re.sub(r"[^A-Za-z0-9]+", "-", name).strip("-")
    return (safe or "problem") + ".cpp"


def write_problem(task: dict, out_dir: str, short: bool, force: bool) -> None:
    name = task.get("name") or "problem"
    tests = task.get("tests") or []
    fname = problem_filename(name, short)
    src = os.path.join(out_dir, fname)

    os.makedirs(out_dir, exist_ok=True)

    batch = task.get("batch") or {}
    size = batch.get("size")
    progress = f" [{size} in this batch]" if isinstance(size, int) and size > 1 else ""
    print(f"\n+ {name}{progress}")
    print(f"  url   {task.get('url', '?')}")

    # Never silently overwrite work in progress.
    if os.path.exists(src) and not force:
        print(f"  skip  {fname} already exists (tests still refreshed)")
    else:
        with open(src, "w", encoding="utf-8") as fh:
            fh.write(load_template())
        print(f"  write {fname}")

    # FastOlympicCoding's schema: a list of {test, correct_answers}.
    foc = [
        {"test": t.get("input", ""), "correct_answers": [t.get("output", "")]}
        for t in tests
    ]
    tests_path = src + TESTS_SUFFIX
    with open(tests_path, "w", encoding="utf-8") as fh:
        json.dump(foc, fh, indent=4)
    print(f"  write {os.path.basename(tests_path)}  ({len(foc)} test(s))")

    # The cbasic template reads stdin from in.txt when bits/ is present, so
    # seeding it makes Ctrl/Cmd+B immediately meaningful. But there is only one
    # in.txt per directory: parsing a whole contest would leave it holding
    # whichever problem happened to arrive last. So only seed it for a
    # single-problem parse, and for a batch point at the test runner instead,
    # which keeps every problem's samples separately.
    batched = isinstance(size, int) and size > 1
    if tests and not batched:
        with open(os.path.join(out_dir, "in.txt"), "w", encoding="utf-8") as fh:
            fh.write(tests[0].get("input", ""))
        print("  write in.txt  (first sample)")
    elif tests:
        print("  note  in.txt not seeded for a batch; use the FOC test runner,")
        print("        or copy a sample in by hand to use Ctrl/Cmd+B")

    tl, ml = task.get("timeLimit"), task.get("memoryLimit")
    if tl or ml:
        print(f"  limits {tl} ms, {ml} MB")
    if task.get("interactive"):
        print("  note  interactive problem -- samples are usually not runnable")


def make_handler(args):
    class Handler(BaseHTTPRequestHandler):
        def do_POST(self):  # noqa: N802  (name fixed by BaseHTTPRequestHandler)
            length = int(self.headers.get("Content-Length") or 0)
            raw = self.rfile.read(length)
            self.send_response(200)
            self.end_headers()
            try:
                task = json.loads(raw.decode("utf-8"))
            except (UnicodeDecodeError, json.JSONDecodeError) as exc:
                print(f"\n! ignored a malformed payload: {exc}")
                return
            try:
                write_problem(task, args.dir, args.short, args.force)
            except OSError as exc:
                print(f"\n! could not write files: {exc}")

        def log_message(self, *_):
            pass  # the prints above are the useful log

    return Handler


def main() -> int:
    p = argparse.ArgumentParser(
        description="Competitive Companion -> CppFastOlympicCoding bridge."
    )
    p.add_argument("-d", "--dir", default=os.getcwd(),
                   help="where to write problems (default: current directory)")
    p.add_argument("-p", "--port", type=int, default=DEFAULT_PORT,
                   help=f"port to listen on (default: {DEFAULT_PORT})")
    p.add_argument("--full-names", dest="short", action="store_false",
                   help="name files A-Theatre-Square.cpp instead of A.cpp")
    p.add_argument("--force", action="store_true",
                   help="overwrite an existing .cpp instead of keeping it")
    args = p.parse_args()

    # Line-buffer so progress shows up live even when piped to a file or a
    # Sublime output panel, not just when attached to a terminal.
    try:
        sys.stdout.reconfigure(line_buffering=True)
    except AttributeError:  # Python < 3.7
        pass

    try:
        server = HTTPServer(("127.0.0.1", args.port), make_handler(args))
    except OSError as exc:
        print(f"Cannot listen on port {args.port}: {exc}", file=sys.stderr)
        print("Another parser is probably already using it -- try --port 10043.",
              file=sys.stderr)
        return 1

    print(f"Listening on http://127.0.0.1:{args.port}/")
    print(f"Writing to   {args.dir}")
    print("Click the green + in Competitive Companion. Ctrl+C to stop.")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nStopped.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
