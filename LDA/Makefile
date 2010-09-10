# Makefile

CXX=g++
CXX_FLAGS=-O3 -Wall

test: lda.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./test_lda.cpp lda.o -o test

main: lda.o
	${CXX} ${CXX_FLAGS} ./main.cpp lda.o -o main

lda.o:
	${CXX} ${CXX_FLAGS} -c ./lda.cpp

clean:
	rm -f lda.o