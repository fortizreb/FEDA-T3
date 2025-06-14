#include "arbolBinarioNombre.h"

Nodo_Arbol_Nombre::Nodo_Arbol_Nombre(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}

Arbol_Binario_Nombre::Arbol_Binario_Nombre() : raiz(nullptr) {}

void Arbol_Binario_Nombre::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

Nodo_Arbol_Nombre* Arbol_Binario_Nombre::insertar_rec(Nodo_Arbol_Nombre* nodo, const Usuario& usuario) {
    if (!nodo) return new Nodo_Arbol_Nombre(usuario);
    if (usuario.screen_name < nodo->usuario.screen_name)
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    else if (usuario.screen_name > nodo->usuario.screen_name)
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    return nodo;
}

bool Arbol_Binario_Nombre::buscar_por_nombre(const string& nombre_usuario) {
    return buscar_por_nombre_rec(raiz, nombre_usuario);
}

bool Arbol_Binario_Nombre::buscar_por_nombre_rec(Nodo_Arbol_Nombre* nodo, const string& nombre_usuario) {
    if (!nodo) return false;
    if (nombre_usuario == nodo->usuario.screen_name) return true;
    if (nombre_usuario < nodo->usuario.screen_name) return buscar_por_nombre_rec(nodo->izquierdo, nombre_usuario);
    return buscar_por_nombre_rec(nodo->derecho, nombre_usuario);
}
