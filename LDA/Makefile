# Makefile

CXX=g++
CXX_FLAGS=-O3 -Wall

lda_test: lda.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./src/lda_test.cc lda.o -o lda_test

lda: lda.o
	${CXX} ${CXX_FLAGS} ./src/lda_main.cc lda.o -o lda

lda.o:
	${CXX} ${CXX_FLAGS} -c ./src/lda.cc

tot_test: tot.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./src/tot_test.cc tot.o -o tot_test

tot: tot.o
	${CXX} ${CXX_FLAGS} ./src/tot_main.cc tot.o -o tot

tot.o:
	${CXX} ${CXX_FLAGS} -c ./src/tot.cc


clean:
	rm -f lda.o tot.o