TARGET=test_mem
BIN=../bin

CC=g++
AM_CXXFLAGS = -std=c++11

all: bin $(TARGET)

bin:
	mkdir -p $(BIN)

$(TARGET): $(TARGET).c
	$(CC) -o $(BIN)/$@ $< 

clean:
	@rm -vf $(BIN)/$(TARGET) *.o

clear: clean

test:
	$(BIN)/$(TARGET)
