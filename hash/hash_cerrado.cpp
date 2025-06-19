#include "hash_cerrado.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <iomanip>
#include <optional>
#include <list>
#include <sstream>
#include <algorithm>

// Implementación de HashCerrado fuera de la clase
HashCerrado::HashCerrado(size_t tamano) {
    tamano_tabla = tamano;
    tabla = std::vector<std::optional<Usuario>>(tamano_tabla);
}

void HashCerrado::insertar(const Usuario& usuario) {
    size_t indice = std::hash<long long>{}(usuario.id) % tamano_tabla;
    while (tabla[indice].has_value()) {
        indice = (indice + 1) % tamano_tabla;
    }
    tabla[indice] = usuario;
}

bool HashCerrado::buscar_por_id(long long id) {
    size_t indice = std::hash<long long>{}(id) % tamano_tabla;
    size_t inicio = indice;
    while (tabla[indice].has_value()) {
        if (tabla[indice]->id == id) return true;
        indice = (indice + 1) % tamano_tabla;
        if (indice == inicio) break;
    }
    return false;
}

bool HashCerrado::buscar_por_nombre(const std::string& nombre_usuario) {
    size_t indice = std::hash<std::string>{}(nombre_usuario) % tamano_tabla;
    size_t inicio = indice;
    while (tabla[indice].has_value()) {
        if (tabla[indice]->screen_name == nombre_usuario) return true;
        indice = (indice + 1) % tamano_tabla;
        if (indice == inicio) break;
    }
    return false;
}
