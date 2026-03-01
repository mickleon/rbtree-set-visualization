CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic

TARGET := main
SRCS := main.cpp rbtree.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all debug clean

all: $(TARGET)

debug: CXXFLAGS += -g
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
