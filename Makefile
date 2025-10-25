CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g
LDFLAGS = -lncurses

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = tron
PREFIX ?= /usr/local

.PHONY: all clean install uninstall run debug help

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete! Run with: ./$(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

run: all
	@echo "Starting game..."
	./$(TARGET)

debug: CXXFLAGS += -DDEBUG -O0
debug: clean all
	@echo "Debug build complete"

install: $(TARGET)
	@echo "Installing $(TARGET) to $(PREFIX)/bin/..."
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin/
	@echo "Installation complete! Run with: tron"

uninstall:
	@echo "Removing $(TARGET) from $(PREFIX)/bin/..."
	rm -f $(PREFIX)/bin/$(TARGET)
	@echo "Uninstall complete"

clean:
	@echo "Cleaning build files..."
	rm -rf $(OBJ_DIR) $(TARGET) $(TARGET).dSYM
	@echo "Clean complete"

help:
	@echo "Tron CLI - Available commands:"
	@echo ""
	@echo "  make          - Build the game"
	@echo "  make run      - Build and run the game"
	@echo "  make clean    - Remove build files"
	@echo "  make install  - Install to system (default: /usr/local/bin)"
	@echo "  make uninstall- Uninstall from system"
	@echo "  make debug    - Build with debug flags"
	@echo ""
	@echo "Examples:"
	@echo "  make                              - basic build"
	@echo "  make install PREFIX=/usr/local    - install to /usr/local/bin"
	@echo "  sudo make install                 - system-wide install"
