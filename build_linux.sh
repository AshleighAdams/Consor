#!/bin/bash

cd ./Consor

files=`find ./ -name "*.cpp" | grep -v "Windows"`
g++-4.8 -g -std=c++11 -pthread $files -o ../consor_linux_test -lboost_locale

objs=`find ./ -name "*.o"`
rm $objs

cd -

#c++ 
