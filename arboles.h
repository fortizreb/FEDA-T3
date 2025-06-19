#ifndef ARBOLES_H
#define ARBOLES_H

#include "usuario.h"
#include <string>
#include <vector>
#include <functional>
using namespace std;

// Nodo para el arbol por ID, guarda un usuario y sus hijos
class nodo_arbol_id {
public:
    Usuario usuario; // el usuario que se guarda aqui
    nodo_arbol_id* izquierdo; // hijo izq
    nodo_arbol_id* derecho;   // hijo der
    nodo_arbol_id(const Usuario& usuario); // constructor, mete el user
};

// Arbol binario de busqueda por ID
class arbol_binario_id {
public:
    nodo_arbol_id* raiz; // la raiz del arbol
    arbol_binario_id(); // constructor, deja raiz en null
    void insertar(const Usuario& usuario); // mete un user
    bool buscar_por_id(long long id) const; // busca por id
    Usuario* buscar_usuario_por_id(long long id); // busca y retorna puntero
    Usuario* buscar_usuario(function<bool(const Usuario&)> criterio); // busca con criterio raro
private:
    nodo_arbol_id* insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario); // inserta recursivo
    bool buscar_por_id_rec(nodo_arbol_id* nodo, long long id) const; // busca recursivo
    Usuario* buscar_usuario_por_id_rec(nodo_arbol_id* nodo, long long id); // busca recursivo
    Usuario* buscar_usuario_rec(nodo_arbol_id* nodo, function<bool(const Usuario&)> criterio); // busca recursivo con criterio
};

// Nodo para el arbol por nombre, igual que el de id pero por nombre
class nodo_arbol_nombre {
public:
    Usuario usuario; // el user
    nodo_arbol_nombre* izquierdo; // hijo izq
    nodo_arbol_nombre* derecho;   // hijo der
    nodo_arbol_nombre(const Usuario& usuario); // constructor
};

// Arbol binario de busqueda por nombre
class arbol_binario_nombre {
public:
    nodo_arbol_nombre* raiz; // la raiz
    arbol_binario_nombre(); // constructor
    void insertar(const Usuario& usuario); // mete user
    bool buscar_por_nombre(const string& nombre_usuario) const; // busca por nombre
    Usuario* buscar_usuario_por_nombre(const string& nombre_usuario); // busca y retorna puntero
    Usuario* buscar_usuario(function<bool(const Usuario&)> criterio); // busca con criterio
private:
    nodo_arbol_nombre* insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario); // inserta recursivo
    bool buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) const; // busca recursivo
    Usuario* buscar_usuario_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario); // busca recursivo
    Usuario* buscar_usuario_rec(nodo_arbol_nombre* nodo, function<bool(const Usuario&)> criterio); // busca recursivo con criterio
};

#endif
