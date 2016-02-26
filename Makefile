FLAGS = -pthread
FILES = main.c Lab3IO.c
OUT = main

all:
	mpicc $(FILES) -o $(OUT) $(FLAGS)

data:
	mpicc datagen.c Lab3IO.c -o datagen
	./datagen

bigdata: data
	./datagen -s 1000 -b 10 -p

test:
	mpicc serialtester.c Lab3IO.c -o tester
	./tester

debug:
	mpicc $(FILES) -o $(OUT) -g $(FLAGS)

clean:
	rm $(OUT) datagen tester data_input data_output
