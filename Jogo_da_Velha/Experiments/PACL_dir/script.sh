#!/bin/bash

rm Erro_PACL.txt;

make;

for i in $(seq 1 10000); do
	./PACL;
	sleep 1;
done

make clean;
