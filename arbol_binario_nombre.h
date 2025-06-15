#ifndef ARBOL_BINARIO_NOMBRE_H
#define ARBOL_BINARIO_NOMBRE_H

#include "usuario.h"
#include <string>
#include <functional>
using namespace std;

class nodo_arbol_nombre {
public:
    Usuario usuario;
    nodo_arbol_nombre* izquierdo;
    nodo_arbol_nombre* derecho;
    nodo_arbol_nombre(const Usuario& usuario);
};

class arbol_binario_nombre {
public:
    nodo_arbol_nombre* raiz;
    arbol_binario_nombre();
    void insertar(const Usuario& usuario);
    bool buscar_por_nombre(const string& nombre_usuario);
    Usuario* buscar_usuario_por_nombre(const string& nombre_usuario); // Versión que retorna puntero al usuario encontrado (o nullptr)
    void imprimir_primeros_n(int n);
    // Búsqueda genérica por criterio
    Usuario* buscar_usuario(std::function<bool(const Usuario&)> criterio);
private:
    nodo_arbol_nombre* insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario);
    bool buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario);
    Usuario* buscar_usuario_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario);
    void imprimir_primeros_n_rec(nodo_arbol_nombre* nodo, int& contador, int n);
    Usuario* buscar_usuario_rec(nodo_arbol_nombre* nodo, std::function<bool(const Usuario&)> criterio);
};

#endif
