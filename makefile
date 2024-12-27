CXX = g++
CXXFLAGS = -std=c++17 -O3 -I./include -I/usr/local/Cellar/eigen/3.4.0_1/include/eigen3/Eigen
LDFLAGS = -lSDL2

TARGET = bin/rubikCube

SRCS = $(wildcard src/*.cpp)
HEADERS = $(wildcard ../include/*.h)
OBJS = $(SRCS:src/%.cpp=bin/obj/%.o)

run: $(TARGET)
	./$(TARGET)

rebuild: clean run

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

bin/obj/%.o: src/%.cpp $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin:
	mkdir -p bin
	mkdir -p bin/obj

clean:
	rm -rf bin