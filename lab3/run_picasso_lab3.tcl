# Create Project 
create_project -name lab3_proj -platform zc706-linux-uart -force

# Add host code
add_host_src -filename "src/lab3_master.c" -type source

# Create a kernel for the matrix multiplication
create_kernel -id fwd97_pd -type clc
add_kernel_src -id fwd97_pd -filename "src/fwd97_pd.cl"

# Create a Xilinx OpenCL Kernel Binary Container
create_xclbin fwd97_pd

# Select the execution target of the kernel
map_add_kernel_instance -xclbin fwd97_pd -id fwd97_pd -target fpga0:OCL_REGION_0

# Compile the host code
compile_host -arch arm -cflags "-g -Wall -D FPGA_DEVICE"

# Build the system
build_system

# Package SD Card Image
build_sdimage

exit
