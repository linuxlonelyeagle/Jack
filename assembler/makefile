assmbler : main.o parser.o symboltable.o
	g++ main.o parser.o symboltable.o  -o assmbler -g
	rm main.o parser.o  symboltable.o
main.o : main.cpp ./include/parser.h
	g++ -c main.cpp -o main.o -g

parser.o : parser.cpp ./include/parser.h
	g++ -c parser.cpp -o parser.o -g
symboltable.o : symboltable.cpp ./include/symboltable.h
	g++ -c symboltable.cpp -o symboltable.o -g 
clean:
	rm assmbler
