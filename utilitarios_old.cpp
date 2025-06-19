revisa que no haya indicios de IA y que las variables estén en español #include <iostream>
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

// Prototipos de parseo (definidos en parseo.cpp)
void parsear_campos_csv(const std::string& linea, std::vector<std::string>& campos, size_t cantidad_campos);
Usuario parsear_usuario(const std::vector<std::string>& campos);

void imprimir_grilla_insercion(const std::vector<resultado_insercion>& grilla, const std::string& titulo, int total_filas, double tiempo_lectura, double tiempo_insercion) {
    std::cout << "\n" << titulo;
    std::cout << "\nLectura y filtrado [" << std::fixed << std::setprecision(3) << tiempo_lectura << " segundos]" << std::endl;
    std::cout << "Inserción [" << std::fixed << std::setprecision(6) << tiempo_insercion << " segundos]";
    std::cout << "\n+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << std::endl;
    std::cout << "| ED   | Operación  | Target              | Nodos creados/Total     | Segundos   | Milisegundos  | Microsegundos | Nanosegundos  |" << std::endl;
    std::cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << std::endl;
    if (grilla.empty()) {
        std::cout << "| (sin datos)" << std::endl;
        std::cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << std::endl;
        return;
    }
    bool first = true;
    resultado_insercion ultimo = grilla.back();
    for (const auto& r : grilla) {
        if (first && r.cantidad_nodos == 0) { first = false; continue; }
        std::cout << "| " << std::setw(4) << std::left << r.estructura_datos
             << " | " << std::setw(11) << std::left << r.operacion
             << " | " << std::setw(19) << std::left << r.objetivo
             << " | " << std::setw(24) << std::left << (std::to_string(r.cantidad_nodos) + "/" + std::to_string(total_filas))
             << " | " << std::setw(10) << std::left << std::fixed << std::setprecision(6) << r.segundos
             << " | " << std::setw(13) << std::left << r.milisegundos
             << " | " << std::setw(13) << std::left << r.microsegundos
             << " | " << std::setw(13) << std::left << r.nanosegundos
             << " |" << std::endl;
        first = false;
    }
    std::cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << std::endl;
    std::cout << "| " << std::setw(4) << std::left << "TOT"
         << " | " << std::setw(10) << std::left << "-"
         << " | " << std::setw(19) << std::left << "-"
         << " | " << std::setw(24) << std::left << "-"
         << " | " << std::setw(10) << std::left << std::fixed << std::setprecision(6) << ultimo.segundos
         << " | " << std::setw(13) << std::left << ultimo.milisegundos
         << " | " << std::setw(13) << std::left << ultimo.microsegundos
         << " | " << std::setw(13) << std::left << ultimo.nanosegundos
         << " |" << std::endl;
    std::cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << std::endl;
}

void exportar_resultados_insercion_csv(const std::vector<resultado_insercion>& grilla_id, const std::vector<resultado_insercion>& grilla_nombre) {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "insert_bst_%H-%M-%S_(%d-%b).csv", local);
    std::string ruta = "resultados/" + std::string(buffer);
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo crear el archivo: " << ruta << std::endl;
        return;
    }
    archivo << "n_usuarios,clave,tiempo_ms\n";
    for (const auto& r : grilla_id) {
        if (r.cantidad_nodos == 0) continue;
        archivo << r.cantidad_nodos << ",id," << std::fixed << std::setprecision(1) << (double)r.milisegundos << "\n";
    }
    for (const auto& r : grilla_nombre) {
        if (r.cantidad_nodos == 0) continue;
        archivo << r.cantidad_nodos << ",screenName," << std::fixed << std::setprecision(1) << (double)r.milisegundos << "\n";
    }
    archivo.close();
    std::cout << "Resultados exportados a '" << ruta << "'" << std::endl;
}

void imprimir_grilla_busqueda(const std::vector<resultado_busqueda>& resultados) {
    std::cout << "\n+----------+-----------+-----------+-------------------+" << std::endl;
    std::cout << "| Usuarios | Clave     | Tipo      | Tiempo (ns)       |" << std::endl;
    std::cout << "+----------+-----------+-----------+-------------------+" << std::endl;
    for (const auto& r : resultados) {
        std::cout << "| " << std::setw(8) << std::left << r.cantidad_usuarios
                  << " | " << std::setw(9) << std::left << r.clave
                  << " | " << std::setw(9) << std::left << r.tipo_busqueda
                  << " | " << std::setw(17) << std::left << std::fixed << std::setprecision(1) << r.tiempo_ns
                  << " |" << std::endl;
    }
    std::cout << "+----------+-----------+-----------+-------------------+" << std::endl;
}

