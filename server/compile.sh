#!/bin/bash
g++   src/server.c ../algorithms/minmax.c ../algorithms/MINMAX.cpp -pthread -o server.out
echo "Pokretanje sa ./server.out <port>"
echo "Za testiranje stavite port 5000"
