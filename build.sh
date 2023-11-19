#!/bin/sh

if [ ! -d build ]; then
	mkdir build
fi


build_command="g++ path_finding/pathfinding.cpp -o build/pathfinding -lGL -lGLU -lglut"
if [ "$1" == "debug" ]; then
	build_command="$build_command -g"
fi

$build_command

cp path_finding/input.bmp build/input.bmp
