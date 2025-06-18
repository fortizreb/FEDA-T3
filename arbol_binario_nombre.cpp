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

struct Usuario {
    long long id;
    std::string screen_name;
    std::vector<std::string> tags;
    std::string avatar;
    int followers_count;
    int friends_count;
    std::string lang;
    long long last_seen;
    long long tweet_id;
    std::vector<long long> friends;
};

struct resultado_insercion {
    std::string estructura_datos;
    std::string operacion;
    std::string objetivo;
    int cantidad_nodos;
    double segundos;
    long long milisegundos;
    long long microsegundos;
    long long nanosegundos;
};

struct resultado_busqueda {
    int cantidad_usuarios;
    std::string clave;
    std::string tipo_busqueda;
    double tiempo_ns;
};

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
    Usuario* buscar_usuario_por_id(long long id);
    void imprimir_primeros_n(int n);
    Usuario* buscar_usuario(std::function<bool(const Usuario&)> criterio);
private:
    nodo_arbol_id* insertar_rec(nodo_arbol_id* nodo, const Usuario& usuario);
    bool buscar_por_id_rec(nodo_arbol_id* nodo, long long id) const;
    Usuario* buscar_usuario_por_id_rec(nodo_arbol_id* nodo, long long id);
    void imprimir_primeros_n_rec(nodo_arbol_id* nodo, int& contador, int n);
    Usuario* buscar_usuario_rec(nodo_arbol_id* nodo, std::function<bool(const Usuario&)> criterio);
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
    bool buscar_por_nombre(const std::string& nombre_usuario) const;
    Usuario* buscar_usuario_por_nombre(const std::string& nombre_usuario);
    void imprimir_primeros_n(int n);
    Usuario* buscar_usuario(std::function<bool(const Usuario&)> criterio);
private:
    nodo_arbol_nombre* insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario);
    bool buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const std::string& nombre_usuario) const;
    Usuario* buscar_usuario_por_nombre_rec(nodo_arbol_nombre* nodo, const std::string& nombre_usuario);
    void imprimir_primeros_n_rec(nodo_arbol_nombre* nodo, int& contador, int n);
    Usuario* buscar_usuario_rec(nodo_arbol_nombre* nodo, std::function<bool(const Usuario&)> criterio);
};

class HashAbierto {
public:
    std::vector<std::list<Usuario>> tabla;
    size_t tamano_tabla;
    HashAbierto(size_t tamano) {
        tamano_tabla = tamano;
        tabla = std::vector<std::list<Usuario>>(tamano_tabla);
    }
    void insertar(const Usuario& usuario) {
        size_t indice = std::hash<long long>{}(usuario.id) % tamano_tabla;
        tabla[indice].push_back(usuario);
    }
    bool buscar_por_id(long long id) {
        size_t indice = std::hash<long long>{}(id) % tamano_tabla;
        for (const auto& usuario : tabla[indice]) {
            if (usuario.id == id) return true;
        }
        return false;
    }
    bool buscar_por_nombre(const std::string& nombre_usuario) {
        size_t indice = std::hash<std::string>{}(nombre_usuario) % tamano_tabla;
        for (const auto& usuario : tabla[indice]) {
            if (usuario.screen_name == nombre_usuario) return true;
        }
        return false;
    }
};

