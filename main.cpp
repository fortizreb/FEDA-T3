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
using namespace std;

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
    vector<Usuario> usuarios_validos;
    while (getline(archivo_csv, linea_csv)) {
        vector<string> campos;
        parsear_campos_csv(linea_csv, campos);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (usuario.id > 0) usuarios_validos.push_back(usuario);
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
    vector<Usuario> usuarios_validos;
    while (getline(archivo_csv, linea_csv)) {
        vector<string> campos;
        parsear_campos_csv(linea_csv, campos);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (!usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
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

void menu_busqueda(arbol_binario_id& bst_id, arbol_binario_nombre& bst_nombre) {
    while (true) {
        int opcion_busqueda = 0;
        cout << "\nBÚSQUEDA:" << endl;
        cout << "1. Buscar por id" << endl;
        cout << "2. Buscar por nombre" << endl;
        cout << "3. Salir" << endl;
        cout << "Opción: ";
        cin >> opcion_busqueda;
        string target_str, campo_str;
        double segundos = 0;
        long long milis = 0, micros = 0, nanos = 0;
        if (opcion_busqueda == 1) {
            long long id_buscar;
            cout << "\nIngrese ID: ";
            cin >> id_buscar;
            auto inicio = chrono::high_resolution_clock::now();
            bool existe = bst_id.buscar_por_id(id_buscar);
            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
            cout << (existe ? "\nEncontrado: SI" : "\nEncontrado: NO") << endl;
            target_str = to_string(id_buscar);
            campo_str = "ID";
            segundos = (double)duracion.count()/1e9;
            milis = duracion.count()/1000000;
            micros = duracion.count()/1000;
            nanos = duracion.count();
        } else if (opcion_busqueda == 2) {
            string nombre_buscar;
            cout << "\nIngrese NOMBRE: ";
            cin >> nombre_buscar;
            nombre_buscar.erase(std::remove(nombre_buscar.begin(), nombre_buscar.end(), '"'), nombre_buscar.end());
            auto inicio = chrono::high_resolution_clock::now();
            bool existe = bst_nombre.buscar_por_nombre(nombre_buscar);
            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<chrono::nanoseconds>(fin - inicio);
            cout << (existe ? "\nEncontrado: SI" : "\nEncontrado: NO") << endl;
            target_str = nombre_buscar;
            campo_str = "NOMBRE";
            segundos = (double)duracion.count()/1e9;
            milis = duracion.count()/1000000;
            micros = duracion.count()/1000;
            nanos = duracion.count();
        } else {
            break;
        }
        // Grilla de tiempos de búsqueda
        cout << "\n+------+------------+---------------------+---------------------+------------+---------------+---------------+---------------+" << endl;
        cout << "| ED   | Operación  | Target              | Valor              | Segundos   | Milisegundos  | Microsegundos | Nanosegundos  |" << endl;
        cout << "+------+------------+---------------------+---------------------+------------+---------------+---------------+---------------+" << endl;
        cout << "| " << setw(4) << left << "BST"
             << " | " << setw(11) << left << "búsqueda"
             << " | " << setw(19) << left << campo_str
             << " | " << setw(19) << left << target_str
             << " | " << setw(10) << left << fixed << setprecision(6) << segundos
             << " | " << setw(13) << left << milis
             << " | " << setw(13) << left << micros
             << " | " << setw(13) << left << nanos
             << " |" << endl;
        cout << "+------+------------+---------------------+---------------------+------------+---------------+---------------+---------------+" << endl;
    }
}

int main() {
    auto inicio_total = chrono::high_resolution_clock::now();
    int opcion = 1; // o menu();
    if (opcion == 1) {        

        arbol_binario_id bst_id;
        vector<ResultadoInsercion> grilla_id; // 

        arbol_binario_nombre bst_nombre;
        vector<ResultadoInsercion> grilla_nombre;

        
        /******* INSERCIÓN BST-ID **********/
        double tiempo_lectura_id = 0, tiempo_insercion_id = 0;
        cout << "Cargando datos en BST por ID..." << endl;
        insertar_bst_id(bst_id, "data.csv", grilla_id, tiempo_lectura_id, tiempo_insercion_id);
        imprimir_grilla_insercion(grilla_id, "INSERCIÓN BST POR ID:", grilla_id.empty() ? 0 : grilla_id.back().nodos, tiempo_lectura_id, tiempo_insercion_id);
        
        cout << "\n\n";
        
        /******* INSERCIÓN BST-NOMBRE **********/
        double tiempo_lectura_nombre = 0, tiempo_insercion_nombre = 0;
        cout << "Cargando datos en BST por NOMBRE..." << endl;
        insertar_bst_nombre(bst_nombre, "data.csv", grilla_nombre, tiempo_lectura_nombre, tiempo_insercion_nombre);
        imprimir_grilla_insercion(grilla_nombre, "INSERCIÓN BST POR NOMBRE:", grilla_nombre.empty() ? 0 : grilla_nombre.back().nodos, tiempo_lectura_nombre, tiempo_insercion_nombre);
       
        auto fin_total = chrono::high_resolution_clock::now();
        cout << "\n\rTOTAL proceso: [" << fixed << setprecision(3) << chrono::duration<double>(fin_total - inicio_total).count() << " segundos]" << endl;
        
        menu_busqueda(bst_id, bst_nombre);
    }
    return 0;
}
