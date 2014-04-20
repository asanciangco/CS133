#!/bin/sh

export OPENCL_ROOT=/usr/local/cuda-5.5
export LD_LIBRARY_PATH=$OPENCL_ROOT/lib64:$LD_LIBRARY_PATH
export PATH=$OPENCL_ROOT/bin:$PATH
