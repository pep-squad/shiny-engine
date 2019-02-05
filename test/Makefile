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
all: motor hcsr04 communications
motor: $(SRC)/motor_main.cpp $(BIN2)/Motor.o
	sudo $(CC) $(FLAGS) $(SRC)/motor_main.cpp -o $(BIN)/motor $(BIN2)/Motor.o $(INCLUDES) $(LIBRARIES)
hcsr04: $(BIN)/hcsr04_test.o
	sudo $(CC) $(FLAGS) $(SRC)/hcsr04_main.cpp -o $(BIN)/object $(BIN)/hcsr04_test.o $(BIN2)/HCSR04.o $(INCLUDES) $(LIBRARIES)
communications: $(BIN)/ble_test.o $(BIN)/serial_number.o
	sudo $(CC) $(FLAGS) $(SRC)/comm_main.cpp -o $(BIN)/comm $(BIN)/ble_test.o $(BIN)/serial_number.o $(BIN2)/BLE.o $(INCLUDES) $(LIBRARIES)
$(BIN)/hcsr04_test.o: $(SRC)/hcsr04_test.cpp $(BIN2)/HCSR04.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/ble_test.o: $(SRC)/ble_test.cpp $(BIN2)/BLE.o
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/serial_number.o: $(SRC)/serial_number.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/HCSR04.o: $(SRC2)/HCSR04.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/BLE.o: $(SRC2)/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/Motor.o: $(SRC2)/Motor.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/motor $(BIN)/object $(BIN)/comm
