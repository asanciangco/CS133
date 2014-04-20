#! /bin/bash
gcc hw2c.c -fopenmp -lm -o hw2c
./hw2c 100000
rm hw2c
