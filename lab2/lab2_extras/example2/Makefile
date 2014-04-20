CC=gcc

EXEDIR	= .

VECADD_EXEC = vecadd_ocl

LINK	 = -O3 -g -lm -I$(OPENCL_ROOT)/include -L$(OPENCL_ROOT)/lib64 -lOpenCL

all: $(EXEDIR)/$(VECADD_EXEC) 

$(EXEDIR)/$(VECADD_EXEC): vectoradd.c vecadd.cl vecadd_cl.h
	$(CC) -o $(EXEDIR)/$(VECADD_EXEC) vectoradd.c $(LINK)

vecadd_cl.h: vecadd.cl
	./cl_trans.sh vecadd.cl

clean:
	rm -rf $(EXEDIR)/$(VECADD_EXEC)
