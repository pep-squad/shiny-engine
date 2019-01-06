# compiler
CC = g++
# flags
FLAGS = -Wall -Wno-psabi
# includes folder
INCLUDES = -Iinclude
# libraries
LIBRARIES = -lwiringPi -lpthread
# directories
BIN = bin
SRC = src
all: main
main: bin/iBeacon.o bin/BLE.o bin/Motor.o
	sudo $(CC) $(FLAGS) src/main.cpp -o bin/app bin/iBeacon.o bin/BLE.o bin/Motor.o $(INCLUDES) $(LIBRARIES)
bin/iBeacon.o: src/iBeacon.cpp bin/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
bin/BLE.o: src/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
bin/Motor.o: src/Motor.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/ble_scan $(BIN)/ble_receive
