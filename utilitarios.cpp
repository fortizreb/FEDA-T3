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
#include <sstream>
#include <algorithm>
#include <random>
using namespace std;

// Definiciones necesarias para autocontenibilidad

class nodo_arbol_id {
public:
    Usuario usuario;
    nodo_arbol_id* izquierdo;
    nodo_arbol_id* derecho;
    nodo_arbol_id(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}
};

class arbol_binario_id {
public:
    nodo_arbol_id* raiz;
    arbol_binario_id() : raiz(nullptr) {}
    void insertar(const Usuario& usuario);
    bool buscar_por_id(long long id) const;
private:
    nodo_arbol_id* insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario);
    bool buscar_por_id_rec(nodo_arbol_id* nodo, long long id) const;
};

class nodo_arbol_nombre {
public:
    Usuario usuario;
    nodo_arbol_nombre* izquierdo;
    nodo_arbol_nombre* derecho;
    nodo_arbol_nombre(const Usuario& usuario) : usuario(usuario), izquierdo(nullptr), derecho(nullptr) {}
};

class arbol_binario_nombre {
public:
    nodo_arbol_nombre* raiz;
    arbol_binario_nombre() : raiz(nullptr) {}
    void insertar(const Usuario& usuario);
    bool buscar_por_nombre(const string& nombre_usuario) const;
private:
    nodo_arbol_nombre* insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario);
    bool buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) const;
};

extern int LIMITE_USUARIOS;

// Prototipos de utilidades
void imprimir_grilla_busqueda(const vector<resultado_busqueda>& resultados);
void exportar_resultados_insercion_csv(const vector<resultado_insercion>& grilla_id, const vector<resultado_insercion>& grilla_nombre);
void exportar_resultados_busqueda_csv(const vector<resultado_busqueda>& resultados);
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const vector<Usuario>& usuarios_validos);
vector<Usuario> leer_usuarios_validos_csv(const string& archivo);
void insertar_bst_id(arbol_binario_id& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);
void insertar_bst_nombre(arbol_binario_nombre& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);
void parsear_campos_csv(const string& linea, vector<string>& campos, size_t cantidad_campos);
Usuario parsear_usuario(const vector<string>& campos);

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

void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const vector<Usuario>& usuarios_validos) {
    vector<int> tamanos = {5000, 10000, 20000, 40000};
    vector<resultado_busqueda> resultados;
    random_device rd;
    mt19937 gen(rd());
    int N_BUSQUEDAS = 100;
    for (int n : tamanos) {
        if (static_cast<size_t>(usuarios_validos.size()) < static_cast<size_t>(n)) break;
        uniform_int_distribution<> dis(0, n-1);
        vector<long long> ids;
        for (int i = 0; i < N_BUSQUEDAS; ++i) ids.push_back(usuarios_validos[dis(gen)].id);
        volatile bool dummy_flag = false;
        auto inicio = chrono::high_resolution_clock::now();
        for (auto id : ids) dummy_flag |= bst_id.buscar_por_id(id);
        auto fin = chrono::high_resolution_clock::now();
        auto duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        double tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "id", "exitosa", tiempo_ns});
        dummy_flag = false;
        inicio = chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) dummy_flag |= bst_id.buscar_por_id(-1 - i);
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "id", "fallida", tiempo_ns});
        vector<string> nombres;
        for (int i = 0; i < N_BUSQUEDAS; ++i) nombres.push_back(usuarios_validos[dis(gen)].screen_name);
        dummy_flag = false;
        inicio = chrono::high_resolution_clock::now();
        for (auto& nombre : nombres) dummy_flag |= bst_nombre.buscar_por_nombre(nombre);
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "exitosa", tiempo_ns});
        dummy_flag = false;
        inicio = chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) dummy_flag |= bst_nombre.buscar_por_nombre("inexistente_user_" + to_string(i));
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "fallida", tiempo_ns});
    }
    // imprimir_grilla_busqueda(resultados); 
    exportar_resultados_busqueda_csv(resultados);
}

// Implementación real de insertar_bst_id
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
    getline(archivo_csv, tmp_linea); // Cabecera
    int lineas_leidas = 0;
    vector<Usuario> usuarios_validos;
    while (getline(archivo_csv, linea_csv) && lineas_leidas < LIMITE_USUARIOS) {
        vector<string> campos;
        parsear_campos_csv(linea_csv, campos, (size_t)10);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (!usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
        lineas_leidas++;
    }
    auto fin_lectura = chrono::high_resolution_clock::now();
    tiempo_lectura = chrono::duration<double>(fin_lectura - inicio_lectura).count();
    bst = arbol_binario_id(); // Asegura árbol vacío
    auto inicio_insercion = chrono::high_resolution_clock::now();
    int idx = 0;
    int max_registros = usuarios_validos.size();
    for (int n = 0; n <= max_registros; n += 5000) {
        if (n == 0) {
            auto fin = chrono::high_resolution_clock::now();
            auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin - inicio_insercion);
            grilla.push_back({"BST", "inserción", "id", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
            continue;
        }
        for (; idx < n && idx < max_registros; ++idx) {
            bst.insertar(usuarios_validos[idx]);
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

bool arbol_binario_id::buscar_por_id(long long id) const {
    return buscar_por_id_rec(raiz, id);
}

bool arbol_binario_id::buscar_por_id_rec(nodo_arbol_id* nodo, long long id) const {
    if (!nodo) return false;
    if (id == nodo->usuario.id) return true;
    if (id < nodo->usuario.id) return buscar_por_id_rec(nodo->izquierdo, id);
    return buscar_por_id_rec(nodo->derecho, id);
}

void arbol_binario_id::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

nodo_arbol_id* arbol_binario_id::insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario) {
    if (!nodo) return new nodo_arbol_id(usuario);
    if (usuario.id < nodo->usuario.id) {
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    } else if (usuario.id > nodo->usuario.id) {
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    }
    return nodo;
}
