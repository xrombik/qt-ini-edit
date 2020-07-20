@echo off
@chcp 1251
cd "build"

echo проверка strs
strs-test.exe
if %ERRORLEVEL%==1 (
	echo     *** ошибка strs ***
	goto end
)

echo проверка 1 inifile
inifile-test.exe "..\config0.ini" "..\config1.ini" "..\config2.ini"
if %ERRORLEVEL%==1 (
	echo     *** ошибка inifile ***
	goto end
)

echo проверка 2 inifile
fc "..\config0.ini" "..\config1.ini" > nul
if %ERRORLEVEL%==1 (
	echo     *** ошибка inifile ***
	goto end
)

:end
	pause
	cd ..
