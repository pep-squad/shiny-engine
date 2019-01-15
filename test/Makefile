# compiler
CC = g++
# flags
FLAGS = -Wall -g -Wno-psabi
# includes folder
INCLUDES = -I../include -Iinclude
# libraries
LIBRARIES = -lwiringPi -lpthread
# directories
BIN = bin
SRC = src
BIN2 = ../bin
SRC2 = ../src
all: main
main: $(BIN)/ble_test.o $(BIN)/ibeacon_scan.o $(BIN)/hcsr04_test.o $(BIN)/motor_test.o
	sudo $(CC) $(FLAGS) $(SRC)/main.cpp -o bin/test \
	 $(BIN)/ble_test.o $(BIN)/ibeacon_scan.o $(BIN)/hcsr04_test.o $(BIN)/motor_test.o $(BIN)/serial_number.o \
	 $(BIN2)/iBeacon.o $(BIN2)/BLE.o $(BIN2)/HCSR04.o $(BIN2)/Motor.o \
	 $(INCLUDES) $(LIBRARIES)
$(BIN)/hcsr04_test.o: $(SRC)/hcsr04_test.cpp $(BIN2)/HCSR04.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/ibeacon_scan.o: $(SRC)/ibeacon_scan.cpp $(BIN2)/iBeacon.o $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/ble_test.o: $(SRC)/ble_test.cpp $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/motor_test.o: $(SRC)/motor_test.cpp $(BIN2)/Motor.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/serial_number.o: $(SRC)/serial_number.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/HCSR04.o: $(SRC2)/HCSR04.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/iBeacon.o: $(SRC2)/iBeacon.cpp $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/BLE.o: $(SRC2)/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/Motor.o: $(SRC2)/Motor.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/test
