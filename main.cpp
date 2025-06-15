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
#include <tuple>
#include <functional>
using namespace std;

// Funciones utilitarias para validar y asignar campos
inline void asignar_si_valido(string& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx && !campos[idx].empty() && campos[idx] != "\"\"" && campos[idx] != "null") {
        destino = campos[idx];
    } else {
        destino = "";
    }
}
inline void asignar_si_valido(int& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = stoi(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}
inline void asignar_si_valido(long long& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = stoll(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}

// Utilidad para limpiar comillas dobles y espacios
inline string limpiar_campo(const string& s) {
    string out = s;
    out.erase(remove(out.begin(), out.end(), '"'), out.end());
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    return out;
}
// Parsear vector<string> desde campo CSV tipo [tag1,tag2,...]
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
// Parsear vector<long long> desde campo CSV tipo [1,2,3]
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

int menu() {
    int opcion = 0;
    cout << "Seleccione la estructura de datos a utilizar:\n";
    cout << "1. Árbol Binario de Búsqueda (BST)\n";
    cout << "2. Tabla Hash\n";
    cout << "3. Vector (opcional)\n";    
    cout << "4. Salir\n";
    cout << "Opción: ";
    cin >> opcion;
    return opcion;
}

int main() {
    int opcion = 0;
    //opcion = menu();
    opcion = 1; // Para pruebas, forzamos a usar BST

    if (opcion == 1) {
        cout << "Cargando datos en BST, espere un momento" << endl;
        // Vector para almacenar las filas de la grilla
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
        vector<ResultadoInsercion> grilla_id;
        vector<ResultadoInsercion> grilla_nombre;
        arbol_binario_id bst_id;
        arbol_binario_nombre bst_nombre;
        ifstream archivo_csv("data.csv");
        if (!archivo_csv.is_open()) {
            cerr << "No se pudo abrir el archivo: data.csv" << endl;
            return 1;
        }
        // Contar total de filas (sin cabecera)
        int total_filas = 0;
        string tmp_linea;

        /* Aquí contamos cuantas filas hay. Esto es solo para comparar la cantidad de registros vs total de filas*/
        getline(archivo_csv, tmp_linea); //lectura forzada para sacar la cabecera del set.
        while (getline(archivo_csv, tmp_linea)){
            total_filas++; //para contar el total
        }   

        /* RESET DATA SET  */
        archivo_csv.clear(); // limpiamos todos los flags del stream.
        archivo_csv.seekg(0); //reseteamos el puntero del set al inicio
        getline(archivo_csv, tmp_linea); // Cabecera de nuevo ya que que comenzamos denuevo la lectura del set.

        /***************************** INSERTAR EN BST POR ID  ****************************/
        // --- NUEVO LÍMITE DE 10,000 INSERCIONES PARA BST POR ID ---
        archivo_csv.clear();
        archivo_csv.seekg(0);
        getline(archivo_csv, tmp_linea); // Cabecera
        int nodos_validos_id = 0;
        vector<Usuario> buffer_usuarios_id;
        buffer_usuarios_id.reserve(5000);
        string linea_csv; // Declaración necesaria para los bucles de inserción
        while (getline(archivo_csv, linea_csv)) {
            // if (nodos_validos_id >= 10000) break; // <--- Comentado para permitir hasta 40000
            vector<string> campos = parsear_csv(linea_csv);
            if (campos.size() < 10) continue;
            Usuario usuario;
            // ID
            string id_limpio = limpiar_campo(campos[0]);
            asignar_si_valido(usuario.id, vector<string>{id_limpio}, 0);
            // Nombre
            usuario.screen_name = limpiar_campo(campos[1]);
            // Tags
            usuario.tags = parsear_tags(campos[2]);
            // Avatar
            usuario.avatar = limpiar_campo(campos[3]);
            // Followers count
            asignar_si_valido(usuario.followers_count, campos, 4);
            // Friends count
            asignar_si_valido(usuario.friends_count, campos, 5);
            // Lang
            usuario.lang = limpiar_campo(campos[6]);
            // Last seen
            asignar_si_valido(usuario.last_seen, campos, 7);
            // Tweet id
            string tweet_id_limpio = limpiar_campo(campos[8]);
            asignar_si_valido(usuario.tweet_id, vector<string>{tweet_id_limpio}, 0);
            // Friends
            usuario.friends = parsear_friends(campos[9]);
            if (usuario.id > 0) {
                buffer_usuarios_id.push_back(usuario);
            }
            if (buffer_usuarios_id.size() == 5000) {
                auto inicio_bloque = chrono::high_resolution_clock::now();
                for (const auto& u : buffer_usuarios_id) {
                    // if (nodos_validos_id >= 10000) break; // <--- Comentado para permitir hasta 40000
                    bst_id.insertar(u);
                    nodos_validos_id++;
                }
                auto fin_bloque = chrono::high_resolution_clock::now();
                auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin_bloque - inicio_bloque);
                grilla_id.push_back({"BST", "inserción", "id", nodos_validos_id, duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
                buffer_usuarios_id.clear();
            }
            if (nodos_validos_id == 40000) break;
        }
        // Si quedan usuarios en el buffer y no se llegó a 10,000 nodos
        if (!buffer_usuarios_id.empty() && nodos_validos_id < 10000) {
            auto inicio_bloque = chrono::high_resolution_clock::now();
            for (const auto& u : buffer_usuarios_id) {
                // if (nodos_validos_id >= 10000) break; // <--- Comentado para permitir hasta 40000
                bst_id.insertar(u);
                nodos_validos_id++;
            }
            auto fin_bloque = chrono::high_resolution_clock::now();
            auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin_bloque - inicio_bloque);
            grilla_id.push_back({"BST", "inserción", "id", nodos_validos_id, duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
        }


        /* RESET DATA SET  */
        archivo_csv.clear();
        archivo_csv.seekg(0);
        getline(archivo_csv, tmp_linea);
        // --- NUEVO LÍMITE DE 10,000 INSERCIONES PARA BST POR NOMBRE ---
        int nodos_validos_nombre = 0;
        vector<Usuario> buffer_usuarios_nombre;
        buffer_usuarios_nombre.reserve(5000);
        while (getline(archivo_csv, linea_csv)) {
            // if (nodos_validos_nombre >= 10000) break; // <--- Comentado para permitir hasta 40000
            vector<string> campos = parsear_csv(linea_csv);
            if (campos.size() < 10) continue;
            Usuario usuario;
            // ID
            string id_limpio = limpiar_campo(campos[0]);
            asignar_si_valido(usuario.id, vector<string>{id_limpio}, 0);
            // Nombre
            usuario.screen_name = limpiar_campo(campos[1]);
            // Tags
            usuario.tags = parsear_tags(campos[2]);
            // Avatar
            usuario.avatar = limpiar_campo(campos[3]);
            // Followers count
            asignar_si_valido(usuario.followers_count, campos, 4);
            // Friends count
            asignar_si_valido(usuario.friends_count, campos, 5);
            // Lang
            usuario.lang = limpiar_campo(campos[6]);
            // Last seen
            asignar_si_valido(usuario.last_seen, campos, 7);
            // Tweet id
            string tweet_id_limpio = limpiar_campo(campos[8]);
            asignar_si_valido(usuario.tweet_id, vector<string>{tweet_id_limpio}, 0);
            // Friends
            usuario.friends = parsear_friends(campos[9]);
            if (!usuario.screen_name.empty()) {
                buffer_usuarios_nombre.push_back(usuario);
            }
            if (buffer_usuarios_nombre.size() == 5000) {
                auto inicio_bloque = chrono::high_resolution_clock::now();
                for (const auto& u : buffer_usuarios_nombre) {
                    // if (nodos_validos_nombre >= 10000) break; // <--- Comentado para permitir hasta 40000
                    bst_nombre.insertar(u);
                    nodos_validos_nombre++;
                }
                auto fin_bloque = chrono::high_resolution_clock::now();
                auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin_bloque - inicio_bloque);
                grilla_nombre.push_back({"BST", "inserción", "nombre", nodos_validos_nombre, duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
                buffer_usuarios_nombre.clear();
            }
            if (nodos_validos_nombre == 40000) break;
        }
        if (!buffer_usuarios_nombre.empty()) {
            auto inicio_bloque = chrono::high_resolution_clock::now();
            for (const auto& u : buffer_usuarios_nombre) {
                bst_nombre.insertar(u);
                nodos_validos_nombre++;
            }
            auto fin_bloque = chrono::high_resolution_clock::now();
            auto duracion_ns = chrono::duration_cast<chrono::nanoseconds>(fin_bloque - inicio_bloque);
            grilla_nombre.push_back({"BST", "inserción", "nombre", nodos_validos_nombre, duracion_ns.count() / 1e9, duracion_ns.count() / 1000000, duracion_ns.count() / 1000, duracion_ns.count()});
        }


        cout << "\nINSERCIÓN BST POR ID:";
        // Mostrar grilla inicial con formato actualizado
        cout << "\n+------+------------+---------------------+--------------------+------------+---------------+---------------+---------------+" << endl;
        cout << "| ED   | Operación  | Target              | Nodos creados      | Segundos   | Milisegundos  | Microsegundos | Nanosegundos  |" << endl;
        cout << "+------+------------+---------------------+--------------------+------------+---------------+---------------+---------------+" << endl;
        for (const auto& r : grilla_id) {
            cout << "| " << setw(4) << left << r.ed
                 << " | " << setw(10) << left << r.operacion
                 << " | " << setw(19) << left << r.target
                 << " | " << setw(18) << left << r.nodos
                 << " | " << setw(10) << left << fixed << setprecision(6) << r.segundos
                 << " | " << setw(13) << left << r.milisegundos
                 << " | " << setw(13) << left << r.microsegundos
                 << " | " << setw(13) << left << r.nanosegundos
                 << " |" << endl;
        }
        cout << "+------+------------+---------------------+--------------------+------------+---------------+---------------+---------------+" << endl;

        cout << "\nINSERCIÓN BST POR NOMBRE:";
        cout << "\n+------+------------+---------------------+--------------------+------------+---------------+---------------+---------------+" << endl;
        cout << "| ED   | Operación  | Target              | Nodos creados      | Segundos   | Milisegundos  | Microsegundos | Nanosegundos  |" << endl;
        cout << "+------+------------+---------------------+--------------------+------------+---------------+---------------+---------------+" << endl;
        for (const auto& r : grilla_nombre) {
            cout << "| " << setw(4) << left << r.ed
                 << " | " << setw(10) << left << r.operacion
                 << " | " << setw(19) << left << r.target
                 << " | " << setw(18) << left << r.nodos
                 << " | " << setw(10) << left << fixed << setprecision(6) << r.segundos
                 << " | " << setw(13) << left << r.milisegundos
                 << " | " << setw(13) << left << r.microsegundos
                 << " | " << setw(13) << left << r.nanosegundos
                 << " |" << endl;
        }
        cout << "+------+------------+---------------------+--------------------+------------+---------------+---------------+---------------+" << endl;

        // Al final del main, imprimir los primeros 5 registros del BST por ID y por nombre en formato de grilla
        cout << "\nPrimeros 5 registros del BST por ID (in-order):" << endl;
        cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+---------------------+" << endl;
        cout << "| id                  | nombre                   | tags (cantidad)   | avatar              | followers_count | friends_count   | lang     | last_seen       | tweet_id            |" << endl;
        cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+---------------------+" << endl;
        int contador_id = 0;
        std::function<void(nodo_arbol_id*)> imprimir_id;
        imprimir_id = [&](nodo_arbol_id* nodo) {
            if (!nodo || contador_id >= 5) return;
            imprimir_id(nodo->izquierdo);
            if (contador_id < 5) {
                string avatar_corto = nodo->usuario.avatar.length() > 15 ? nodo->usuario.avatar.substr(0, 15) + "..." : nodo->usuario.avatar;
                cout << "| " << setw(19) << left << nodo->usuario.id
                     << " | " << setw(24) << left << nodo->usuario.screen_name
                     << " | " << setw(17) << left << (nodo->usuario.tags.empty() ? "0" : to_string(nodo->usuario.tags.size()))
                     << " | " << setw(19) << left << avatar_corto
                     << " | " << setw(15) << left << nodo->usuario.followers_count
                     << " | " << setw(15) << left << nodo->usuario.friends_count
                     << " | " << setw(8) << left << nodo->usuario.lang
                     << " | " << setw(15) << left << nodo->usuario.last_seen
                     << " | " << setw(15) << left << nodo->usuario.tweet_id
                     << " |" << endl;
                contador_id++;
            }
            imprimir_id(nodo->derecho);
        };
        imprimir_id(bst_id.raiz);
        cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+---------------------+" << endl;

        cout << "\nPrimeros 5 registros del BST por NOMBRE (in-order):" << endl;
        cout << "+--------------------------+---------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+---------------------+" << endl;
        cout << "| nombre                   | id                  | tags (cantidad)   | avatar              | followers_count | friends_count   | lang     | last_seen       | tweet_id            |" << endl;
        cout << "+--------------------------+---------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+---------------------+" << endl;
        int contador_nombre = 0;
        std::function<void(nodo_arbol_nombre*)> imprimir_nombre;
        imprimir_nombre = [&](nodo_arbol_nombre* nodo) {
            if (!nodo || contador_nombre >= 5) return;
            imprimir_nombre(nodo->izquierdo);
            if (contador_nombre < 5) {
                string avatar_corto_nombre = nodo->usuario.avatar.length() > 15 ? nodo->usuario.avatar.substr(0, 15) + "..." : nodo->usuario.avatar;
                cout << "| " << setw(24) << left << nodo->usuario.screen_name
                     << " | " << setw(19) << left << nodo->usuario.id
                     << " | " << setw(17) << left << (nodo->usuario.tags.empty() ? "0" : to_string(nodo->usuario.tags.size()))
                     << " | " << setw(19) << left << avatar_corto_nombre
                     << " | " << setw(15) << left << nodo->usuario.followers_count
                     << " | " << setw(15) << left << nodo->usuario.friends_count
                     << " | " << setw(8) << left << nodo->usuario.lang
                     << " | " << setw(15) << left << nodo->usuario.last_seen
                     << " | " << setw(15) << left << nodo->usuario.tweet_id
                     << " |" << endl;
                contador_nombre++;
            }
            imprimir_nombre(nodo->derecho);
        };
        imprimir_nombre(bst_nombre.raiz);
        cout << "+--------------------------+---------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+---------------------+" << endl;

        // Menú de búsqueda en bucle
        while (true) {
            int opcion_busqueda = 0;
            cout << "\nBÚSQUEDA:" << endl;
            //cout << "\n¿Qué tipo de búsqueda desea realizar?" << endl;
            cout << "1. Buscar por id" << endl;
            cout << "2. Buscar por nombre" << endl;
            cout << "3. Salir" << endl;
            cout << "Opción: ";
            cin >> opcion_busqueda;
            tuple<string, string, string, int, double> resultado_busqueda;
            bool hizo_busqueda = false;
            if (opcion_busqueda == 1) {
                long long id_buscar;
                cout << "\nIngrese ID: ";
                cin >> id_buscar;
                auto inicio_busqueda_id = chrono::high_resolution_clock::now();
                bool existe = bst_id.buscar_por_id(id_buscar);
                auto fin_busqueda_id = chrono::high_resolution_clock::now();
                auto duracion_busqueda_id = chrono::duration_cast<chrono::nanoseconds>(fin_busqueda_id - inicio_busqueda_id);
                if (existe) {
                    cout << "\nEncontrado: SI" << endl;
                } else {
                    cout << "\nEncontrado: NO" << endl;
                }
                Usuario* usuario = bst_id.buscar_usuario([&](const Usuario& u){ return u.id == id_buscar; });
                cout << "\nResultado de búsqueda por ID:" << endl;
                cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+-----------------+" << endl;
                cout << "| id                  | nombre                   | tags (cantidad)   | avatar              | followers_count | friends_count   | lang     | last_seen       | tweet_id        |" << endl;
                cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+-----------------+" << endl;
                if (usuario) {
                    string avatar_corto = usuario->avatar.length() > 15 ? usuario->avatar.substr(0, 15) + "..." : usuario->avatar;
                    cout << "| " << setw(19) << left << usuario->id
                         << " | " << setw(24) << left << usuario->screen_name
                         << " | " << setw(17) << left << (usuario->tags.empty() ? "0" : to_string(usuario->tags.size()))
                         << " | " << setw(19) << left << avatar_corto
                         << " | " << setw(15) << left << usuario->followers_count
                         << " | " << setw(15) << left << usuario->friends_count
                         << " | " << setw(8) << left << usuario->lang
                         << " | " << setw(15) << left << usuario->last_seen
                         << " | " << setw(15) << left << usuario->tweet_id
                         << " |" << endl;
                } else {
                    cout << "| No encontrado                                                                                                                      |" << endl;
                }
                cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+-----------------+" << endl;
                cout << "Tiempo: " << duracion_busqueda_id.count() << " ns" << endl;
            } else if (opcion_busqueda == 2) {
                string nombre_buscar;
                cout << "\nIngrese NOMBRE: ";
                cin >> nombre_buscar;
                nombre_buscar.erase(std::remove(nombre_buscar.begin(), nombre_buscar.end(), '"'), nombre_buscar.end());
                auto inicio_busqueda_nombre = chrono::high_resolution_clock::now();
                bool existe = bst_nombre.buscar_por_nombre(nombre_buscar);
                auto fin_busqueda_nombre = chrono::high_resolution_clock::now();
                auto duracion_busqueda_nombre = chrono::duration_cast<chrono::nanoseconds>(fin_busqueda_nombre - inicio_busqueda_nombre);
                if (existe) {
                    cout << "\nEncontrado: SI" << endl;
                } else {
                    cout << "\nEncontrado: NO" << endl;
                }
                Usuario* usuario = bst_nombre.buscar_usuario([&](const Usuario& u){ return u.screen_name == nombre_buscar; });
                cout << "\nResultado de búsqueda por NOMBRE:" << endl;
                cout << "+--------------------------+---------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+-----------------+" << endl;
                cout << "| nombre                   | id                  | tags (cantidad)   | avatar              | followers_count | friends_count   | lang     | last_seen       | tweet_id        |" << endl;
                cout << "+---------------------+--------------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+-----------------+" << endl;
                if (usuario) {
                    string avatar_corto = usuario->avatar.length() > 15 ? usuario->avatar.substr(0, 15) + "..." : usuario->avatar;
                    cout << "| " << setw(24) << left << usuario->screen_name
                         << " | " << setw(19) << left << usuario->id
                         << " | " << setw(17) << left << (usuario->tags.empty() ? "0" : to_string(usuario->tags.size()))
                         << " | " << setw(19) << left << avatar_corto
                         << " | " << setw(15) << left << usuario->followers_count
                         << " | " << setw(15) << left << usuario->friends_count
                         << " | " << setw(8) << left << usuario->lang
                         << " | " << setw(15) << left << usuario->last_seen
                         << " | " << setw(15) << left << usuario->tweet_id
                         << " |" << endl;
                } else {
                    cout << "| No encontrado                                                                                                                      |" << endl;
                }
                cout << "+--------------------------+---------------------+-------------------+---------------------+-----------------+-----------------+----------+-----------------+-----------------+" << endl;
                cout << "Tiempo: " << duracion_busqueda_nombre.count() << " ns" << endl;
            } else {
                break;
            }
            // Mostrar solo el resultado de la búsqueda actual
            if (hizo_busqueda) {
                cout << "\n+-----+--------------+---------+-----------------+---------------------+" << endl;
                cout << "| ED  | Operación    | Target  | Nodos afectados | Tiempo (segundos)   |" << endl;
                cout << "+-----+--------------+---------+-----------------+---------------------+" << endl;
                cout << "| " << setw(3) << left << get<0>(resultado_busqueda)
                     << " | " << setw(13) << left << get<1>(resultado_busqueda)
                     << " | " << setw(7) << left << get<2>(resultado_busqueda)
                     << " | " << setw(15) << left << get<3>(resultado_busqueda)
                     << " | " << setw(19) << left << fixed << setprecision(6) << get<4>(resultado_busqueda)
                     << " |" << endl;
                cout << "+-----+--------------+---------+-----------------+---------------------+" << endl;
            }
        }

        
        
    } else if (opcion == 2) {
        cout << "Trabajando con Tabla Hash" << endl;
        // Aquí irá la lógica para Hash
    } else if (opcion == 3) {
        cout << "Trabajando con Vector" << endl;
        vector<Usuario> usuarios = leer_usuarios_desde_csv("data.csv");
        cout << "Usuarios cargados en vector: " << usuarios.size() << endl;
        // Aquí puedes agregar más lógica para trabajar con el vector
    } else {
        cout << "Opción no válida." << endl;
        return 1;
    }
    return 0;
}
