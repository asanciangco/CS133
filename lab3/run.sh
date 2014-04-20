#! /bin/bash
clear

# Make and run the project, cleaning up afterwards
make
./bin/dwt3d /usr/local/cs133/lab1/data/3d256f.dat
make clean

# Check the output of the program
./bin/check_each dwt3d_out.dat ./bin/3d256f_level1_single.dat


