#!/bin/bash

cflags="$cflags -std=c++11"
cflags="$cflags `pkg-config --libs --cflags consor`"

for file in Examples/*.cpp; do
	#generate the file
	echo '#include <iostream>
	#include <Consor/PlatformConsoleRenderer.hpp>
	#include <Consor/PlatformInputSystem.hpp>
	#include <Consor/Containers/WindowContainer.hpp>
	#include <Consor/WindowSystem.hpp>
	#include <Consor/Skin.hpp>
	#include <Consor/Util/Prompts.hpp>
	#include <Consor/Util/StringUtils.hpp>
	#include <Consor/Util/Math.hpp>
	
	using namespace std;
	using namespace Consor;

	int main (int argc, char *argv[])
	{' > /tmp/example.cpp

	cat $file >> /tmp/example.cpp
	echo 'return 0;
}' >> /tmp/example.cpp
	
	echo "compiling $file..."
	result_file="${file%.cpp}.txt"
	c++ /tmp/example.cpp -o /tmp/example $cflags &> /tmp/example.log
		
	if [ $? == 0 ]; then # did we build it okay?
		/tmp/example &> $result_file
	else
		echo "Build failed:"
		cat /tmp/example.log
		cat /tmp/example.log > $result_file
	fi
done
