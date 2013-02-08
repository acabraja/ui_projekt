#!/bin/bash
g++ ../server/src/server.c ../algorithms/minmax.c ../algorithms/MINMAX.cpp -pthread -o server.out
echo "Server je kompajliran"
echo "Za pokretanje ./server.out <port>"
gcc ../client/C_client/UI_client.c ../algorithms/minmax.c -o client.out
echo "Klient je kompajliran"
echo "Za pokretanje ./clent <IP> <port>"
echo "Klijenta i servere pokrenuti u razlicitim terminalima i zapoceti igru"
