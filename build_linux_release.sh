#!/bin/bash

cd ./Consor

# we want to use g++-4.8, you should change this to your compiler
compiler="g++-4.8"
link=""

# general stuff
output="linux_build/Consor.a"
std="c++11"
debug="-g3"
files=`find ./ -name "*.cpp" | grep -v "Windows"`

# link to the boost locale lib
link=$link"-lboost_locale "

echo `$compiler -c $debug -std=$std -pthread $files $link`



objs=`find ./ -name "*.o"`

ar rvs ../$output $objs
rm $objs

cd -

