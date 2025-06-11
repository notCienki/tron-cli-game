CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = `pkg-config --libs ncurses`
INCLUDES = `pkg-config --cflags ncurses`

all: tron

tron: main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) main.cpp -o tron $(LDFLAGS)

clean:
	rm -f tron

