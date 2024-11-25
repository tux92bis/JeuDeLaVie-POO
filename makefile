CXX = g++
CXXFLAGS = -Wall -I../include
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRCS = $(wildcard ../src/*.cpp)
OBJS = $(SRCS:../src/%.cpp=%.o)

TARGET = JeuDeLaVie

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: ../src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
