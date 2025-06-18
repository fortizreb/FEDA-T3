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

// Funciones de parseo y limpieza
std::string limpiar_campo(const std::string& s) {
    std::string out = s;
    out.erase(remove(out.begin(), out.end(), '"'), out.end());
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    return out;
}

std::vector<std::string> parsear_tags(const std::string& campo) {
    std::vector<std::string> tags;
    std::string limpio = limpiar_campo(campo);
    if (limpio.size() > 2 && limpio.front() == '[' && limpio.back() == ']') {
        std::string inner = limpio.substr(1, limpio.size() - 2);
        std::stringstream ss(inner);
        std::string tag;
        while (getline(ss, tag, ',')) {
            if (!tag.empty()) tags.push_back(tag);
        }
    }
    return tags;
}

std::vector<long long> parsear_friends(const std::string& campo) {
    std::vector<long long> friends;
    std::string limpio = limpiar_campo(campo);
    if (limpio.size() > 2 && limpio.front() == '[' && limpio.back() == ']') {
        std::string inner = limpio.substr(1, limpio.size() - 2);
        std::stringstream ss(inner);
        std::string num;
        while (getline(ss, num, ',')) {
            try { if (!num.empty()) friends.push_back(stoll(num)); } catch (...) {}
        }
    }
    return friends;
}

Usuario parsear_usuario(const std::vector<std::string>& campos) {
    Usuario usuario;
    std::string id_limpio = limpiar_campo(campos[0]);
    usuario.id = (!id_limpio.empty() && std::all_of(id_limpio.begin(), id_limpio.end(), ::isdigit)) ? std::stoll(id_limpio) : 0;
    usuario.screen_name = limpiar_campo(campos[1]);
    usuario.tags = parsear_tags(campos[2]);
    usuario.avatar = limpiar_campo(campos[3]);
    usuario.followers_count = (campos.size() > 4 && !campos[4].empty() && std::all_of(campos[4].begin(), campos[4].end(), ::isdigit)) ? std::stoi(campos[4]) : 0;
    usuario.friends_count = (campos.size() > 5 && !campos[5].empty() && std::all_of(campos[5].begin(), campos[5].end(), ::isdigit)) ? std::stoi(campos[5]) : 0;
    usuario.lang = limpiar_campo(campos[6]);
    usuario.last_seen = (campos.size() > 7 && !campos[7].empty() && std::all_of(campos[7].begin(), campos[7].end(), ::isdigit)) ? std::stoll(campos[7]) : 0;
    std::string tweet_id_limpio = limpiar_campo(campos[8]);
    usuario.tweet_id = (!tweet_id_limpio.empty() && std::all_of(tweet_id_limpio.begin(), tweet_id_limpio.end(), ::isdigit)) ? std::stoll(tweet_id_limpio) : 0;
    usuario.friends = parsear_friends(campos[9]);
    return usuario;
}

void parsear_campos_csv(const std::string& linea, std::vector<std::string>& campos, size_t esperado) {
    campos.clear();
    campos.reserve(esperado);
    size_t start = 0, end = 0;
    while ((end = linea.find(',', start)) != std::string::npos && campos.size() < esperado - 1) {
        campos.push_back(linea.substr(start, end - start));
        start = end + 1;
    }
    campos.push_back(linea.substr(start));
}

void asignar_si_valido_string(std::string& destino, const std::vector<std::string>& campos, size_t idx) {
    if (campos.size() > idx && !campos[idx].empty() && campos[idx] != "\"\"" && campos[idx] != "null") {
        destino = campos[idx];
    } else {
        destino = "";
    }
}
void asignar_si_valido_int(int& destino, const std::vector<std::string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = std::stoi(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}
void asignar_si_valido_longlong(long long& destino, const std::vector<std::string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = std::stoll(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}

std::vector<Usuario> leer_usuarios_validos_csv(const std::string& archivo) {
    std::vector<Usuario> usuarios_validos;
    std::ifstream archivo_csv(archivo);
    if (!archivo_csv.is_open()) return usuarios_validos;
    std::string tmp_linea, linea_csv;
    getline(archivo_csv, tmp_linea); // Cabecera
    while (getline(archivo_csv, linea_csv)) {
        std::vector<std::string> campos;
        parsear_campos_csv(linea_csv, campos, 10);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (usuario.id > 0 && !usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
        if (usuarios_validos.size() >= LIMITE_USUARIOS) break;
    }
    return usuarios_validos;
}
