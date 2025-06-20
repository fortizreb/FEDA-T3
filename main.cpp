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

int LIMITE_USUARIOS = 10000;

int main() {
    // Limpiar carpeta resultados al inicio
    std::filesystem::remove_all("resultados");
    std::filesystem::create_directory("resultados");

    int opcion = 2; 
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
        
        // Instanciar tablas hash con tamaños ajustados según tipo
        // Para tabla hash abierta (lista) usamos un tamaño primo mayor a 55.000 para un factor de carga cercano a 0.73
        size_t tamano_tabla_abierto = 55001;
        HashAbierto hash_abierto(tamano_tabla_abierto);

        // Para tabla hash cerrada (linear probing): usamos un tamaño primo mayor a 80.000 para mantener un factor de carga cercano a 0.5
        size_t tamano_tabla_cerrado = 80021;
        HashCerrado hash_cerrado(tamano_tabla_cerrado);

        // Hash Abierto
        vector<resultado_insercion> grilla_ha;
        double tiempo_ha = 0;
        insertar_hash_abierto(hash_abierto, usuarios_validos, grilla_ha, tiempo_ha);
        exportar_resultados_insercion_hash_csv(grilla_ha, "abierto");
        cout << "Tiempo de inserción Hash Abierto: " << tiempo_ha << " segundos" << endl;

        // Hash Cerrado
        vector<resultado_insercion> grilla_hc;
        double tiempo_hc = 0;
        insertar_hash_cerrado(hash_cerrado, usuarios_validos, grilla_hc, tiempo_hc);
        exportar_resultados_insercion_hash_csv(grilla_hc, "cerrado");
        cout << "Tiempo de inserción Hash Cerrado: " << tiempo_hc << " segundos" << endl;
        
        
        // Búsqueda en Hash Abierto y Cerrado
        cout << "\n********[INSERCIÓN HASH ABIERTO Y CERRADO]********" << endl;
        long long id_buscar = usuarios_validos[0].id;
        string nombre_buscar = usuarios_validos[0].screen_name;
        cout << "Buscar id en Hash Abierto: " << (buscar_hash_abierto_id(hash_abierto, id_buscar) ? "ENCONTRADO" : "NO ENCONTRADO") << endl;
        cout << "Buscar id en Hash Cerrado: " << (buscar_hash_cerrado_id(hash_cerrado, id_buscar) ? "ENCONTRADO" : "NO ENCONTRADO") << endl;
        cout << "Buscar nombre en Hash Abierto: " << (buscar_hash_abierto_nombre(hash_abierto, nombre_buscar) ? "ENCONTRADO" : "NO ENCONTRADO") << endl;
        cout << "Buscar nombre en Hash Cerrado: " << (buscar_hash_cerrado_nombre(hash_cerrado, nombre_buscar) ? "ENCONTRADO" : "NO ENCONTRADO") << endl;
    }
    return 0;
}