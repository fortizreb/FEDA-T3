CXX = g++
CXXFLAGS = -Wall -std=c++17 -I. -Iutilidades
SRC = main.cpp arbol/arbol_binario_id.cpp arbol/arbol_binario_nombre.cpp parseo.cpp utilidades/utilitarios.cpp hash/hash_abierto.cpp hash/hash_cerrado.cpp
TARGET = test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) -lstdc++fs

clean:
	rm -f $(TARGET)


#file Makefile para probar si CRLF (en mac no funca)
#dos2unix Makefile
