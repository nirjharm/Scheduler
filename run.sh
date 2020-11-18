#!/bin/bash

make clean
make

./scheduler <inputfiles/$1 2>/dev/null >inputfiles/Schedule

cp graphicfiles/htmlbase.txt Result.html
./converter < inputfiles/Schedule >> Result.html 


