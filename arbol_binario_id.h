#ifndef ARBOL_BINARIO_ID_H
#define ARBOL_BINARIO_ID_H

#include "usuario.h"
#include <string>
#include <functional>
using namespace std;

class nodo_arbol_id {
public:
    Usuario usuario;
    nodo_arbol_id* izquierdo;
    nodo_arbol_id* derecho;
    nodo_arbol_id(const Usuario& usuario);
};

class arbol_binario_id {
public:
    nodo_arbol_id* raiz;
    arbol_binario_id();
    void insertar(const Usuario& usuario);
    bool buscar_por_id(long long id);
    Usuario* buscar_usuario_por_id(long long id); // Versión que retorna puntero al usuario encontrado (o nullptr)
    void imprimir_primeros_n(int n); //Solo para pruebas

    // Búsqueda genérica por criterio (bool)
    Usuario* buscar_usuario(std::function<bool(const Usuario&)> criterio);

    
private:
    nodo_arbol_id* insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario);
    bool buscar_por_id_rec(nodo_arbol_id* nodo, long long id);
    Usuario* buscar_usuario_por_id_rec(nodo_arbol_id* nodo, long long id);
    void imprimir_primeros_n_rec(nodo_arbol_id* nodo, int& contador, int n); //Solo para pruebas
    Usuario* buscar_usuario_rec(nodo_arbol_id* nodo, std::function<bool(const Usuario&)> criterio);
};

#endif
