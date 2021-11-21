CXX=g++
CXX_FLAGS=-std=c++11 -O3 -Wall -Werror -Wpedantic
LDFLAGS=-lcrypto

CXX_SOURCES=game.cpp
OUT_NAME=game

RM=rm -rf

# The project is tiny so do clean build at all times
# to suppress $TARGET is up to date" in case we're
# updating the module.
game: clean
	$(CXX) $(CXX_FLAGS) $(CXX_SOURCES) -o$@ $(LDFLAGS)

all: game

clean:
	$(RM) $(OUT_NAME)
