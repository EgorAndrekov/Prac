all: test

test: String.o StringList.o List.o main.o
	g++ -o test String.o StringList.o List.o main.o

String.o: String.cpp String.h IterableObject.h
	g++ -c String.cpp

StringList.o: StringList.cpp StringList.h String.h List.h
	g++ -c StringList.cpp

List.o: List.cpp List.h IterableObject.h
	g++ -c List.cpp

main.o: main.cpp IterableObject.h String.h List.h StringList.h
	g++ -c main.cpp

clean:
	rm -f *.o test
