#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H

#include "usuario.h"

class Arbol_Binario {
public:
    Arbol_Binario();
    virtual void insertar(const Usuario& usuario);
    virtual bool buscar(long long id);
protected:
    // Puedes definir aquí un nodo base si lo necesitas
    // struct Nodo { ... };
    void* raiz;
};

#endif // ARBOL_BINARIO_H
