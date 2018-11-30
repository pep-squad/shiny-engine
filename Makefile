# compiler
CC = g++
# flags
FLAGS = -Wall -g
# includes folder
INCLUDES = -Iinclude
# directories
BIN = bin
SRC = src
all: bin/iBeacon.o bin/BLE.o
	sudo $(CC) $(FLAGS) src/main.cpp -o bin/scanner bin/iBeacon.o bin/BLE.o -Iinclude
bin/iBeacon.o: src/iBeacon.cpp bin/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES)
bin/BLE.o: src/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/scanner
