#ifndef HASH_CERRADO_H
#define HASH_CERRADO_H

#include <vector>
#include <optional>
#include <string>
#include "utilidades/usuario.h"

class HashCerrado {
public:
    std::vector<std::optional<Usuario>> tabla;
    size_t tamano_tabla;
    HashCerrado(size_t tamano);
    void insertar(const Usuario& usuario);
    bool buscar_por_id(long long id);
    bool buscar_por_nombre(const std::string& nombre_usuario);
};

#endif // HASH_CERRADO_H
