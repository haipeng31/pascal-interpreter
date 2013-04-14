OBJ = RegularExpLoader.o RegularExp.o NFAModel.o NFAConverter.o DFAConverter.o DFAModel.o TokenScanner.o
CFLAGS = -c -g -Wall
main:$(OBJ) main.o
	g++ $(OBJ) main.o -o main

main.o:main.cpp
	g++ $(CFLAGS) main.cpp

TokenScanner.o:TokenScanner.cpp
	g++ $(CFLAGS) TokenScanner.cpp

DFAConverter.o:DFAConverter.cpp
	g++ $(CFLAGS) DFAConverter.cpp

DFAModel.o:DFAModel.cpp
	g++ $(CFLAGS) DFAModel.cpp

NFAConverter.o:NFAConverter.cpp
	g++ $(CFLAGS) NFAConverter.cpp

NFAModel.o:NFAModel.cpp
	g++ $(CFLAGS) NFAModel.cpp

RegularExpLoader.o:RegularExpLoader.cpp
	g++ $(CFLAGS) RegularExpLoader.cpp

RegularExp.o:RegularExp.cpp
	g++ $(CFLAGS) RegularExp.cpp

clean:
	rm *.o
