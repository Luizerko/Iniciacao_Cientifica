#!/bin/bash

rm Erro_PACL.txt;

make;

for i in $(seq 1 10000); do
	./APACL;
	sleep 1;
done

make clean;
