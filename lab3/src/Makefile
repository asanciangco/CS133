CC=gcc

EXEDIR	= ../bin

DWT2_EXEC = dwt2d
DWT3_EXEC = dwt3d

LINK = -O3 -lm

all: $(EXEDIR)/$(DWT3_EXEC) 

$(EXEDIR)/$(DWT3_EXEC):
	$(CC) -o $(EXEDIR)/$(DWT3_EXEC) lab3_master.c $(LINK)

clean:
	rm -rf *.o
	rm -rf $(EXEDIR)/$(DWT3_EXEC)
