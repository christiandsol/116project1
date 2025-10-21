CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = cpusim
SRC = cpusim.cpp util.cpp CPU.cpp Register.cpp Memory.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

