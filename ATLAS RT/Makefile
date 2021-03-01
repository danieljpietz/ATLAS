
CFILES = src
CFLAGS = -g -Wall -O3 -lpthread -lrt -lJetsonGPIO 
BIN = bin

all:
	g++ $(CFILES)/*.cpp $(CFLAGS) -o $(BIN)/ATLAS.out

run:
	sudo ./$(BIN)/ATLAS.out
