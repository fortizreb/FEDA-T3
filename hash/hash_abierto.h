#ifndef HASH_ABIERTO_H
#define HASH_ABIERTO_H

#include <vector>
#include <list>
#include <string>
#include "utilidades/usuario.h"

class HashAbierto {
public:
    std::vector<std::list<Usuario>> tabla;
    size_t tamano_tabla;
    HashAbierto(size_t tamano);
    void insertar(const Usuario& usuario);
    bool buscar_por_id(long long id);
    bool buscar_por_nombre(const std::string& nombre_usuario);
};

#endif // HASH_ABIERTO_H
