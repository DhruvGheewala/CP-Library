<#
One-shot Sublime Text setup for CP-Library (Windows).

    .\install.ps1              link configs into Packages\User (copy if links are denied)
    .\install.ps1 -Copy        always copy
    .\install.ps1 -DryRun      print what would happen, touch nothing
    .\install.ps1 -Uninstall   remove what this script installed

PowerShell blocks unsigned local scripts by default. If you get
"running scripts is disabled on this system", either run it once as:

    powershell -ExecutionPolicy Bypass -File .\install.ps1

or lift the restriction for your own account permanently:

    Set-ExecutionPolicy -Scope CurrentUser RemoteSigned
#>

[CmdletBinding()]
param(
    [switch]$Copy,
    [switch]$DryRun,
    [switch]$Uninstall
)

$ErrorActionPreference = 'Stop'
$Repo  = $PSScriptRoot
$Stamp = Get-Date -Format 'yyyyMMdd-HHmmss'

# Plain string equality rather than Compare-Object: Get-Content -Raw yields
# $null for an empty file, and Compare-Object throws on a null argument.
function Same-Content($a, $b) {
    (Get-Content -LiteralPath $a -Raw) -eq (Get-Content -LiteralPath $b -Raw)
}

function Step($m) { Write-Host "`n== $m" -ForegroundColor Cyan }

# ------------------------------------------------- 1. find Packages\User

# Newest Sublime first: ST4 dropped the version number from the folder name.
$candidates = @(
    (Join-Path $env:APPDATA 'Sublime Text'),
    (Join-Path $env:APPDATA 'Sublime Text 3')
)
if ($env:SUBLIME_DATA_DIR) { $candidates = @($env:SUBLIME_DATA_DIR) }

$dataDir = $candidates | Where-Object { Test-Path (Join-Path $_ 'Packages') } | Select-Object -First 1

if (-not $dataDir) {
    Write-Host "Could not find a Sublime Text data directory. Looked in:"
    $candidates | ForEach-Object { Write-Host "  $_" }
    Write-Host ""
    Write-Host "Launch Sublime Text once (it creates the folder on first run), then"
    Write-Host "re-run this script. Portable install? Point at it explicitly:"
    Write-Host '  $env:SUBLIME_DATA_DIR = "D:\Sublime\Data"; .\install.ps1'
    exit 1
}

$userDir = Join-Path $dataDir 'Packages\User'
Step 'Sublime data directory'
Write-Host "  $dataDir"

# ------------------------------------------------- 2. collect what to install

# Sublime does not recurse for .sublime-snippet, so Sublime\Snippets\* gets
# flattened into Packages\User rather than kept as a subfolder.
$files  = @(Get-ChildItem -LiteralPath (Join-Path $Repo 'Sublime') -File |
            Where-Object { $_.Extension -in '.sublime-build','.sublime-settings',
                                            '.sublime-keymap','.sublime-completions',
                                            '.cfconfig' })
$files += @(Get-ChildItem -LiteralPath (Join-Path $Repo 'Sublime\Snippets') -File -Filter '*.sublime-snippet')

# The Unix build system, its shim and the POSIX keymaps would only clutter the
# command palette on Windows.
$skip = @(
    'CPP17 (Unix).sublime-build',
    'cpbuild.sh',
    'Default (OSX).sublime-keymap',
    'Default (Linux).sublime-keymap',
    'FastOlympicCoding (OSX).sublime-settings',
    'FastOlympicCoding (Linux).sublime-settings'
)

# ------------------------------------------------- 3. uninstall path

if ($Uninstall) {
    Step 'Removing installed files from Packages\User'
    $removed = 0
    foreach ($f in $files) {
        $dst = Join-Path $userDir $f.Name
        if (-not (Test-Path -LiteralPath $dst)) { continue }
        $item = Get-Item -LiteralPath $dst -Force
        $isLink = $null -ne $item.LinkType
        if ($isLink -or (Same-Content $f.FullName $dst)) {
            Remove-Item -LiteralPath $dst -Force
            Write-Host "  removed $($f.Name)"
            $removed++
        }
    }
    Write-Host ""
    Write-Host "Removed $removed file(s). Backups named *.bak-* were left alone."
    exit 0
}