class HashCerrado {
public:
    std::vector<std::optional<Usuario>> tabla;
    size_t tamano_tabla;
    HashCerrado(size_t tamano) {
        tamano_tabla = tamano;
        tabla = std::vector<std::optional<Usuario>>(tamano_tabla);
    }
    void insertar(const Usuario& usuario) {
        size_t indice = std::hash<long long>{}(usuario.id) % tamano_tabla;
        while (tabla[indice].has_value()) {
            indice = (indice + 1) % tamano_tabla;
        }
        tabla[indice] = usuario;
    }
    bool buscar_por_id(long long id) {
        size_t indice = std::hash<long long>{}(id) % tamano_tabla;
        size_t inicio = indice;
        while (tabla[indice].has_value()) {
            if (tabla[indice]->id == id) return true;
            indice = (indice + 1) % tamano_tabla;
            if (indice == inicio) break;
        }
        return false;
    }
    bool buscar_por_nombre(const std::string& nombre_usuario) {
        size_t indice = std::hash<std::string>{}(nombre_usuario) % tamano_tabla;
        size_t inicio = indice;
        while (tabla[indice].has_value()) {
            if (tabla[indice]->screen_name == nombre_usuario) return true;
            indice = (indice + 1) % tamano_tabla;
            if (indice == inicio) break;
        }
        return false;
    }
};

extern int LIMITE_USUARIOS;

// Prototipos globales para todo el proyecto
void imprimir_grilla_insercion(const std::vector<resultado_insercion>& grilla, const std::string& titulo, int total_filas, double tiempo_lectura, double tiempo_insercion);
void imprimir_grilla_busqueda(const std::vector<resultado_busqueda>& resultados);
void exportar_resultados_insercion_csv(const std::vector<resultado_insercion>& grilla_id, const std::vector<resultado_insercion>& grilla_nombre);
void exportar_resultados_busqueda_csv(const std::vector<resultado_busqueda>& resultados);
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const std::vector<Usuario>& usuarios_validos);
std::vector<Usuario> leer_usuarios_validos_csv(const std::string& archivo);
void insertar_bst_id(arbol_binario_id& arbol, const std::string& archivo, std::vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);
void insertar_bst_nombre(arbol_binario_nombre& arbol, const std::string& archivo, std::vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);

void parsear_campos_csv(const std::string& linea, std::vector<std::string>& campos, size_t esperado = 10);
Usuario parsear_usuario(const std::vector<std::string>& campos);

using namespace std;

void arbol_binario_nombre::insertar(const Usuario& usuario) {
    raiz = insertar_rec(raiz, usuario);
}

nodo_arbol_nombre* arbol_binario_nombre::insertar_rec(nodo_arbol_nombre* nodo, const Usuario& usuario) {
    if (!nodo) return new nodo_arbol_nombre(usuario);
    if (usuario.screen_name < nodo->usuario.screen_name)
        nodo->izquierdo = insertar_rec(nodo->izquierdo, usuario);
    else if (usuario.screen_name > nodo->usuario.screen_name)
        nodo->derecho = insertar_rec(nodo->derecho, usuario);
    return nodo;
}

bool arbol_binario_nombre::buscar_por_nombre(const string& nombre_usuario) const {
    return buscar_por_nombre_rec(raiz, nombre_usuario);
}

bool arbol_binario_nombre::buscar_por_nombre_rec(nodo_arbol_nombre* nodo, const string& nombre_usuario) const {
    if (!nodo) return false;
    if (nombre_usuario == nodo->usuario.screen_name) return true;
    if (nombre_usuario < nodo->usuario.screen_name) return buscar_por_nombre_rec(nodo->izquierdo, nombre_usuario);
    return buscar_por_nombre_rec(nodo->derecho, nombre_usuario);
}

Usuario* arbol_binario_nombre::buscar_usuario_por_nombre(const string& nombre_usuario) {
    return buscar_usuario_por_nombre_rec(raiz, nombre_usuario);
}

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

void arbol_binario_nombre::imprimir_primeros_n(int n) {
    int contador = 0;
    imprimir_primeros_n_rec(raiz, contador, n);
}

void arbol_binario_nombre::imprimir_primeros_n_rec(nodo_arbol_nombre* nodo, int& contador, int n) {
    if (!nodo || contador >= n) return;
    imprimir_primeros_n_rec(nodo->izquierdo, contador, n);
    if (contador < n) {
        std::cout << nodo->usuario.id << " | " << nodo->usuario.screen_name << std::endl;
        contador++;
    }
    imprimir_primeros_n_rec(nodo->derecho, contador, n);
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
