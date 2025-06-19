#include "arboles.h"
#include "usuario.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <iomanip>
#include <optional>
#include <list>
#include "utilitarios.h"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>
using namespace std;

extern int LIMITE_USUARIOS; //variable global para el límite de usuarios

/*
Exporta los resultados de inserción a un archivo CSV.
Crea el archivo con la fecha y hora, y guarda los tiempos de inserción para cada cantidad de usuarios.
*/
void exportar_resultados_insercion_csv(const vector<resultado_insercion>& grilla_id, const vector<resultado_insercion>& grilla_nombre) {
    time_t ahora = time(nullptr);
    tm* local = localtime(&ahora);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "insert_bst_%H-%M-%S_(%d-%b).csv", local);
    string ruta = "resultados/" + string(buffer);
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo: " << ruta << endl;
        return;
    }
    archivo << "n_usuarios,clave,tiempo_ms\n";
    for (const auto& r : grilla_id) {
        if (r.cantidad_nodos == 0) continue;
        archivo << r.cantidad_nodos << ",id," << fixed << setprecision(1) << (double)r.milisegundos << "\n";
    }
    for (const auto& r : grilla_nombre) {
        if (r.cantidad_nodos == 0) continue;
        archivo << r.cantidad_nodos << ",screenName," << fixed << setprecision(1) << (double)r.milisegundos << "\n";
    }
    archivo.close();
    cout << "Resultados exportados a '" << ruta << "'" << endl;
}

/*
Exporta los resultados de búsqueda a un archivo CSV.
Guarda los tiempos de búsqueda para cada tipo (exitosa/fallida) y clave (id o nombre).
*/
void exportar_resultados_busqueda_csv(const vector<resultado_busqueda>& resultados) {
    time_t ahora = time(nullptr);
    tm* local = localtime(&ahora);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "busqueda_bst_%H-%M-%S_(%d-%b).csv", local);
    string ruta = "resultados/" + string(buffer);
    ofstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo: " << ruta << endl;
        return;
    }
    archivo << "n_usuarios,clave,tipo,tiempo_ns\n";
    for (const auto& r : resultados) {
        archivo << r.cantidad_usuarios << "," << r.clave << "," << r.tipo_busqueda << "," << fixed << setprecision(1) << r.tiempo_ns << "\n";
    }
    archivo.close();
    cout << "Resultados de búsqueda exportados a '" << ruta << "'" << endl;
}

/*
Hace pruebas de búsqueda en los dos árboles (por id y por nombre),
con distintos tamaños y tanto búsquedas exitosas como fallidas.
Al final exporta los resultados a CSV.
*/
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const vector<Usuario>& usuarios_validos) {
    vector<int> tamanos = {5000, 10000, 20000, 40000}; // Distintos tamaños de prueba
    vector<resultado_busqueda> resultados;
    random_device rd;
    mt19937 gen(rd()); // Generador de números aleatorios
    int N_BUSQUEDAS = 100; // Cuántas búsquedas hacer por tamaño
    for (int n : tamanos) {
        if (static_cast<size_t>(usuarios_validos.size()) < static_cast<size_t>(n)) break; // Si no hay suficientes usuarios, corta
        
        /*BUSQUEDA POR IDs*/
        uniform_int_distribution<> dis(0, n-1); // Para elegir usuarios al azar
        vector<long long> ids;
        for (int i = 0; i < N_BUSQUEDAS; ++i) ids.push_back(usuarios_validos[dis(gen)].id); // Elige ids al azar
        volatile bool bandera_resultado = false;
        auto inicio = chrono::high_resolution_clock::now(); // Marca el tiempo antes de buscar
        for (auto id : ids) bandera_resultado |= bst_id.buscar_por_id(id); // Busca cada id en el árbol por id
        auto fin = chrono::high_resolution_clock::now(); 
        auto duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio); 
        double tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS; 
        resultados.push_back({n, "id", "exitosa", tiempo_ns}); // Guarda el resultado de búsquedas exitosas
        // BUSQUEDA FALLIDA FORZADA:
        bandera_resultado = false;
        inicio = chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) bandera_resultado |= bst_id.buscar_por_id(-1 - i); // Busca ids que no existen
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "id", "fallida", tiempo_ns}); // guardamos el resultado de búsquedas fallidas

        /*BUSQUEDA POR NOMBRES*/
        vector<string> nombres;
        for (int i = 0; i < N_BUSQUEDAS; ++i) nombres.push_back(usuarios_validos[dis(gen)].screen_name); // elige nombres al azar
        bandera_resultado = false;
        inicio = chrono::high_resolution_clock::now();
        for (auto& nombre : nombres) bandera_resultado |= bst_nombre.buscar_por_nombre(nombre); // Busca cada nombre en el árbol por nombre
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "exitosa", tiempo_ns}); // Guarda el resultado de búsquedas exitosas por nombre
        // BUSQUEDA FALLIDA FORZADA:
        bandera_resultado = false;
        inicio = chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) bandera_resultado |= bst_nombre.buscar_por_nombre("inexistente_user_" + to_string(i)); // Busca nombres que no existen
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "fallida", tiempo_ns}); // Guarda el resultado de búsquedas fallidas por nombre
    }
    // Al final exporta todo a CSV
    exportar_resultados_busqueda_csv(resultados);
}

