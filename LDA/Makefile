# Makefile

CXX=g++
CXX_FLAGS=-O3 -Wall

lda_test: lda.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./src/tests/lda_test.cc lda.o -o lda_test

main: lda.o tot.o
	${CXX} ${CXX_FLAGS} ./src/main.cc lda.o tot.o -o main

lda.o:
	${CXX} ${CXX_FLAGS} -c ./src/lda.cc

tot_test: tot.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./src/tests/tot_test.cc tot.o -o tot_test

tot.o:
	${CXX} ${CXX_FLAGS} -c ./src/tot.cc


clean:
	rm -f lda.o tot.o