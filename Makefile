TARGET = Solver
CXXFLAGS=-g -O3 -std=c++17

CURRENT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR = $(CURRENT_DIR)/build
DATA_DIR = $(CURRENT_DIR)/data
SRC_DIRS = $(CURRENT_DIR)/src
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

all: $(TARGET)
	@echo Current Dir: $(CURRENT_DIR)
	$(MKDIR_P) $(DATA_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(DATA_DIR)
	$(RM) -r $(TARGET)
MKDIR_P ?= mkdir -p


