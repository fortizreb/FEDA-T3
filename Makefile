CXX = g++
CXXFLAGS = -Wall -std=c++17
SRC = main.cpp lectura_data.cpp arbol_binario_id.cpp arbol_binario_nombre.cpp
TARGET = test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)


#file Makefile para probar si CRLF (en mac no funca)
#dos2unix Makefile
