# Makefile

CXX=g++
CXX_FLAGS=-O3 -Wall

lda_test: lda.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./lda_test.cc lda.o -o lda_test

lda: lda.o
	${CXX} ${CXX_FLAGS} ./lda_main.cc lda.o -o lda

lda.o:
	${CXX} ${CXX_FLAGS} -c ./lda.cc

tot_test: tot.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./tot_test.cc tot.o -o tot_test

tot: tot.o
	${CXX} ${CXX_FLAGS} ./tot_main.cc tot.o -o tot

tot.o:
	${CXX} ${CXX_FLAGS} -c ./tot.cc


clean:
	rm -f lda.o tot.o