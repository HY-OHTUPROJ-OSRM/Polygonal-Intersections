@echo off
cd /D "%~dp0"
make

if %errorlevel% == 0 (
	echo ===========================================================================
	cd bin
	"Polygonal-Intersections.exe"
) else (
	pause
)
exit
