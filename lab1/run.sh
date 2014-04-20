#! /bin/bash
clear
make
./bin/dwt3d /usr/local/cs133/lab1/data/3d512f.dat 3
#make clean
./bin/check dwt3d_out.dat /usr/local/cs133/lab1/data/3d512f_level3_result.dat
