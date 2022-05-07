jack : main.o JackTokenizer.o  CompilationEngine.o SymbolTable.o VMWriter.o
	g++  main.o JackTokenizer.o CompilationEngine.o SymbolTable.o VMWriter.o -o Jack
	rm JackTokenizer.o main.o CompilationEngine.o  SymbolTable.o  VMWriter.o

main.o : main.cpp ./include/CompilationEngine.h
	g++ -c main.cpp -o main.o

JackTokenizer.o : JackTokenizer.cpp ./include/JackTokenizer.h
	g++ -c JackTokenizer.cpp -o JackTokenizer.o

CompilationEngine.o : CompilationEngine.cpp ./include/CompilationEngine.h
	g++ -c CompilationEngine.cpp -o CompilationEngine.o 

SymbolTable.o : SymbolTable.cpp ./include/SymbolTable.h
	g++ -c SymbolTable.cpp -o SymbolTable.o

VMWriter.o : VMWriter.cpp ./include/VMWriter.h
	g++ -c VMWriter.cpp -o VMWriter.o 









