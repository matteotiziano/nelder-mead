
FLAGS = -std=c99 -Wall -Wextra -pedantic
CC_OPT = -O3 -ffast-math -fno-common
BIN = nm
SRC = main.c nelder_mead.c point.c ackley.c

compile:
	gcc $(FLAGS) $(CC_OPT) -o $(BIN) $(SRC)

test: compile
	time ./$(BIN) -2.10 -3.04 4.50 
