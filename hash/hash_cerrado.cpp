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

// Constructor: crea la tabla con el tamaño indicado (ocupamos 50.0000)
HashCerrado::HashCerrado(size_t tamano) {
    tamano_tabla = tamano;
    tabla = std::vector<std::optional<Usuario>>(tamano_tabla);
}

/****** LINEAR PROBING  **********/
/* la función que ocupo es std::hash que es una función de hash estándar en C++. 
 Lo que nos permite esta función es obtener un valor hash para un tipo de dato dado,
 en este caso, el id del usuario. Luego, usamos el operador módulo (%) para asegurarnos
 de que el índice esté dentro del rango de la tabla.
*/
void HashCerrado::insertar(const Usuario& usuario) {
    // Calcula el índice base usando el hash del id
    size_t indice = std::hash<long long>{}(usuario.id) % tamano_tabla;
    // Mientras la posición esté ocupada, avanza al siguiente índice (linear probing)
    while (tabla[indice].has_value()) {
        indice = (indice + 1) % tamano_tabla; // Si llega al final, vuelve al inicio
    }
    // Cuando encuentra una posición libre, inserta el usuario
    tabla[indice] = usuario;
}



// Busca un usuario por id. Devuelve true si lo encuentra
bool HashCerrado::buscar_por_id(long long id) {
    size_t indice = std::hash<long long>{}(id) % tamano_tabla;
    size_t inicio = indice;
    while (tabla[indice].has_value()) {
        if (tabla[indice]->id == id) return true;
        indice = (indice + 1) % tamano_tabla;
        if (indice == inicio) break; // Si da la vuelta, termina
    }
    return false;
}

// Busca un usuario por nombre. Devuelve true si lo encuentra
bool HashCerrado::buscar_por_nombre(const std::string& nombre_usuario) {
    size_t indice = std::hash<std::string>{}(nombre_usuario) % tamano_tabla;
    size_t inicio = indice;
    while (tabla[indice].has_value()) {
        if (tabla[indice]->screen_name == nombre_usuario) return true;
        indice = (indice + 1) % tamano_tabla;
        if (indice == inicio) break; // Si da la vuelta, termina
    }
    return false;
}
