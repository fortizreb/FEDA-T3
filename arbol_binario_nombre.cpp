#include "arboles.h"
#include "usuario.h"
#include "utilitarios.h"
using namespace std;

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

extern int LIMITE_USUARIOS;

// Funciones de inserción y búsqueda en BST:
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const vector<Usuario>& usuarios_validos); // ejecuta búsquedas en ambos árboles
vector<Usuario> leer_usuarios_validos_csv(const string& archivo); //lee usuarios válidos desde CSV
void insertar_bst_id(arbol_binario_id& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion); // inserta en BST por ID
void insertar_bst_nombre(arbol_binario_nombre& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion); // inserta en BST por nombre

// Funciones de parseo de datos 
void parsear_campos_csv(const string& linea, vector<string>& campos, size_t esperado = 10); // separa campos de una línea CSV
Usuario parsear_usuario(const vector<string>& campos); // crea usuario desde campos

/* Esta función es basada en la de insertar del ejemplo de Víctor, pero aquí usamos screen_name en vez de enteros.
 Si el nodo está vacío, crea uno nuevo. Si el nombre es menor, baja por la izquierda; si es mayor, baja por la derecha. Así el árbol queda ordenado por nombre. Es el mismo algoritmo de BST clásico, pero con strings.
*/
void arbol_binario_nombre::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

/*
Esta función es la versión recursiva de insertar.
Compara el nombre del usuario con el del nodo actual y decide si va a la izquierda o derecha.
Si el nodo es null, crea uno nuevo. Es la base del BST, pero con nombres.
*/
nodo_arbol_nombre* arbol_binario_nombre::insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario) {
    if (!nodo) return new nodo_arbol_nombre(usuario);
    if (usuario.screen_name < nodo->usuario.screen_name)
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    else if (usuario.screen_name > nodo->usuario.screen_name)
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    return nodo;
}

// Esta función busca por nombre
bool arbol_binario_nombre::buscar_por_nombre(const string& nombre_usuario) const {
    return buscar_por_nombre_rec(raiz, nombre_usuario);
}

// versión recursiva de buscar
bool arbol_binario_nombre::buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) const {
    if (!nodo) return false;
    if (nombre_usuario == nodo->usuario.screen_name) return true;
    if (nombre_usuario < nodo->usuario.screen_name) return buscar_por_nombre_rec(nodo->izquierdo, nombre_usuario);
    return buscar_por_nombre_rec(nodo->derecho, nombre_usuario);
}

// Busca y retorna puntero al usuario por nombre 
Usuario* arbol_binario_nombre::buscar_usuario_por_nombre(const string& nombre_usuario) {
    return buscar_usuario_por_nombre_rec(raiz, nombre_usuario);
}

// Busca recursivo y retorna puntero
Usuario* arbol_binario_nombre::buscar_usuario_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) {
    if (!nodo) return nullptr;
    if (nombre_usuario == nodo->usuario.screen_name) return &nodo->usuario;
    if (nombre_usuario < nodo->usuario.screen_name) return buscar_usuario_por_nombre_rec(nodo->izquierdo, nombre_usuario);
    return buscar_usuario_por_nombre_rec(nodo->derecho, nombre_usuario);
}

Usuario* arbol_binario_nombre::buscar_usuario(std::function<bool(const Usuario&)> criterio) {
    return buscar_usuario_rec(raiz, criterio);
}

Usuario* arbol_binario_nombre::buscar_usuario_rec(nodo_arbol_nombre* nodo, std::function<bool(const Usuario&)> criterio) {
    if (!nodo) return nullptr;
    if (criterio(nodo->usuario)) return &nodo->usuario;
    Usuario* encontrado = buscar_usuario_rec(nodo->izquierdo, criterio);
    if (encontrado) return encontrado;
    return buscar_usuario_rec(nodo->derecho, criterio);
}

void insertar_bst_nombre(arbol_binario_nombre& bst, const string& archivo, std::vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion) {
    auto inicio_lectura = chrono::high_resolution_clock::now();
    ifstream archivo_csv(archivo);
    if (!archivo_csv.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        tiempo_lectura = 0;
        tiempo_insercion = 0;
        return;
    }
    string tmp_linea, linea_csv;
    getline(archivo_csv, tmp_linea); // Cabecera
    int lineas_leidas = 0;
    vector<Usuario> usuarios_validos;
    while (getline(archivo_csv, linea_csv) && lineas_leidas < LIMITE_USUARIOS) {
        vector<string> campos;
        parsear_campos_csv(linea_csv, campos, 10);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (!usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
        lineas_leidas++;
    }
    auto fin_lectura = chrono::high_resolution_clock::now();
    tiempo_lectura = chrono::duration<double>(fin_lectura - inicio_lectura).count();
    bst = arbol_binario_nombre(); // Asegura árbol vacío
    auto inicio_insercion = chrono::high_resolution_clock::now();
    int idx = 0;
    int max_registros = usuarios_validos.size();
    for (int n = 0; n <= max_registros; n += 5000) {
        if (n == 0) {
            auto fin = chrono::high_resolution_clock::now();
            auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin - inicio_insercion);
            grilla.push_back({"BST", "inserción", "nombre", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
            continue;
        }
        for (; idx < n && idx < max_registros; ++idx) {
            bst.insertar(usuarios_validos[idx]);
        }
        auto fin = chrono::high_resolution_clock::now();
        auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin - inicio_insercion);
        grilla.push_back({"BST", "inserción", "nombre", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
    }
    auto fin_insercion = chrono::high_resolution_clock::now();
    tiempo_insercion = chrono::duration<double>(fin_insercion - inicio_insercion).count();
}

// Constructor del nodo, inserta el usuario y deja hijos en null
nodo_arbol_nombre::nodo_arbol_nombre(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}

// Constructor del árbol, parte vacío
arbol_binario_nombre::arbol_binario_nombre() : raiz(nullptr) {}
