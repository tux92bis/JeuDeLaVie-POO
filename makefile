CXX = g++
CXXFLAGS = -I include
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

TARGET = bin/JeuDeLaVie

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $^ -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj bin

.PHONY: all clean
