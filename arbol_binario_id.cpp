#include "arboles.h"
#include "usuario.h"
using namespace std;

// constructor del nodo, inserta al usuario y deja hijos en null
nodo_arbol_id::nodo_arbol_id(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}

// constructor del árbol, parte vacio
arbol_binario_id::arbol_binario_id() : raiz(nullptr) {}

// busca si existe un id en el árbol 
bool arbol_binario_id::buscar_por_id(long long id) const {
    return buscar_por_id_rec(raiz, id);
}

// funcion recursiva para buscar id, va bajando por el arbol
bool arbol_binario_id::buscar_por_id_rec(nodo_arbol_id* nodo, long long id) const {
    if (!nodo) return false; // si no hay nodo, no esta
    if (id == nodo->usuario.id) return true; // lo encontro
    if (id < nodo->usuario.id) return buscar_por_id_rec(nodo->izquierdo, id); // va izq
    return buscar_por_id_rec(nodo->derecho, id); // va der
}

// inserta un usuario al arbol 
void arbol_binario_id::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

/*
Esta función la saqué (adapté) del laboratorio que hizo Víctor sobre árboles BST.
Básicamente hace lo mismo que el ejemplo de él: si el nodo está vacío, crea uno nuevo con el usuario.
Si el id del usuario es menor, baja por la izquierda; si es mayor, baja por la derecha.
Así el árbol queda ordenado por id. Es el mismo algoritmo que mostró Víctor, solo que aquí es con usuarios
*/
nodo_arbol_id* arbol_binario_id::insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario) {
    if (!nodo) return new nodo_arbol_id(usuario); // si no hay nodo, crea uno
    if (usuario.id < nodo->usuario.id) {
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario); // va izq
    } else if (usuario.id > nodo->usuario.id) {
        nodo->derecho = insertar_rec(nodo->derecho, usuario); // va der
    }
    return nodo; // retorna el nodo (ya sea nuevo o modificado)
}
