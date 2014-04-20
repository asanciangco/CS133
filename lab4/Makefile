CFLAG=-O3
BIN=.
DATA=./data
STRING1=largeX.txt
STRING2=largeY.txt

all: seq par

seq: smith_waterman.c
	gcc $(CFLAG) -o $(BIN)/smith_waterman_sequential smith_waterman.c

par: smith_waterman_mpi.c
	mpicc $(CFLAG) -o $(BIN)/smith_waterman_parallel smith_waterman_mpi.c

run: run_seq run_par

run_seq: seq
	$(BIN)/smith_waterman_sequential $(DATA)/$(STRING1) $(DATA)/$(STRING2)

run_par: par
	clear
	mpirun -np 12 $(BIN)/smith_waterman_parallel $(DATA)/$(STRING1) $(DATA)/$(STRING2)
	
clean:
	rm smith_waterman_parallel
	rm smith_waterman_sequential