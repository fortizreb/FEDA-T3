#include "arbol_binario_id.h"
#include <iostream>

nodo_arbol_id::nodo_arbol_id(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}

arbol_binario_id::arbol_binario_id() : raiz(nullptr) {}

void arbol_binario_id::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

nodo_arbol_id* arbol_binario_id::insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario) {
    if (!nodo) return new nodo_arbol_id(usuario);
    if (usuario.id < nodo->usuario.id)
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    else if (usuario.id > nodo->usuario.id)
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    return nodo;
}

bool arbol_binario_id::buscar_por_id(long long id) {
    return buscar_por_id_rec(raiz, id);
}

bool arbol_binario_id::buscar_por_id_rec(nodo_arbol_id* nodo, long long id) {
    if (!nodo) return false;
    if (id == nodo->usuario.id) return true;
    if (id < nodo->usuario.id) return buscar_por_id_rec(nodo->izquierdo, id);
    return buscar_por_id_rec(nodo->derecho, id);
}

Usuario* arbol_binario_id::buscar_usuario_por_id(long long id) {
    return buscar_usuario_por_id_rec(raiz, id);
}

Usuario* arbol_binario_id::buscar_usuario_por_id_rec(nodo_arbol_id* nodo, long long id) {
    if (!nodo) return nullptr;
    if (id == nodo->usuario.id) return &nodo->usuario;
    if (id < nodo->usuario.id) return buscar_usuario_por_id_rec(nodo->izquierdo, id);
    return buscar_usuario_por_id_rec(nodo->derecho, id);
}

Usuario* arbol_binario_id::buscar_usuario(std::function<bool(const Usuario&)> criterio) {
    return buscar_usuario_rec(raiz, criterio);
}

Usuario* arbol_binario_id::buscar_usuario_rec(nodo_arbol_id* nodo, std::function<bool(const Usuario&)> criterio) {
    if (!nodo) return nullptr;
    if (criterio(nodo->usuario)) return &nodo->usuario;
    Usuario* encontrado = buscar_usuario_rec(nodo->izquierdo, criterio);
    if (encontrado) return encontrado;
    return buscar_usuario_rec(nodo->derecho, criterio);
}

void arbol_binario_id::imprimir_primeros_n(int n) {
    int contador = 0;
    imprimir_primeros_n_rec(raiz, contador, n);
}

void arbol_binario_id::imprimir_primeros_n_rec(nodo_arbol_id* nodo, int& contador, int n) {
    if (!nodo || contador >= n) return;
    imprimir_primeros_n_rec(nodo->izquierdo, contador, n);
    if (contador < n) {
        std::cout << nodo->usuario.id << " | " << nodo->usuario.screen_name << std::endl;
        contador++;
    }
    imprimir_primeros_n_rec(nodo->derecho, contador, n);
}
