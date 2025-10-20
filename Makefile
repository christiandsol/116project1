CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = cpusim
SRC = cpusim.cpp CPU.cpp Register.cpp util.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

