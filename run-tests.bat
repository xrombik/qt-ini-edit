@echo off
@chcp 1251
cd build
strs-test.exe
if %ERRORLEVEL%==1 (
	echo "    *** ������ strs"
	goto end
)

inifile-test.exe "..\config0.ini" "..\config1.ini" "..\config2.ini"
if %ERRORLEVEL%==1 (
	echo "    *** ������ inifile"
	goto end
)

:end
	cd ..
