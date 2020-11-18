#!/bin/bash
set -x

make randgen

for((i = 0; i < $1;i++))
do
	./randgen > inputfiles/input$i
	sleep 1
done

