vm : main.o parser.o   codewrite.o
	g++ main.o parser.o codewrite.o -o vm 
	rm main.o parser.o  codewrite.o
main.o : main.cpp ./include/parser.h ./include/codewrite.h  ./include/asmdefine.h
	g++ -c main.cpp -o main.o

parser.o : parser.cpp ./include/parser.h
	g++ -c parser.cpp -o parser.o

codewrite.o :codewrite.cpp ./include/codewrite.h
	g++ -c codewrite.cpp -o codewrite.o