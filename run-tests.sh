#!/bin/sh

cd ./build
set -e
echo "проверка strs" || echo "    *** ошибка ***"
./strs-test
echo "проверка 1 inifile"
./inifile-test "../config0.ini" "../config1.ini" "../config2.ini" || echo "    *** ошибка ***"
echo "проверка 2 inifile"
cmp -s "../config0.ini" "../config1.ini" || echo "    *** ошибка ***"
