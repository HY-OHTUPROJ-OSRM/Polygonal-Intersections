@echo off
cd /D "%~dp0"
set target=cli
make %target% CXX=C:/winlibs-x86_64-posix-seh-gcc-10.1.0-llvm-10.0.0-mingw-w64-7.0.0-r3/mingw64/bin/g++.exe

if %errorlevel% == 0 (
	cd bin/%target%
	@echo ^(started^)
	Polygonal-Intersections-%target%.exe
) else (
	pause
)
exit
