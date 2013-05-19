#!/bin/bash

files=`find ./ -name *.cpp | grep -v "Windows"`
echo g++-4.8 $files -o consor_linux_test

objs=`find ./ -name *.o`
rm $objs

#c++ 
