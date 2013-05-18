#!/bin/bash

files=`find ./ -name *.cpp | grep -v "Windows"`

echo c++ $files -o consor_linux_test
#c++ 
