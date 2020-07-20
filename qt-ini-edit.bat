PATH=%PATH%;c:\Qt\5.15.0\msvc2019_64\bin;c:\Qt\5.15.0\msvc2019_64\plugins\platforms
::cd build
::qt-ini-edit.exe "../config0.ini" "../config3.ini"

cd "out"
cd "build"
cd "x64-Debug (по умолчанию)"
qt-ini-edit.exe "../../../config0.ini" "../../../config3.ini"
cd ..
cd ..
cd ..
