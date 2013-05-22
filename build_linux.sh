#!/bin/bash

cd ./Consor

# we want to use g++-4.8, you should change this to your compiler
compiler="g++-4.8"
link=""

# general stuff
output="linux_build/consor"
std="c++11"
debug="-g"
files=`find ./ -name "*.cpp" | grep -v "Windows"`

# link to the boost locale lib
link=$link"-lboost_locale "

echo `$compiler $debug -std=$std -pthread $files -o ../$output $link`

#objs=`find ./ -name "*.o"`
#rm $objs

cd -

