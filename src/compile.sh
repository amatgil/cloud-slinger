#!/bin/sh

FLAGS=""
while read p; do FLAGS="$FLAGS $p"; done < ../compile_flags.txt

LIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

clang main.c -g $FLAGS -O1 -o main $LIBS
