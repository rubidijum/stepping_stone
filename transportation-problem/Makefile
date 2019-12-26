PROGRAM = main
CC = g++
CFLAGS  = -std=c++1z -g -Wall -Wextra
LDLIBS = -lm

main: main.o
	$(CC) $(CFLAGS)  -o main *.o $(LDLIBS)
main.o: main.cpp  
	$(CC) $(CFLAGS) -c main.cpp $(LDLIBS)

clean:
	@rm -f *.o main
