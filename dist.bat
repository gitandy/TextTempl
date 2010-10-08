@echo off
set PATH=C:\Qt\2010.04\qt\bin;C:\Qt\2010.04\mingw\bin;C:\Programme\Git\bin;C:\Programme\7-Zip;C:\Programme\Inno Setup 5
mingw32-make -f Makefile.Dist installer
pause