#include <iostream>
#include "arbol_binario_nombre.h"

nodo_arbol_nombre::nodo_arbol_nombre(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}

arbol_binario_nombre::arbol_binario_nombre() : raiz(nullptr) {}

void arbol_binario_nombre::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

nodo_arbol_nombre* arbol_binario_nombre::insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario) {
    if (!nodo) return new nodo_arbol_nombre(usuario);
    if (usuario.screen_name < nodo->usuario.screen_name)
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    else if (usuario.screen_name > nodo->usuario.screen_name)
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    return nodo;
}

bool arbol_binario_nombre::buscar_por_nombre(const string& nombre_usuario) {
    return buscar_por_nombre_rec(raiz, nombre_usuario);
}

bool arbol_binario_nombre::buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) {
    if (!nodo) return false;
    if (nombre_usuario == nodo->usuario.screen_name) return true;
    if (nombre_usuario < nodo->usuario.screen_name) return buscar_por_nombre_rec(nodo->izquierdo, nombre_usuario);
    return buscar_por_nombre_rec(nodo->derecho, nombre_usuario);
}

Usuario* arbol_binario_nombre::buscar_usuario_por_nombre(const string& nombre_usuario) {
    return buscar_usuario_por_nombre_rec(raiz, nombre_usuario);
}

Usuario* arbol_binario_nombre::buscar_usuario_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) {
    if (!nodo) return nullptr;
    if (nombre_usuario == nodo->usuario.screen_name) return &nodo->usuario;
    if (nombre_usuario < nodo->usuario.screen_name) return buscar_usuario_por_nombre_rec(nodo->izquierdo, nombre_usuario);
    return buscar_usuario_por_nombre_rec(nodo->derecho, nombre_usuario);
}

Usuario* arbol_binario_nombre::buscar_usuario(std::function<bool(const Usuario&)> criterio) {
    return buscar_usuario_rec(raiz, criterio);
}

Usuario* arbol_binario_nombre::buscar_usuario_rec(nodo_arbol_nombre* nodo, std::function<bool(const Usuario&)> criterio) {
    if (!nodo) return nullptr;
    if (criterio(nodo->usuario)) return &nodo->usuario;
    Usuario* encontrado = buscar_usuario_rec(nodo->izquierdo, criterio);
    if (encontrado) return encontrado;
    return buscar_usuario_rec(nodo->derecho, criterio);
}

void arbol_binario_nombre::imprimir_primeros_n(int n) {
    int contador = 0;
    imprimir_primeros_n_rec(raiz, contador, n);
}

void arbol_binario_nombre::imprimir_primeros_n_rec(nodo_arbol_nombre* nodo, int& contador, int n) {
    if (!nodo || contador >= n) return;
    imprimir_primeros_n_rec(nodo->izquierdo, contador, n);
    if (contador < n) {
        std::cout << nodo->usuario.id << " | " << nodo->usuario.screen_name << std::endl;
        contador++;
    }
    imprimir_primeros_n_rec(nodo->derecho, contador, n);
}
