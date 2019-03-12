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
all: motor hcsr04 comms rgb follow
motor: $(SRC)/motor_main.cpp $(BIN2)/Motor.o
	sudo $(CC) $(FLAGS) $(SRC)/motor_main.cpp -o $(BIN)/motor $(BIN2)/Motor.o $(INCLUDES) $(LIBRARIES)
hcsr04: $(SRC)/hcsr04_test.cpp $(BIN2)/HCSR04.o
	sudo $(CC) $(FLAGS) $(SRC)/hcsr04_test.cpp -o $(BIN)/object $(BIN2)/HCSR04.o $(INCLUDES) $(LIBRARIES)
comms: $(BIN)/serial_number.o
	sudo $(CC) $(FLAGS) $(SRC)/comm_main.cpp -o $(BIN)/comms $(BIN)/serial_number.o $(BIN2)/BLE.o $(INCLUDES) $(LIBRARIES)
rgb: $(BIN2)/TCS3200.o
	sudo $(CC) $(FLAGS) $(SRC)/tcs3200_test.cpp -o $(BIN)/rgb $(BIN2)/TCS3200.o $(INCLUDES) $(LIBRARIES)
follow: $(BIN2)/HCSR04.o $(BIN2)/Motor.o
	sudo $(CC) $(FLAGS) $(SRC)/distance_follow.cpp -o $(BIN)/follow $(BIN2)/HCSR04.o $(BIN2)/Motor.o $(INCLUDES) $(LIBRARIES)
line-bounce: $(BIN2)/TCS3200.o $(BIN2)/Motor.o
	sudo $(CC) $(FLAGS) $(SRC)/line_bounce.cpp -o $(BIN)/line-bounce $(BIN2)/TCS3200.o $(BIN2)/Motor.o $(INCLUDES) $(LIBRARIES)
$(BIN)/serial_number.o: $(SRC)/serial_number.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/HCSR04.o: $(SRC2)/HCSR04.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/BLE.o: $(SRC2)/BLE.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/Motor.o: $(SRC2)/Motor.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
$(BIN2)/TCS3200.o: $(SRC2)/TCS3200.cpp
	sudo $(CC) $(FLAGS) -c -o $@ $< $(INCLUDES) $(LIBRARIES)
clean:
	sudo rm -rf $(BIN)/*.o $(BIN)/motor $(BIN)/object $(BIN)/comms $(BIN)/rgb $(BIN)/follow $(BIN)/line-bounce
