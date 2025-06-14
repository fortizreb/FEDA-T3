#ifndef ARBOL_BINARIO_ID_H
#define ARBOL_BINARIO_ID_H

#include "usuario.h"
#include <string>
using namespace std;

class Nodo_Arbol_Id {
public:
    Usuario usuario;
    Nodo_Arbol_Id* izquierdo;
    Nodo_Arbol_Id* derecho;
    Nodo_Arbol_Id(const Usuario& usuario);
};

class Arbol_Binario_Id {
public:
    Nodo_Arbol_Id* raiz;
    Arbol_Binario_Id();
    void insertar(const Usuario& usuario);
    bool buscar_por_id(long long id);
private:
    Nodo_Arbol_Id* insertar_rec(Nodo_Arbol_Id* nodo, const Usuario& usuario);
    bool buscar_por_id_rec(Nodo_Arbol_Id* nodo, long long id);
};

#endif
