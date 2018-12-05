# compiler
CC = g++
# flags
FLAGS = -Wall -g -Wno-psabi
# includes folder
INCLUDES = -I../include -Iinclude
# directories
BIN = bin
SRC = src
BIN2 = ../bin
SRC2 = ../src
all: main
main: $(BIN)/ble_test.o $(BIN)/ibeacon_test.o $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) $(SRC)/main.cpp -o bin/test \
	$(BIN)/ble_test.o $(BIN)/ibeacon_test.o $(BIN2)/iBeacon.o $(BIN2)/BLE.o $(INCLUDES)
$(BIN)/ibeacon_test.o: $(SRC)/ibeacon_test.cpp $(BIN2)/iBeacon.o $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES)
$(BIN)/ble_test.o: $(SRC)/ble_test.cpp $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES)
$(BIN2)/iBeacon.o: $(SRC2)/iBeacon.cpp $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES)
$(BIN2)/BLE.o: $(SRC2)/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES)
clean:
	sudo rm -rf $(BIN2)/*.o $(BIN2)/test
