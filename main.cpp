#include "usuario.h"
#include "lectura_data.h"
#include "arbol_binario_id.h"
#include "arbol_binario_nombre.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip> //para las funciones setw y setprecision de la grilla donde se muestran los resultados
#include <algorithm>
#include <vector>
#include <sstream>
#include <ctime>
#include <random>

using namespace std;

// Variable global para controlar la cantidad máxima de filas a insertar y probar
int LIMITE_USUARIOS = 40000;

// Funciones para validar y asignar campos (inline para evitar overhead de llamadas)
inline void asignar_si_valido_string(string& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx && !campos[idx].empty() && campos[idx] != "\"\"" && campos[idx] != "null") {
        destino = campos[idx];
    } else {
        destino = "";
    }
}
inline void asignar_si_valido_int(int& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = stoi(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}
inline void asignar_si_valido_longlong(long long& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = stoll(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}

// función para limpiar comillas dobles y espacios
inline string limpiar_campo(const string& s) {
    string out = s;
    out.erase(remove(out.begin(), out.end(), '"'), out.end());
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    return out;
}
// Parsear vector<string> desde csv tipo [tag1,tag2,...]
inline vector<string> parsear_tags(const string& campo) {
    vector<string> tags;
    string limpio = limpiar_campo(campo);
    if (limpio.size() > 2 && limpio.front() == '[' && limpio.back() == ']') {
        string inner = limpio.substr(1, limpio.size() - 2);
        stringstream ss(inner);
        string tag;
        while (getline(ss, tag, ',')) {
            if (!tag.empty()) tags.push_back(tag);
        }
    }
    return tags;
}
// Parsear friends desde CSV tipo [id1,id2,id3]
inline vector<long long> parsear_friends(const string& campo) {
    vector<long long> friends;
    string limpio = limpiar_campo(campo);
    if (limpio.size() > 2 && limpio.front() == '[' && limpio.back() == ']') {
        string inner = limpio.substr(1, limpio.size() - 2);
        stringstream ss(inner);
        string num;
        while (getline(ss, num, ',')) {
            try { if (!num.empty()) friends.push_back(stoll(num)); } catch (...) {}
        }
    }
    return friends;
}

struct ResultadoInsercion {
    string ed;
    string operacion;
    string target;
    int nodos;
    double segundos;
    long long milisegundos;
    long long microsegundos;
    long long nanosegundos;
};

// Cambia a nanosegundos para mayor precisión
struct ResultadoBusqueda {
    int n_usuarios;
    string clave;
    string tipo_busqueda;
    double tiempo_ns; // nanosegundos
};

//llenamos la estructura Usuario a partir de un vector de strings
Usuario parsear_usuario(const vector<string>& campos) {
    Usuario usuario;
    string id_limpio = limpiar_campo(campos[0]);
    asignar_si_valido_longlong(usuario.id, vector<string>{id_limpio}, 0);
    usuario.screen_name = limpiar_campo(campos[1]);
    usuario.tags = parsear_tags(campos[2]);
    usuario.avatar = limpiar_campo(campos[3]);
    asignar_si_valido_int(usuario.followers_count, campos, 4);
    asignar_si_valido_int(usuario.friends_count, campos, 5);
    usuario.lang = limpiar_campo(campos[6]);
    // asignar_si_valido(usuario.last_seen, campos, 7);
    asignar_si_valido_longlong(usuario.last_seen, campos, 7); // corregido para tipo long long
    string tweet_id_limpio = limpiar_campo(campos[8]);
    asignar_si_valido_longlong(usuario.tweet_id, vector<string>{tweet_id_limpio}, 0);
    usuario.friends = parsear_friends(campos[9]);
    return usuario;
}

// Optimización: parseo y validación rápida de usuarios desde CSV usando solo STL
// Reemplaza el parseo de usuarios en contar_usuarios_validos_id y contar_usuarios_validos_nombre
// y en la carga de usuarios para inserción

// Nueva función de parseo rápido de línea CSV (sin manejo de comillas internas)
inline void parsear_campos_csv(const string& linea, vector<string>& campos, size_t esperado = 10) {
    campos.clear();
    campos.reserve(esperado);
    size_t start = 0, end = 0;
    while ((end = linea.find(',', start)) != string::npos && campos.size() < esperado - 1) {
        campos.push_back(linea.substr(start, end - start));
        start = end + 1;
    }
    campos.push_back(linea.substr(start));
}

// Usar esta función en vez de parsear_csv en los lugares críticos
// Ejemplo de uso en contar_usuarios_validos_id:
// vector<string> campos;
// parsear_campos_csv(linea_csv, campos);
// ...
/// (igual para la carga de usuarios en insertar_bst_id y nombre)

//Función para inserción en BST-ID
void insertar_bst_id(arbol_binario_id& bst, const string& archivo, std::vector<ResultadoInsercion>& grilla, double& tiempo_lectura, double& tiempo_insercion) {
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
        parsear_campos_csv(linea_csv, campos);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (usuario.id > 0) usuarios_validos.push_back(usuario);
        lineas_leidas++;
    }
    auto fin_lectura = chrono::high_resolution_clock::now();
    tiempo_lectura = chrono::duration<double>(fin_lectura - inicio_lectura).count();
    // Inserción incremental y medición acumulativa
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
    auto fin_insercion = chrono::high_resolution_clock::now();
    tiempo_insercion = chrono::duration<double>(fin_insercion - inicio_insercion).count();
}

void insertar_bst_nombre(arbol_binario_nombre& bst, const string& archivo, std::vector<ResultadoInsercion>& grilla, double& tiempo_lectura, double& tiempo_insercion) {
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
        parsear_campos_csv(linea_csv, campos);
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

void imprimir_grilla_insercion(const vector<ResultadoInsercion>& grilla, const string& titulo, int total_filas, double tiempo_lectura, double tiempo_insercion) {
    cout << "\n" << titulo;
    cout << "\nLectura y filtrado [" << fixed << setprecision(3) << tiempo_lectura << " segundos]" << endl;
    cout << "Inserción [" << fixed << setprecision(6) << tiempo_insercion << " segundos]";
    cout << "\n+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << endl;
    cout << "| ED   | Operación  | Target              | Nodos creados/Total     | Segundos   | Milisegundos  | Microsegundos | Nanosegundos  |" << endl;
    cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << endl;
    bool first = true;
    ResultadoInsercion ultimo = grilla.back();
    for (const auto& r : grilla) {
        if (first && r.nodos == 0) { first = false; continue; } // omite el nodo 0
        cout << "| " << setw(4) << left << r.ed
             << " | " << setw(11) << left << r.operacion
             << " | " << setw(19) << left << r.target
             << " | " << setw(24) << left << (to_string(r.nodos) + "/" + to_string(total_filas))
             << " | " << setw(10) << left << fixed << setprecision(6) << r.segundos
             << " | " << setw(13) << left << r.milisegundos
             << " | " << setw(13) << left << r.microsegundos
             << " | " << setw(13) << left << r.nanosegundos
             << " |" << endl;
        first = false;
    }
    // Footer con el tiempo real total de inserción (última fila)
    cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << endl;
    cout << "| " << setw(4) << left << "TOT"
         << " | " << setw(10) << left << "-"
         << " | " << setw(19) << left << "-"
         << " | " << setw(24) << left << "-"
         << " | " << setw(10) << left << fixed << setprecision(6) << ultimo.segundos
         << " | " << setw(13) << left << ultimo.milisegundos
         << " | " << setw(13) << left << ultimo.microsegundos
         << " | " << setw(13) << left << ultimo.nanosegundos
         << " |" << endl;
    cout << "+------+------------+---------------------+--------------------------+------------+---------------+---------------+---------------+" << endl;
}

// Función para exportar resultados a un archivo CSV
void exportar_resultados_csv(const vector<ResultadoInsercion>& grilla_id, const vector<ResultadoInsercion>& grilla_nombre) {
    // Obtener fecha y hora actual
    time_t now = time(nullptr);
    tm* local = localtime(&now);
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
        if (r.nodos == 0) continue;
        archivo << r.nodos << ",id," << fixed << setprecision(1) << (double)r.milisegundos << "\n";
    }
    for (const auto& r : grilla_nombre) {
        if (r.nodos == 0) continue;
        archivo << r.nodos << ",screenName," << fixed << setprecision(1) << (double)r.milisegundos << "\n";
    }
    archivo.close();
    cout << "Resultados exportados a '" << ruta << "'" << endl;
}

void pruebas_busqueda_y_exportar(const vector<Usuario>& usuarios_validos) {
    cout << "REALIZANDO BUSQUEDAS POR ID Y NOMBRE..." << endl;
    vector<int> tamanos = {5000, 10000, 20000, 40000};
    vector<ResultadoBusqueda> resultados;
    random_device rd;
    mt19937 gen(rd());
    int N_BUSQUEDAS = 100;
    for (int n : tamanos) {
        if (usuarios_validos.size() < n) break;
        arbol_binario_id bst_id;
        for (int i = 0; i < n; ++i) bst_id.insertar(usuarios_validos[i]);
        arbol_binario_nombre bst_nombre;
        for (int i = 0; i < n; ++i) bst_nombre.insertar(usuarios_validos[i]);
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
        // --- Búsquedas fallidas ID ---
        dummy_flag = false;
        inicio = chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) dummy_flag |= bst_id.buscar_por_id(-1 - i);
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "id", "fallida", tiempo_ns});
        vector<string> names;
        for (int i = 0; i < N_BUSQUEDAS; ++i) names.push_back(usuarios_validos[dis(gen)].screen_name);
        dummy_flag = false;
        inicio = chrono::high_resolution_clock::now();
        for (auto& name : names) dummy_flag |= bst_nombre.buscar_por_nombre(name);
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "exitosa", tiempo_ns});
        // --- Búsquedas fallidas screenName ---
        dummy_flag = false;
        inicio = chrono::high_resolution_clock::now();
        for (int i = 0; i < N_BUSQUEDAS; ++i) dummy_flag |= bst_nombre.buscar_por_nombre("inexistente_user_" + to_string(i));
        fin = chrono::high_resolution_clock::now();
        duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
        tiempo_ns = static_cast<double>(duracion.count()) / N_BUSQUEDAS;
        resultados.push_back({n, "screenName", "fallida", tiempo_ns});
    }
    // Imprimir una sola grilla final
    cout << "\n+----------+-----------+--------------+-------------+" << endl;
    cout << "| Usuarios |   Clave   | TipoBusqueda | Tiempo_ns   |" << endl;
    cout << "+----------+-----------+--------------+-------------+" << endl;
    for (const auto& r : resultados) {
        cout << "| " << setw(8) << r.n_usuarios << " | " << setw(9) << r.clave << " | " << setw(12) << r.tipo_busqueda << " | " << setw(11) << fixed << setprecision(1) << r.tiempo_ns << " |" << endl;
    }
    cout << "+----------+-----------+--------------+-------------+" << endl;
    // Exportar resultados
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "busqueda_bst_%H-%M-%S_(%d-%b).csv", local);
    string ruta = "resultados/" + string(buffer);
    ofstream archivo(ruta);
    archivo << "n_usuarios,clave,tipo_busqueda,tiempo_ns\n";
    for (const auto& r : resultados) {
        archivo << r.n_usuarios << ',' << r.clave << ',' << r.tipo_busqueda << ',' << fixed << setprecision(1) << r.tiempo_ns << '\n';
    }
    archivo.close();
    cout << "Resultados de búsqueda exportados a '" << ruta << "'" << endl;
}

