@echo off

REM By erekle https://codeforces.com/profile/erekle

REM ------------------------------------------
REM ------------------------------------------
REM This utility times execution of programmes
REM ------------------------------------------
REM ------------------------------------------


REM For usage and help, run this bat file without any arguments



REM -------- BEGIN: Read main parameters --------
set self=%~n0
set executable=%1
set flag=%2
set infile=%3
REM -------- BEGIN: Read main parameters --------


REM -------- BEGIN: Read command input --------
set params=%2
shift
shift
:loop1
if "%1"=="" goto after_loop
set params=%params% %1
shift
goto loop1
:after_loop
REM -------- E N D: Read command input --------


IF [%executable%]==[] (
	echo.
	echo %self%: Times execution of a program or a command
	echo.
	echo Usage:
	echo.        [97m%self% [96mprog.exe
	echo.        [97m%self% [96mprog.exe [92mparam1 param2 param3 [90m...
	echo.        [97m%self% [96mprog.exe [95m-f [93minput.txt[0m
	exit \b
)

IF NOT EXIST %executable% (
	echo [91mProgram not found: [95m%executable%[0m
	exit \b
)

IF [%flag%]==[] (
	powershell -Command "$duration = Measure-Command { .\%executable% | Out-Default }; $seconds = $duration.TotalSeconds; Write-Host "`n____________________________________________" -ForegroundColor Cyan; Write-Host 'Execution time (seconds):' -NoNewline; Write-Host "`t$seconds" -ForegroundColor Cyan; exit $LastExitCode"
	GOTO checkerrors
	exit \b
)

IF [%flag%]==[-f] (
	IF [%infile%]==[] (
		echo [91mOption -f should be followed by input file name[0m, for example:
		echo.      [96m%self% %executable% [95m-f [93minput.txt[0m
		exit \b
	)
) ELSE (
	powershell -Command "$duration = Measure-Command { echo %params% | .\%executable% | Out-Default }; $seconds = $duration.TotalSeconds; Write-Host "`n____________________________________________" -ForegroundColor Cyan; Write-Host 'Execution time (seconds):' -NoNewline; Write-Host "`t$seconds" -ForegroundColor Cyan; exit $LastExitCode"
	GOTO checkerrors
	exit \b
)

REM Consider last option: %flag% is -f and %infile% was supplied

IF NOT EXIST %infile% (
	echo [91mInput file not found: [95m%infile%[0m
	exit \b
)

powershell -Command "$duration = Measure-Command { .\%executable% | Out-Default }" < %infile%; $seconds = $duration.TotalSeconds; Write-Host "`n____________________________________________" -ForegroundColor Cyan; Write-Host 'Execution time (seconds):' -NoNewline; Write-Host "`t$seconds" -ForegroundColor Cyan; exit $LastExitCode
exit \b

:return
echo [97m____________________________________________[0m
set /p ch=Press enter to continue...
exit

:checkerrors
IF NOT %errorlevel%==0 (
	echo [91mWarning: [0mExit code:             [91m%errorlevel%[0m
) ELSE (
	echo [0mExit code:                      [92m%errorlevel%[0m
)
GOTO return