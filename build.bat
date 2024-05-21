@echo off
cd /D "%~dp0"
make

if %errorlevel% == 0 (
	echo ===========================================================================
	cd bin/gui
	"Polygonal-Intersections-GUI.exe"
) else (
	pause
)
exit
