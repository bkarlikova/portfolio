SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET := $(BIN_DIR)/main.o
BINARY := main.out
DEPENDENCIES := $(OBJS:%.o=%.d)
VAR := ""

#deault compiler
CXX := g++
#deafult flags
CXXFLAGS:= -std=c++23 -Wall -pedantic -g

ifdef __APPLE__
CXX := clang++
CXXFLAGS:= -std=c++23 -Wall -pedantic -fsanitize=address -Werror -Wextra -g
endif

ifdef PRINT
CXXFLAGS += -DPRINT
endif

.PHONY: all clean read run

compile: $(TARGET)
	@cp $(TARGET) $(BINARY)

all: compile

#linker
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

#compiler
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(BINARY)

read:
	VAR

run: compile
	./main.out $(VAR)

$(OBJECT_DIR)/main.o: Makefile

-include $(DEPENDENCIES)
