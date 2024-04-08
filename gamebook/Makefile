# this makefile is mostly copied from https://gitlab.fit.cvut.cz/breindan/bi-pa2-tutorials-b222/-/blob/c8d38c96e981329c796b7737b6e048221820cca7/cv09/assignment/Makefile

ZIP_FILES := zadani.txt prohlaseni.txt Makefile src examples libs Doxyfile assets

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET := $(BIN_DIR)/main.o
BINARY := karlibar
DEPENDENCIES := $(OBJS:%.o=%.d)

#deault compiler
CXX := g++
#deafult flags
CXXFLAGS:= -std=c++17 -Wall -pedantic -g -I libs

ifdef __APPLE__
CXX := clang++
CXXFLAGS:= -std=c++17 -Wall -pedantic -fsanitize=address -Werror -Wextra -g -I libs
endif

ifdef TEST
CXXFLAGS:= -std=c++17 -Wall -pedantic -fsanitize=address -Werror -Wextra -g -I libs -DTEST
endif

ifdef DEBUG
CXXFLAGS:= -std=c++17 -Wall -pedantic -fsanitize=address -Werror -Wextra -g -I libs -DDEBUG
endif

ifdef __PROGTEST__
CXX := g++
CXXFLAGS:= -std=c++17 -Wall -pedantic -I libs
endif

.PHONY: all clean run zip doc test

compile: $(TARGET)
	@cp $(TARGET) $(BINARY)

all: compile doc

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) doc $(BINARY) karlibar.zip

run: compile
	@./$(BINARY)

doc: $(SRCS) Doxyfile
	doxygen

zip: clean
	@cp assets/zadani.md zadani.txt
	@mkdir -p karlibar
	@cp -r $(ZIP_FILES) karlibar
	zip -r karlibar.zip karlibar
	@rm -rf karlibar

$(OBJECT_DIR)/main.o: Makefile

-include $(DEPENDENCIES)