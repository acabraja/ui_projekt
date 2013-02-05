#!/bin/bash
gcc src/server.c ../algorithms/minmax.c -pthread -o server.out
echo "Pokretanje sa ./server.out <port>"
echo "Za testiranje stavite port 5000"
