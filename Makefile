CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Iinclude

TARGET = multi_knapsack
SRCS = src/main.cpp src/instance.cpp src/solution.cpp src/sa.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean
