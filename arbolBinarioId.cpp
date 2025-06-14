#include "arbolBinarioId.h"

Nodo_Arbol_Id::Nodo_Arbol_Id(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}

Arbol_Binario_Id::Arbol_Binario_Id() : raiz(nullptr) {}

void Arbol_Binario_Id::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

Nodo_Arbol_Id* Arbol_Binario_Id::insertar_rec(Nodo_Arbol_Id* nodo, const Usuario& usuario) {
    if (!nodo) return new Nodo_Arbol_Id(usuario);
    if (usuario.id < nodo->usuario.id)
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    else if (usuario.id > nodo->usuario.id)
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    return nodo;
}

bool Arbol_Binario_Id::buscar_por_id(long long id) {
    return buscar_por_id_rec(raiz, id);
}

bool Arbol_Binario_Id::buscar_por_id_rec(Nodo_Arbol_Id* nodo, long long id) {
    if (!nodo) return false;
    if (id == nodo->usuario.id) return true;
    if (id < nodo->usuario.id) return buscar_por_id_rec(nodo->izquierdo, id);
    return buscar_por_id_rec(nodo->derecho, id);
}
