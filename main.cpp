#include "arboles.h"
#include "utilitarios.h"
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
#include "usuario.h"
#include "hash_abierto.h"
#include "hash_cerrado.h"
#include <filesystem>

using namespace std;

int LIMITE_USUARIOS = 40000;

int main() {
    // Limpiar carpeta resultados al inicio
    std::filesystem::remove_all("resultados");
    std::filesystem::create_directory("resultados");

    int opcion = 1; 
    if( opcion ==1){

        
        auto inicio_total_insercion = chrono::high_resolution_clock::now();
        arbol_binario_id bst_id;
        vector<resultado_insercion> grilla_id;
        arbol_binario_nombre bst_nombre;
        vector<resultado_insercion> grilla_nombre;
        double tiempo_lectura_id = 0, tiempo_insercion_id = 0;
        double tiempo_lectura_nombre = 0, tiempo_insercion_nombre = 0;


        /******** INSERCIÓN EN BST POR NOMBRE E ID **********/  
        cout << "\n********[INSERCIÓN BST]********" << endl;
        cout << "Cargando datos en BST por ID..." << endl;
        insertar_bst_id(bst_id, "data.csv", grilla_id, tiempo_lectura_id, tiempo_insercion_id);
        cout << "Cargando datos en BST por NOMBRE..." << endl;
        insertar_bst_nombre(bst_nombre, "data.csv", grilla_nombre, tiempo_lectura_nombre, tiempo_insercion_nombre);
        auto fin_total_insercion = chrono::high_resolution_clock::now();
        cout << "\nTotal inserción: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total_insercion - inicio_total_insercion).count() << " segundos]" << endl;
        exportar_resultados_insercion_csv(grilla_id, grilla_nombre); //guardamos resultados en archivo.

        /******** BÚSQUEDA EN BST POR NOMBRE E ID **********/
        cout << "\n********[BÚSQUEDA BST]********" << endl;
        auto inicio_total_busqueda = chrono::high_resolution_clock::now();
        cout << "Buscando en BST por ID y por NOMBRE..." << endl;
        vector<Usuario> usuarios_validos = leer_usuarios_validos_csv("data.csv"); 
        busqueda_bst(bst_id, bst_nombre, usuarios_validos);
        auto fin_total_busqueda = chrono::high_resolution_clock::now();
        cout << "\nTotal búsqeuda: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total_busqueda - inicio_total_busqueda).count() << " segundos]" << endl;

    } else if (opcion == 2) {
        // INSERCIÓN Y MEDICIÓN EN HASH ABIERTO Y CERRADO
        cout << "\n********[INSERCIÓN HASH ABIERTO Y CERRADO]********" << endl;
        vector<Usuario> usuarios_validos = leer_usuarios_validos_csv("data.csv");
        // Instanciar tablas hash
        size_t tamano_tabla = usuarios_validos.size() * 2;
        HashAbierto hash_abierto(tamano_tabla);
        HashCerrado hash_cerrado(tamano_tabla);
        // Hash Abierto
        vector<resultado_insercion> grilla_ha;
        auto inicio_ha = chrono::high_resolution_clock::now();
        for (const auto& usuario : usuarios_validos) {
            hash_abierto.insertar(usuario);
        }
        auto fin_ha = chrono::high_resolution_clock::now();
        double tiempo_ha = chrono::duration<double>(fin_ha - inicio_ha).count();
        grilla_ha.push_back({"HashAbierto", "insercion", "id", (int)usuarios_validos.size(), tiempo_ha, (long long)(tiempo_ha*1000), (long long)(tiempo_ha*1000000), (long long)(tiempo_ha*1000000000)});
        exportar_resultados_csv(grilla_ha, "hash", "insercion", "abierto");
        cout << "Tiempo de inserción Hash Abierto: " << tiempo_ha << " segundos" << endl;
        // Hash Cerrado
        vector<resultado_insercion> grilla_hc;
        auto inicio_hc = chrono::high_resolution_clock::now();
        for (const auto& usuario : usuarios_validos) {
            hash_cerrado.insertar(usuario);
        }
        auto fin_hc = chrono::high_resolution_clock::now();
        double tiempo_hc = chrono::duration<double>(fin_hc - inicio_hc).count();
        grilla_hc.push_back({"HashCerrado", "insercion", "id", (int)usuarios_validos.size(), tiempo_hc, (long long)(tiempo_hc*1000), (long long)(tiempo_hc*1000000), (long long)(tiempo_hc*1000000000)});
        exportar_resultados_csv(grilla_hc, "hash", "insercion", "cerrado");
        cout << "Tiempo de inserción Hash Cerrado: " << tiempo_hc << " segundos" << endl;
    }
    return 0;
}