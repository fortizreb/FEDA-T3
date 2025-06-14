// Árbol binario de búsqueda sin balanceo
// Permite insertar, buscar y eliminar usuarios por clave

#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H

#include "usuario.h"
#include <string>
using namespace std;

class NodoArbol {
public:
    Usuario usuario;
    NodoArbol* izquierdo;
    NodoArbol* derecho;
    NodoArbol(const Usuario& usuario);
};

class ArbolBinario {
public:
    NodoArbol* raiz;
    ArbolBinario();
    void insertar(const Usuario& usuario);
    bool buscarPorId(long long id);
    bool buscarPorNombre(const string& nombreUsuario);
    // Puedes agregar métodos para eliminar y recorrer
};

#endif // ARBOL_BINARIO_H
