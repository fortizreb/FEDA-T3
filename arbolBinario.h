#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H

#include "usuario.h"
#include <string>
using namespace std;

class Nodo_Arbol {
public:
    Usuario usuario;
    Nodo_Arbol* izquierdo;
    Nodo_Arbol* derecho;
    Nodo_Arbol(const Usuario& usuario);
};

class Arbol_Binario {
public:
    Nodo_Arbol* raiz;
    Arbol_Binario();
    void insertar(const Usuario& usuario);
    bool buscar_por_id(long long id);
    bool buscar_por_nombre(const string& nombre_usuario);
private:
    Nodo_Arbol* insertar_rec(Nodo_Arbol* nodo, const Usuario& usuario);
    bool buscar_por_id_rec(Nodo_Arbol* nodo, long long id);
    bool buscar_por_nombre_rec(Nodo_Arbol* nodo, const string& nombre_usuario);
};

#endif
