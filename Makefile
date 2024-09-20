CXX = g++
CXXFLAGS = -Iextern/googletest/googletest/include -Iinclude -std=c++17 -pthread

GTEST_LIB = -Lextern/googletest/lib -lgtest -lgtest_main

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.0)
TARGET = ducksearch
TEST_SRC = $(wildcard tests/*.cpp)
TEST_TARGET=ducksearch_tests
DIST_DIR = dist

GTEST_BUILD_DIR = extern/googletest
GTEST_MARKER = $(GTEST_BUILD_DIR)/.built

all: $(GTEST_MARKER) $(TARGET)

$(GTEST_MARKER):
	@echo "Building Google Test"
	cd $(GTEST_BUILD_DIR) && cmake . && make
	touch $@


$(TARGET): $(OBJ)
	$(CXX) -o $@ $^


test: $(GTEST_MARKER) $(TEST_TARGET)
	./$(TEST_TARGET)


$(TEST_TARGET): $(filter-out src/main.cpp, $(SRC)) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIB)

%.0: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET) $(TEST_TARGET)
	rm -f $(GTEST_BUILD_DIR)/.built