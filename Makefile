CXX = g++
CXXFLAGS = -Iextern/googletest/googletest/include -Iinclude -std=c++17 -pthread

GTEST_LIB = -Lextern/googletest/build/lib -lgtest -lgtest_main

TARGET_DIR = bin
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.0)
TEST_SRC = $(wildcard tests/*.cpp)
TARGET = $(TARGET_DIR)/ducksearch
TEST_TARGET= $(TARGET_DIR)/ducksearch_tests

GTEST_BUILD_DIR = extern/googletest/build
GTEST_MARKER = $(GTEST_BUILD_DIR)/.built

all: $(GTEST_MARKER) $(TARGET_DIR) $(TARGET)

$(TARGET_DIR):
	mkdir -p $@

$(GTEST_MARKER):
	@echo "Building Google Test"
	mkdir -p $(GTEST_BUILD_DIR)
	cd $(GTEST_BUILD_DIR) && cmake .. && make
	touch $@


$(TARGET): $(OBJ)
	$(CXX) -o $@ $^


test: $(GTEST_MARKER) $(TARGET_DIR) $(TEST_TARGET)
	./$(TEST_TARGET)


$(TEST_TARGET): $(filter-out src/main.cpp, $(SRC)) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIB)

%.0: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET)
	rm -rf $(GTEST_BUILD_DIR)