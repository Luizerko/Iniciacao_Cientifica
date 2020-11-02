#!/bin/bash

rm Erro_APACL.txt;

make;

for i in $(seq 1 10000); do
	./APACL $i;
done

make clean;
