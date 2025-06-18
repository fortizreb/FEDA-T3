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

int LIMITE_USUARIOS = 20000;

// Prototipos globales para todo el proyecto
void imprimir_grilla_insercion(const std::vector<resultado_insercion>& grilla, const std::string& titulo, int total_filas, double tiempo_lectura, double tiempo_insercion);
void imprimir_grilla_busqueda(const std::vector<resultado_busqueda>& resultados);
void exportar_resultados_insercion_csv(const std::vector<resultado_insercion>& grilla_id, const std::vector<resultado_insercion>& grilla_nombre);
void exportar_resultados_busqueda_csv(const std::vector<resultado_busqueda>& resultados);
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const std::vector<Usuario>& usuarios_validos);
std::vector<Usuario> leer_usuarios_validos_csv(const std::string& archivo);
void insertar_bst_id(arbol_binario_id& arbol, const std::string& archivo, std::vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);
void insertar_bst_nombre(arbol_binario_nombre& arbol, const std::string& archivo, std::vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);

using namespace std;

int main() {
    auto inicio_total = chrono::high_resolution_clock::now();
    arbol_binario_id bst_id;
    vector<resultado_insercion> grilla_id;
    arbol_binario_nombre bst_nombre;
    vector<resultado_insercion> grilla_nombre;
    double tiempo_lectura_id = 0, tiempo_insercion_id = 0;
    double tiempo_lectura_nombre = 0, tiempo_insercion_nombre = 0;
    while (true) {
        cout << "\n===== MENÚ PRINCIPAL =====" << endl;
        cout << "1. Cargar e insertar datos en BST por ID y NOMBRE" << endl;
        cout << "2. Salir" << endl;
        cout << "Seleccione una opción: ";
        int opcion;
        cin >> opcion;
        if (opcion == 1) {
            cout << "Cargando datos en BST por ID..." << endl;
            insertar_bst_id(bst_id, "data.csv", grilla_id, tiempo_lectura_id, tiempo_insercion_id);
            imprimir_grilla_insercion(grilla_id, "INSERCIÓN BST POR ID:", grilla_id.empty() ? 0 : grilla_id.back().cantidad_nodos, tiempo_lectura_id, tiempo_insercion_id);
            cout << "\n\n";
            cout << "Cargando datos en BST por NOMBRE..." << endl;
            insertar_bst_nombre(bst_nombre, "data.csv", grilla_nombre, tiempo_lectura_nombre, tiempo_insercion_nombre);
            imprimir_grilla_insercion(grilla_nombre, "INSERCIÓN BST POR NOMBRE:", grilla_nombre.empty() ? 0 : grilla_nombre.back().cantidad_nodos, tiempo_lectura_nombre, tiempo_insercion_nombre);
            auto fin_total = chrono::high_resolution_clock::now();
            cout << "\nTotal inserción: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total - inicio_total).count() << " segundos]" << endl;
            exportar_resultados_insercion_csv(grilla_id, grilla_nombre); //guardamos resultados en archivo.
            cout << "\nBuscando en BST por ID por NOMBRE..." << endl;
            vector<Usuario> usuarios_validos = leer_usuarios_validos_csv("data.csv"); 
            busqueda_bst(bst_id, bst_nombre, usuarios_validos);
        } else if (opcion == 2) {
            break;
        } else {
            cout << "Opción no válida. Intente de nuevo." << endl;
        }
    }
    return 0;
}