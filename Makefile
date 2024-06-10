OBJECTS = counts.o tests.o

# OS identification from: https://stackoverflow.com/questions/714100/os-detecting-makefile
OS := $(shell uname -s)

ifeq ($(OS), Darwin) 
  INCLUDE_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/include
  LIB_PATH := /opt/homebrew/Cellar/criterion/2.4.1_1/lib
  CC = gcc-12
endif
ifeq ($(OS), Linux) 
  INCLUDE_PATH := /util/criterion/include
  LIB_PATH := /util/criterion/lib/x86_64-linux-gnu
  CC = gcc
endif

CC = gcc
NO_DEBUG_FLAGS = -c -Wall -std=c11
DEBUG_FLAGS = -g -c -Wall -std=c11
FLAGS = $(DEBUG_FLAGS)

SRC = functions.c
TST = tests.c
GEX = counts-gprof
VEX = mainFile
TEX = tests
FLAGS = -pg -fprofile-arcs -ftest-coverage
CFLAGS = -L $(LIB_PATH) -I $(INCLUDE_PATH)
CLIB = -lcriterion

.PHONY: clean
clean: 
	rm -rf *~ *.o $(GEX) $(TEX) $(VEX) *.dSYM *.gc?? analyze.txt gmon.out sandbox-gmon.* screaming.txt testingFile.txt

functions.o: functions.c globals.h functions.h
	gcc -c -g -O0 -Wall -std=c11 $(SRC)

tests.o: tests.c functions.h
	gcc -c -g -O0 -Wall -std=c11 -I $(INCLUDE_PATH) $(TST)

c-exec: functions.o
	gcc -g -O0 -Wall $(FLAGS) $(CFLAGS) -o $(GEX) $(SRC) main.c

c-test: tests.o functions.o
	gcc -g -O0 -Wall $(FLAGS) $(CFLAGS) -o $(TEX) $(SRC) tests.c $(CLIB)

v-exec: functions.o
	gcc -g -O0 -Wall $(CFLAGS) -o $(VEX) $(SRC) main.c

.PHONY: andRunPerformance
andRunPerformance:
	make clean
	make c-exec
	@echo "******************************************************************************"
	@echo "** Gathering performance data."
	@echo "******************************************************************************"
	./$(GEX)
	gprof -b ./$(GEX) gmon.out > analyze.txt 
	gcov $(SRC) 
	@echo "******************************************************************************"
	@echo "** Done!"
	@echo "** Look at analyze.txt for gprof timing data"
	@echo "** Look at $(SRC).gcov for gcov annotated source code with execution counts."
	@echo "******************************************************************************"

.PHONY: andRunTests
andRunTests:
	make clean
	make c-test
	make v-exec
	@echo "******************************************************************************"
	@echo "** Running tests. This may take a minute..."
	@echo "******************************************************************************"
	./$(TEX)
	gcov $(SRC) 
	@echo "******************************************************************************"
	@echo "** Done!"
	@echo "** Look at $(SRC).gcov for gcov annotated source code with execution."
	@echo "******************************************************************************"

.PHONY: andRunCallGrind
andRunCallGrind:
	make clean
	make v-exec
	@echo "******************************************************************************"
	@echo "** Running program on large file using callgrind. This may take a minute... "
	@echo "******************************************************************************"
	valgrind --tool=callgrind ./$(VEX) 1000 1000
	@echo "******************************************************************************"
	@echo "** Done!"
	@echo "** Use callgrind_annotate to interpret the data file."
	@echo "******************************************************************************"