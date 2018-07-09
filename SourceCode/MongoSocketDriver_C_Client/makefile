main: main.c DatabaseHandler.o
	cc -o main main.c DatabaseHandler.o -Wall
DatabaseHandler.o: DatabaseHandler.h DatabaseHandler.c
	cc -c DatabaseHandler.c -Wall
clean:
	rm *.o main