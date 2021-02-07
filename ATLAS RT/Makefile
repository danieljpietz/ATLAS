
CFILES = src
CFLAGS = -Wall -O3 -lpthread -lrt -lJetsonGPIO 
BIN = bin

all:
	g++ $(CFLAGS) $(CFILES)/*.cpp -o $(BIN)/ATLAS.out

run:
	sudo ./$(BIN)/ATLAS.out
