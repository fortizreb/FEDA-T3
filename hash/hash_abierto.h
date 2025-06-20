#ifndef HASH_ABIERTO_H
#define HASH_ABIERTO_H

#include <vector>
#include <list>
#include <string>
#include "utilidades/usuario.h"

// Clase para la tabla de hash abierto (usamos listas para manejar colisiones)
class HashAbierto {
public:
    std::vector<std::list<Usuario>> tabla; // Cada posición es una lista de usuarios
    size_t tamano_tabla; // Tamaño de la tabla
    HashAbierto(size_t tamano); // Constructor
    void insertar(const Usuario& usuario); // Inserta un usuario en la tabla
    bool buscar_por_id(long long id); // Busca usuario por id
    bool buscar_por_nombre(const std::string& nombre_usuario); // Busca usuario por nombre
};

#endif 
