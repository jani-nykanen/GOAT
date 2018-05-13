#!/bin/sh
make
if [ $? -eq 0 ]; then
    ./goat $1 $2 $3
fi