int main() {
    auto inicio_total = chrono::high_resolution_clock::now();
    arbol_binario_id bst_id;
    vector<ResultadoInsercion> grilla_id;
    arbol_binario_nombre bst_nombre;
    vector<ResultadoInsercion> grilla_nombre;
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
            imprimir_grilla_insercion(grilla_id, "INSERCIÓN BST POR ID:", grilla_id.empty() ? 0 : grilla_id.back().nodos, tiempo_lectura_id, tiempo_insercion_id);
            cout << "\n\n";
            cout << "Cargando datos en BST por NOMBRE..." << endl;
            insertar_bst_nombre(bst_nombre, "data.csv", grilla_nombre, tiempo_lectura_nombre, tiempo_insercion_nombre);
            imprimir_grilla_insercion(grilla_nombre, "INSERCIÓN BST POR NOMBRE:", grilla_nombre.empty() ? 0 : grilla_nombre.back().nodos, tiempo_lectura_nombre, tiempo_insercion_nombre);
            auto fin_total = chrono::high_resolution_clock::now();
            cout << "\nTOTAL proceso: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total - inicio_total).count() << " segundos]" << endl;
            // Guardar automáticamente el archivo CSV
            exportar_resultados_csv(grilla_id, grilla_nombre);
            // Generar vector de usuarios válidos para pruebas de búsqueda
            vector<Usuario> usuarios_validos;
            ifstream archivo_csv("data.csv");
            string tmp_linea, linea_csv;
            getline(archivo_csv, tmp_linea); // Cabecera
            while (getline(archivo_csv, linea_csv)) {
                vector<string> campos;
                parsear_campos_csv(linea_csv, campos);
                if (campos.size() < 10) continue;
                Usuario usuario = parsear_usuario(campos);
                if (usuario.id > 0 && !usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
                if (usuarios_validos.size() >= LIMITE_USUARIOS) break;
            }
            pruebas_busqueda_y_exportar(usuarios_validos);
        } else if (opcion == 2) {
            cout << "Saliendo..." << endl;
            break;
        } else {
            cout << "Opción no válida. Intente de nuevo." << endl;
        }
    }
    return 0;
}
