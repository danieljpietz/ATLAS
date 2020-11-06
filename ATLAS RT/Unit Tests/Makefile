CC=g++

CDEFS=
CFLAGS= -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lpthread


CFILES= RunTests.cpp CircularBufferTests.cpp


all:
	$(CC) -c $(CFLAGS) $(CFILES) $(LIBS)
	$(CC) $(LIBS) *.o -o RunTests
	rm *.o
