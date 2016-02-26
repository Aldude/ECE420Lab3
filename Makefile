FLAGS = -pthread
FILES = main.c Lab3IO.c
OUT = main

all:
	gcc $(FILES) -o $(OUT) $(FLAGS)

data:
	gcc datagen.c Lab3IO.c -o datagen
	./datagen

bigdata: data
	./datagen -s 1000 -b 10 -p

test:
	gcc serialtester.c Lab3IO.c -o tester
	./tester

debug:
	gcc $(FILES) -o $(OUT) -g $(FLAGS)

clean:
	rm $(OUT) datagen tester data_input data_output
