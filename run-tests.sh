#!/bin/sh

cd ./build

./strs-test || echo "ошибка strs"
./inifile-test "../config0.ini" "../config1.ini" "../config2.ini" || echo "ошибка inifile"
