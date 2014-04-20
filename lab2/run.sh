#! /bin/bash
clear
make
./bin/dwt3d /usr/local/cs133/lab1/data/3d256f.dat 3
#make clean
./check_each dwt3d_out.dat /usr/local/cs133/lab1/data/3d256f_level3.dat
