CC=gcc

EXEDIR	= ../bin

DWT2_EXEC = dwt2d
DWT3_EXEC = dwt3d

LINK = -O3 -lm #-fopenmp

all: $(EXEDIR)/$(DWT2_EXEC) $(EXEDIR)/$(DWT3_EXEC) 

$(EXEDIR)/$(DWT2_EXEC): cdf97.* dwt2d_main.* util.*
	$(CC) -o $(EXEDIR)/$(DWT2_EXEC) cdf97.c dwt2d_main.c util.c $(LINK)

$(EXEDIR)/$(DWT3_EXEC): cdf97.* dwt3d_main.* util.*
	$(CC) -o $(EXEDIR)/$(DWT3_EXEC) cdf97.c dwt3d_main.c util.c $(LINK)

clean:
	rm -rf *.o
	rm -rf $(EXEDIR)/$(DWT2_EXEC)
	rm -rf $(EXEDIR)/$(DWT3_EXEC)
