CXX = g++
CXXFLAGS = -Wall -std=c++17
SRC = main.cpp lecturaData.cpp
TARGET = test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