void exportar_resultados_busqueda_csv(const std::vector<resultado_busqueda>& resultados) {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "busqueda_bst_%H-%M-%S_(%d-%b).csv", local);
    std::string ruta = "resultados/" + std::string(buffer);
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo crear el archivo: " << ruta << std::endl;
        return;
    }
    archivo << "n_usuarios,clave,tipo,tiempo_ns\n";
    for (const auto& r : resultados) {
        archivo << r.cantidad_usuarios << "," << r.clave << "," << r.tipo_busqueda << "," << std::fixed << std::setprecision(1) << r.tiempo_ns << "\n";
    }
    archivo.close();
    std::cout << "Resultados de búsqueda exportados a '" << ruta << "'" << std::endl;
}

void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const std::vector<Usuario>& usuarios_validos) {
    std::vector<int> tamanos = {5000, 10000, 20000, 40000};
    std::vector<resultado_busqueda> resultados;
    std::random_device rd;
    std::mt19937 gen(rd());
    int N_BUSQUEDAS = 100;
    for (int n : tamanos) {
        if (static_cast<size_t>(usuarios_validos.size()) < static_cast<size_t>(n)) break;
        std::uniform_int_distribution<> dis(0, n-1);
        std::vector<long long> ids;
        for (int i = 0; i < N_BUSQUEDAS; ++i) ids.push_back(usuarios_validos[dis(gen)].id);
        volatile bool dummy_flag = false;
        auto inicio = std::chrono::high_resolution_clock::now();
        for (auto id : ids) dummy_flag |= bst_id.buscar_por_id(id);
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
        double tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "id", "exitosa", tiempo_ns});
        dummy_flag = false;
        inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) dummy_flag |= bst_id.buscar_por_id(-1 - i);
        fin = std::chrono::high_resolution_clock::now();
        duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "id", "fallida", tiempo_ns});
        std::vector<std::string> nombres;
        for (int i = 0; i < N_BUSQUEDAS; ++i) nombres.push_back(usuarios_validos[dis(gen)].screen_name);
        dummy_flag = false;
        inicio = std::chrono::high_resolution_clock::now();
        for (auto& nombre : nombres) dummy_flag |= bst_nombre.buscar_por_nombre(nombre);
        fin = std::chrono::high_resolution_clock::now();
        duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "exitosa", tiempo_ns});
        dummy_flag = false;
        inicio = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) dummy_flag |= bst_nombre.buscar_por_nombre("inexistente_user_" + std::to_string(i));
        fin = std::chrono::high_resolution_clock::now();
        duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "fallida", tiempo_ns});
    }
    // imprimir_grilla_busqueda(resultados); // Comentado para no imprimir la grilla
    exportar_resultados_busqueda_csv(resultados);
}

// Implementación real de insertar_bst_id
void insertar_bst_id(arbol_binario_id& bst, const std::string& archivo, std::vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion) {
    auto inicio_lectura = std::chrono::high_resolution_clock::now();
    std::ifstream archivo_csv(archivo);
    if (!archivo_csv.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << archivo << std::endl;
        tiempo_lectura = 0;
        tiempo_insercion = 0;
        return;
    }
    std::string tmp_linea, linea_csv;
    std::getline(archivo_csv, tmp_linea); // Cabecera
    int lineas_leidas = 0;
    std::vector<Usuario> usuarios_validos;
    while (std::getline(archivo_csv, linea_csv) && lineas_leidas < LIMITE_USUARIOS) {
        std::vector<std::string> campos;
        parsear_campos_csv(linea_csv, campos, (size_t)10);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (!usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
        lineas_leidas++;
    }
    auto fin_lectura = std::chrono::high_resolution_clock::now();
    tiempo_lectura = std::chrono::duration<double>(fin_lectura - inicio_lectura).count();
    bst = arbol_binario_id(); // Asegura árbol vacío
    auto inicio_insercion = std::chrono::high_resolution_clock::now();
    int idx = 0;
    int max_registros = usuarios_validos.size();
    for (int n = 0; n <= max_registros; n += 5000) {
        if (n == 0) {
            auto fin = std::chrono::high_resolution_clock::now();
            auto duracion_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio_insercion);
            grilla.push_back({"BST", "inserción", "id", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
            continue;
        }
        for (; idx < n && idx < max_registros; ++idx) {
            bst.insertar(usuarios_validos[idx]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio_insercion);
        grilla.push_back({"BST", "inserción", "id", n, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
    }
    // Inserta los que faltan si no es múltiplo de 5000
    if (idx < max_registros) {
        for (; idx < max_registros; ++idx) {
            bst.insertar(usuarios_validos[idx]);
        }
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio_insercion);
        grilla.push_back({"BST", "inserción", "id", max_registros, (double)duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
    }
    tiempo_insercion = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - inicio_insercion).count();
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
