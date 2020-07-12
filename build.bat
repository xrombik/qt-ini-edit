if not exist "build" mkdir "build"
cd build
cmake ..
msbuild qt-ini-edit.sln
cd ..
