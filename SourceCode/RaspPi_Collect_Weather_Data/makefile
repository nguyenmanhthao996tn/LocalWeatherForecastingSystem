serialTest: serialTest.c OutputData.o myBuffer.o
	cc -o serialTest serialTest.c OutputData.o myBuffer.o -Wall

OutputData.o: OutputData.h OutputData.c
	cc -c OutputData.c -Wall

myBuffer.o: myBuffer.h myBuffer.c
	cc -c myBuffer.c -Wall

clean:
	rm *.o serialTest