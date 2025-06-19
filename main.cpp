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

using namespace std;

int LIMITE_USUARIOS = 10000;

int main() {
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

        
    return 0;
}