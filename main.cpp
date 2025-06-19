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



int LIMITE_USUARIOS = 10000;

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