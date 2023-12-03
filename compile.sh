#!/bin/bash

# cc ./*c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
gcc -o server ./src/server.c
gcc -o client ./src/client.c