# ------------------------------------------------- 4. install

Step 'Installing into Packages\User'
if (-not $DryRun) { New-Item -ItemType Directory -Force -Path $userDir | Out-Null }

# Creating a symlink on Windows needs either elevation or Developer Mode, so
# fall back to copying rather than failing the whole install.
$useLinks = -not $Copy
$installed = 0; $backedUp = 0; $fellBack = $false

foreach ($f in $files) {
    if ($skip -contains $f.Name) { continue }
    $dst = Join-Path $userDir $f.Name

    if (Test-Path -LiteralPath $dst) {
        $item = Get-Item -LiteralPath $dst -Force
        if ($null -eq $item.LinkType) {
            if (-not (Same-Content $f.FullName $dst)) {
                if ($DryRun) {
                    Write-Host "  would back up $($f.Name) -> $($f.Name).bak-$Stamp"
                } else {
                    Move-Item -LiteralPath $dst -Destination "$dst.bak-$Stamp"
                    Write-Host "  backed up $($f.Name) -> $($f.Name).bak-$Stamp"
                }
                $backedUp++
            }
        }
    }

    if ($DryRun) { Write-Host "  would install $($f.Name)"; $installed++; continue }

    Remove-Item -LiteralPath $dst -Force -ErrorAction SilentlyContinue
    $linked = $false
    if ($useLinks) {
        try {
            New-Item -ItemType SymbolicLink -Path $dst -Target $f.FullName -ErrorAction Stop | Out-Null
            $linked = $true
        } catch {
            $useLinks = $false; $fellBack = $true
        }
    }
    if (-not $linked) { Copy-Item -LiteralPath $f.FullName -Destination $dst -Force }
    $installed++
}

Write-Host "  $installed file(s), $backedUp backup(s)"
if ($fellBack) {
    Write-Host "  (symlinks denied -- copied instead. Turn on Settings -> Privacy &" -ForegroundColor Yellow
    Write-Host "   security -> For developers -> Developer Mode to get live links.)" -ForegroundColor Yellow
}

if ($DryRun) { Step 'Dry run -- nothing was changed'; exit 0 }

# ------------------------------------------------- 5. compiler check

Step 'Compiler'
$gxx = Get-Command g++ -ErrorAction SilentlyContinue
if ($gxx) {
    Write-Host "  g++ : $($gxx.Source)"
    Write-Host "  $((& g++ --version | Select-Object -First 1))"
} else {
    Write-Host "  g++ not on PATH." -ForegroundColor Yellow
    Write-Host "  Install MinGW-w64 (https://www.msys2.org/) and add its bin\ to PATH."
}

# ------------------------------------------------- 6. what is left to do

Step 'Next steps'
@"
1. Restart Sublime Text. Package Control reads the installed_packages list in
   Package Control.sublime-settings and downloads the 10 missing packages by
   itself -- watch the status bar.

   No Package Control yet? Command Palette (ctrl+shift+p) ->
   "Install Package Control", then restart again.

2. Tools -> Build System -> "CPP17".

3. Copy bits\ next to whatever you are solving -- the template does
   #include "bits/stdc++.h", which resolves relative to your source file:
     Copy-Item -Recurse "$Repo\bits" C:\my-contest\

4. In a .cpp file type  cbasic  then Tab to drop in the template.

5. Optional: for the "Timeit" build variant, put timeit.bat on your PATH.
"@ | Write-Host

Write-Host ""
if ($useLinks) {
    Write-Host "Done. Configs are symlinked, so edits in Sublime show up as git changes here."
} else {
    Write-Host "Done. Configs were copied -- re-run this script after changing them in the repo."
}
