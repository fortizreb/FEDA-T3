#ifndef HASH_CERRADO_H
#define HASH_CERRADO_H

#include <vector>
#include <optional>
#include <string>
#include "utilidades/usuario.h"

// Clase para la tabla de hash cerrado ocuparemos LINEAR PROBING para manejar colisiones
class HashCerrado {
public:
    std::vector<std::optional<Usuario>> tabla; // Cada posición puede tener un usuario o estar vacía
    size_t tamano_tabla; // Tamaño de la tabla
    HashCerrado(size_t tamano); // Constructor
    void insertar(const Usuario& usuario); // Inserta un usuario en la tabla
    bool buscar_por_id(long long id); // Busca usuario por id
    bool buscar_por_nombre(const std::string& nombre_usuario); // Busca usuario por nombre
};

#endif 