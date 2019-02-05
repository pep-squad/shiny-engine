# compiler
CC = g++
# flags
FLAGS = -Wall -Wno-psabi -g
# includes folder
INCLUDES = -Iinclude
# libraries
LIBRARIES = -lwiringPi -lpthread
# directories
BIN = bin
SRC = src
all: main
main: bin/BLE.o bin/Motor.o
	sudo $(CC) $(FLAGS) src/main.cpp -o bin/app bin/BLE.o bin/Motor.o $(INCLUDES) $(LIBRARIES)
bin/BLE.o: src/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
bin/Motor.o: src/Motor.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/ble_scan $(BIN)/ble_receive
