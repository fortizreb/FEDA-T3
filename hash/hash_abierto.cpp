#include "hash_abierto.h"
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
using namespace std;

// Implementación de HashAbierto fuera de la clase
HashAbierto::HashAbierto(size_t tamano) {
    tamano_tabla = tamano;
    tabla = std::vector<std::list<Usuario>>(tamano_tabla);
}

void HashAbierto::insertar(const Usuario& usuario) {
    size_t indice = std::hash<long long>{}(usuario.id) % tamano_tabla;
    tabla[indice].push_back(usuario);
}

bool HashAbierto::buscar_por_id(long long id) {
    size_t indice = std::hash<long long>{}(id) % tamano_tabla;
    for (const auto& usuario : tabla[indice]) {
        if (usuario.id == id) return true;
    }
    return false;
}

bool HashAbierto::buscar_por_nombre(const std::string& nombre_usuario) {
    size_t indice = std::hash<std::string>{}(nombre_usuario) % tamano_tabla;
    for (const auto& usuario : tabla[indice]) {
        if (usuario.screen_name == nombre_usuario) return true;
    }
    return false;
}
