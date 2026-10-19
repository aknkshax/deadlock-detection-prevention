# ============================================================================
# Makefile - Deadlock Detection and Prevention System
# ============================================================================

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC_DIR   := src
TEST_DIR  := tests
BUILD_DIR := build

CORE_SRCS := $(SRC_DIR)/ResourceManager.cpp \
             $(SRC_DIR)/WaitForGraph.cpp \
             $(SRC_DIR)/CoffmanPrevention.cpp

MAIN_SRC  := $(SRC_DIR)/main.cpp
TEST_SRC  := $(TEST_DIR)/test_suite.cpp

TARGET    := deadlock_sim
TEST_BIN  := run_tests

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(MAIN_SRC) $(CORE_SRCS)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(CORE_SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TEST_BIN): $(TEST_SRC) $(CORE_SRCS)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(CORE_SRCS) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(TARGET) $(TEST_BIN)
