#ifndef HASH_CERRADO_H
#define HASH_CERRADO_H

#include <optional>
#include <vector>
using namespace std;

#include "usuario.h"

class HashCerrado {
public:
    vector<optional<Usuario>> tabla;
    size_t tamanoTabla;
    HashCerrado(size_t tamano);
    void insertar(const Usuario& usuario);
    bool buscarPorId(long long id);
    bool buscarPorNombre(const string& nombreUsuario);
    
};

#endif 
