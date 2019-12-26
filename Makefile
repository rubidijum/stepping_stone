PROGRAM = main
CC = g++
CFLAGS  = -std=c++17 -g -Wall -Wextra
LDLIBS = -lm

main: main.o transportation_problem.o stepping_stone.o
	$(CC) $(CFLAGS)  -o main $^ $(LDLIBS)
main.o: main.cpp ./transportation-problem/transportation_problem.hpp
	$(CC) $(CFLAGS) -c $^ $(LDLIBS)
transportation_problem.o: ./transportation-problem/transportation_problem.cpp ./transportation-problem/transportation_problem.hpp
	$(CC) $(CFLAGS) -c $^ $(LDLIBS)
stepping_stone.o: stepping_stone.cpp stepping_stone.hpp
	$(CC) $(CFLAGS) -c $^ $(LDLIBS)

clean:
	@rm -f *.o main
