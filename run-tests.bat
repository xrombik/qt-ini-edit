@echo off
@chcp 1251
cd "build"

echo �������� strs
strs-test.exe
if %ERRORLEVEL%==1 (
	echo     *** ������ strs ***
	goto end
)

echo �������� 1 inifile
inifile-test.exe "..\config0.ini" "..\config1.ini" "..\config2.ini"
if %ERRORLEVEL%==1 (
	echo     *** ������ inifile ***
	goto end
)

echo �������� 2 inifile
fc "..\config0.ini" "..\config1.ini" > nul
if %ERRORLEVEL%==1 (
	echo     *** ������ inifile ***
	goto end
)

:end
	pause
	cd ..
