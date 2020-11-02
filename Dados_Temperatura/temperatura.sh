#!/bin/bash

w3m https://www.climatempo.com.br/previsao-do-tempo/agora/cidade/558/saopaulo-sp | cat | grep 'º' >> dados.txt;

sleep 3600;

