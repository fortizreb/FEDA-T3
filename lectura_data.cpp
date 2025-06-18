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

std::vector<Usuario> leer_usuarios_desde_csv(const std::string& rutaArchivo) {
   
    return {};
}

std::vector<std::string> parsear_csv(const std::string& linea) {
    std::vector<std::string> campos;
    std::string campo;
    bool en_comillas = false;
    int corchetes = 0;
    for (size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c == '"') {
            en_comillas = !en_comillas;
            campo += c;
        } else if (c == '[' && !en_comillas) {
            corchetes++;
            campo += c;
        } else if (c == ']' && !en_comillas) {
            corchetes--;
            campo += c;
        } else if (c == ',' && !en_comillas && corchetes == 0) {
            campos.push_back(campo);
            campo.clear();
        } else {
            campo += c;
        }
    }
    campos.push_back(campo);
    return campos;
}
