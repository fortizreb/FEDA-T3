#include "arbol/arboles.h"
#include "utilidades/utilitarios.h"
#include "utilidades/usuario.h"
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
#include "hash/hash_abierto.h"
#include "hash/hash_cerrado.h"
#include <filesystem>

using namespace std;

int LIMITE_USUARIOS = 40000;

void flujo_bst() {
    auto inicio_total_insercion = chrono::high_resolution_clock::now();
    arbol_binario_id bst_id;
    vector<resultado_insercion> grilla_id;
    arbol_binario_nombre bst_nombre;
    vector<resultado_insercion> grilla_nombre;
    double tiempo_lectura_id = 0, tiempo_insercion_id = 0;
    double tiempo_lectura_nombre = 0, tiempo_insercion_nombre = 0;
    cout << "\n********[INSERCIÓN BST]********" << endl;
    cout << "Cargando datos en BST por ID..." << endl;
    insertar_bst_id(bst_id, "data.csv", grilla_id, tiempo_lectura_id, tiempo_insercion_id);
    cout << "Cargando datos en BST por NOMBRE..." << endl;
    insertar_bst_nombre(bst_nombre, "data.csv", grilla_nombre, tiempo_lectura_nombre, tiempo_insercion_nombre);
    auto fin_total_insercion = chrono::high_resolution_clock::now();
    cout << "\nTotal inserción: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total_insercion - inicio_total_insercion).count() << " segundos]" << endl;
    exportar_resultados_insercion_csv(grilla_id, grilla_nombre);
    cout << "\n********[BÚSQUEDA BST]********" << endl;
    auto inicio_total_busqueda = chrono::high_resolution_clock::now();
    cout << "Buscando en BST por ID y por NOMBRE..." << endl;
    vector<Usuario> usuarios_validos = leer_usuarios_validos_csv("data.csv");
    busqueda_bst(bst_id, bst_nombre, usuarios_validos);
    auto fin_total_busqueda = chrono::high_resolution_clock::now();
    cout << "\nTotal búsqeuda: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total_busqueda - inicio_total_busqueda).count() << " segundos]" << endl;
}

void flujo_hash() {
    cout << "\n********[INSERCIÓN HASH ABIERTO Y CERRADO]********" << endl;
    vector<Usuario> usuarios_validos = leer_usuarios_validos_csv("data.csv");
    size_t tamano_tabla_abierto = 55001;
    HashAbierto hash_abierto(tamano_tabla_abierto);
    size_t tamano_tabla_cerrado = 80021;
    HashCerrado hash_cerrado(tamano_tabla_cerrado);
    vector<resultado_insercion> grilla_ha;
    double tiempo_ha = 0;
    insertar_hash_abierto(hash_abierto, usuarios_validos, grilla_ha, tiempo_ha);
    exportar_resultados_insercion_hash_csv(grilla_ha, "abierto");
    cout << "Tiempo de inserción Hash Abierto: " << tiempo_ha << " segundos" << endl;
    vector<resultado_insercion> grilla_hc;
    double tiempo_hc = 0;
    insertar_hash_cerrado(hash_cerrado, usuarios_validos, grilla_hc, tiempo_hc);
    exportar_resultados_insercion_hash_csv(grilla_hc, "cerrado");
    cout << "Tiempo de inserción Hash Cerrado: " << tiempo_hc << " segundos" << endl;
    cout << "\n********[BÚSQUEDA HASH ABIERTO Y CERRADO]********" << endl;
    auto inicio_total_busqueda = chrono::high_resolution_clock::now();
    busqueda_hash("abierto", usuarios_validos);
    busqueda_hash("cerrado", usuarios_validos);
    auto fin_total_busqueda = chrono::high_resolution_clock::now();
    cout << "\nTotal búsqeuda hash: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total_busqueda - inicio_total_busqueda).count() << " segundos]" << endl;
}

int main() {
    // Limpiar carpeta resultados al inicio
    std::filesystem::remove_all("resultados");
    std::filesystem::create_directory("resultados");
    // Limpiar carpeta graficos al inicio
    std::filesystem::remove_all("graficos");
    std::filesystem::create_directory("graficos");

    int opcion = 3; // 1: BST, 2: tabla hash, 3: Ambos
    if (opcion == 1) {
        flujo_bst();
    } else if (opcion == 2) {
        flujo_hash();
    } else if (opcion == 3) {
        flujo_bst();
        flujo_hash();
    }
    return 0;
}