/*
Lee usuarios desde un archivo CSV y los inserta en el árbol por ID.
Mide el tiempo de lectura e inserción, y guarda los resultados en la grilla.
*/
void insertar_bst_id(arbol_binario_id& bst, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion) {
    auto inicio_lectura = chrono::high_resolution_clock::now(); 
    ifstream archivo_csv(archivo);
    if (!archivo_csv.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo << endl; 
        tiempo_lectura = 0;
        tiempo_insercion = 0;
        return;
    }
    string tmp_linea, linea_csv;
    getline(archivo_csv, tmp_linea); 
    int lineas_leidas = 0;
    vector<Usuario> usuarios_validos;

    while (getline(archivo_csv, linea_csv) && lineas_leidas < LIMITE_USUARIOS) {
        vector<string> campos;
        parsear_campos_csv(linea_csv, campos, (size_t)10); 
        if (campos.size() < 10) continue; 
        Usuario usuario = parsear_usuario(campos); 
        if (!usuario.screen_name.empty()) usuarios_validos.push_back(usuario); // Solo agrega si tiene nombre
        lineas_leidas++;
    }
    auto fin_lectura = chrono::high_resolution_clock::now(); 
    tiempo_lectura = chrono::duration<double>(fin_lectura - inicio_lectura).count(); // Calcula tiempo de lectura
    bst = arbol_binario_id(); // Deja el árbol vacío antes de insertar
    auto inicio_insercion = chrono::high_resolution_clock::now(); 
    int idx = 0;
    int max_registros = usuarios_validos.size();
    // Inserta usuarios en bloques de 5000 y guarda el tiempo
    for (int n = 0; n <= max_registros; n += 5000) {
        if (n == 0) {
            auto fin = chrono::high_resolution_clock::now();
            auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin - inicio_insercion);
            grilla.push_back({"BST", "inserción", "id", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
            continue;
        }
        for (; idx < n && idx < max_registros; ++idx) {
            bst.insertar(usuarios_validos[idx]); // Inserta usuario en el árbol
        }
        auto fin = chrono::high_resolution_clock::now();
        auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin - inicio_insercion);
        grilla.push_back({"BST", "inserción", "id", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
    }
    // Inserta los que faltan si no es múltiplo de 5000
    if (idx < max_registros) {
        for (; idx < max_registros; ++idx) {
            bst.insertar(usuarios_validos[idx]);
        }
        auto fin = chrono::high_resolution_clock::now();
        auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin - inicio_insercion);
        grilla.push_back({"BST", "inserción", "id", max_registros, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
    }
    tiempo_insercion = chrono::duration<double>(chrono::high_resolution_clock::now() - inicio_insercion).count();
}

/*
Exporta resultados de inserción a un archivo CSV.
Ahora con variante opcional para el nombre del archivo.
*/
void exportar_resultados_csv(const std::vector<resultado_insercion>& resultados, const std::string& estructura, const std::string& tipo, const std::string& variante) {
    std::filesystem::create_directories("resultados");
    time_t ahora = time(nullptr);
    tm* local = localtime(&ahora);
    std::string nombre = tipo + "_" + estructura;
    if (!variante.empty()) nombre += "_" + variante;
    char buffer[128];
    strftime(buffer, sizeof(buffer), (nombre + "_%H-%M-%S_(%d-%b).csv").c_str(), local);
    std::string ruta = "resultados/" + std::string(buffer);
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo crear el archivo: " << ruta << std::endl;
        return;
    }
    archivo << "n_usuarios,clave,tiempo_ms\n";
    for (const auto& r : resultados) {
        archivo << r.cantidad_nodos << "," << r.objetivo << "," << std::fixed << std::setprecision(1) << (double)r.milisegundos << "\n";
    }
    archivo.close();
    std::cout << "Resultados de " << tipo << " (" << estructura << (variante.empty() ? "" : ", " + variante) << ") exportados a '" << ruta << "'" << std::endl;
}
