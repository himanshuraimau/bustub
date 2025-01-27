CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TEST_FLAGS = -lgtest -lgtest_main -pthread

# Source files
SRC = hyperloglog.cpp
TEST_SRC = hyperloglog_test.cpp

# Object files
OBJ = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Targets
all: test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(TEST_OBJ) -o hyperloglog_test $(TEST_FLAGS)
	./hyperloglog_test

clean:
	rm -f *.o hyperloglog_test

.PHONY: all clean test
