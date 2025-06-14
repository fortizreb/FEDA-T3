#ifndef ARBOL_BINARIO_NOMBRE_H
#define ARBOL_BINARIO_NOMBRE_H

#include "usuario.h"
#include <string>
using namespace std;

class Nodo_Arbol_Nombre {
public:
    Usuario usuario;
    Nodo_Arbol_Nombre* izquierdo;
    Nodo_Arbol_Nombre* derecho;
    Nodo_Arbol_Nombre(const Usuario& usuario);
};

class Arbol_Binario_Nombre {
public:
    Nodo_Arbol_Nombre* raiz;
    Arbol_Binario_Nombre();
    void insertar(const Usuario& usuario);
    bool buscar_por_nombre(const string& nombre_usuario);
private:
    Nodo_Arbol_Nombre* insertar_rec(Nodo_Arbol_Nombre* nodo, const Usuario& usuario);
    bool buscar_por_nombre_rec(Nodo_Arbol_Nombre* nodo, const string& nombre_usuario);
};

#endif
