@echo off
cd /D "%~dp0"
set target=gui
make %target% CXX=C:/winlibs-x86_64-posix-seh-gcc-14.1.0-llvm-18.1.5-mingw-w64msvcrt-11.0.1-r1/mingw64/bin/g++.exe

if %errorlevel% == 0 (
	cd bin/%target%
	@echo ^(started^)
	Polygonal-Intersections-%target%.exe
) else (
	pause
)
exit
