#ifndef HASH_ABIERTO_H
#define HASH_ABIERTO_H

#include <list>
#include <vector>
using namespace std;

#include "usuario.h"

class HashAbierto {
public:
    vector<list<Usuario>> tabla;
    size_t tamanoTabla;
    HashAbierto(size_t tamano);
    void insertar(const Usuario& usuario);
    bool buscarPorId(long long id);
    bool buscarPorNombre(const string& nombreUsuario);
};

#endif // HASH_ABIERTO_H
