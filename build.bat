@echo off
cd /D "%~dp0"
make gui

if %errorlevel% == 0 (
	cd bin/gui
	"Polygonal-Intersections-GUI.exe"
) else (
	pause
)
exit
