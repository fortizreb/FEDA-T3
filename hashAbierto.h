// Tabla hash con hashing abierto (listas enlazadas)
// Permite insertar y buscar usuarios por clave

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
    // Puedes agregar métodos para eliminar
};

#endif // HASH_ABIERTO_H
