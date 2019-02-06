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
main: $(BIN)/BLE.o $(BIN)/Motor.o $(BIN)/TCS3200.o
	sudo $(CC) $(FLAGS) src/main.cpp -o $(BIN)/app $(BIN)/BLE.o $(BIN)/Motor.o $(BIN)/TCS3200.o $(INCLUDES) $(LIBRARIES)
$(BIN)/BLE.o: src/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/Motor.o: src/Motor.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN)/TCS3200.o: src/TCS3200.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/app
