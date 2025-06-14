#include "usuario.h"
#include "lecturaData.h"
#include "arbol_binario_id.h"
#include "arbol_binario_nombre.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip> //para las funciones setw y setprecision de la grilla donde se muestran los resultados
#include <algorithm>
#include <vector>
#include <tuple>
using namespace std;

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
        vector<tuple<string, string, string, int, double>> grilla;
        Arbol_Binario_Id bst_id;
        Arbol_Binario_Nombre bst_nombre;
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
        int nodos_validos_id = 0;
        string linea_csv;
        auto inicio_id = chrono::high_resolution_clock::now(); //start chrono
        while (getline(archivo_csv, linea_csv)) {
            vector<string> campos = parsear_csv(linea_csv); //esta función la traigo de lecturaData.cpp
            
            if (campos.size() < 10) continue;
            Usuario usuario;
            
            string id_limpio = campos[0];  //quitar commillas dobles de id.
            id_limpio.erase(std::remove(id_limpio.begin(), id_limpio.end(), '"'), id_limpio.end());

            // Limpiar comillas dobles del campo screen_name
            string nombre_limpio = campos[1];
            nombre_limpio.erase(std::remove(nombre_limpio.begin(), nombre_limpio.end(), '"'), nombre_limpio.end());
            usuario.screen_name = nombre_limpio;
            
            try { usuario.id = stoll(id_limpio); } catch (...) { usuario.id = 0; }// Asignar el id_limpio
            
            if (usuario.id > 0) {
                nodos_validos_id++;
                bst_id.insertar(usuario);
            }
        }
        auto fin_id = chrono::high_resolution_clock::now(); //stop chrono
        chrono::duration<double> duracion_id = fin_id - inicio_id;
        grilla.push_back({"BST", "inserción", "id", nodos_validos_id, duracion_id.count()});


        /* RESET DATA SET  */
        archivo_csv.clear();
        archivo_csv.seekg(0);
        getline(archivo_csv, tmp_linea);

        /************************** INSERTAR EN BST POR NOMBRE_USUARIO ****************************/
        int nodos_validos_nombre = 0;
        auto inicio_nombre = chrono::high_resolution_clock::now(); //start chrono
        while (getline(archivo_csv, linea_csv)) {
            vector<string> campos = parsear_csv(linea_csv);
            if (campos.size() < 10) continue;
            Usuario usuario;
            usuario.screen_name = campos[1];
            // Limpiar comillas dobles del campo ID
            string id_limpio = campos[0];
            id_limpio.erase(std::remove(id_limpio.begin(), id_limpio.end(), '"'), id_limpio.end());
            try { usuario.id = stoll(id_limpio); } catch (...) { usuario.id = 0; }
            // ...parseo de campos igual que en lecturaData.cpp...
            if (!usuario.screen_name.empty()) {
                nodos_validos_nombre++;
                bst_nombre.insertar(usuario);
            }
        }
        auto fin_nombre = chrono::high_resolution_clock::now(); //stop chrono
        chrono::duration<double> duracion_nombre = fin_nombre - inicio_nombre;


        
        cout << "\nINSERCIÓN:";
        
        grilla.push_back({"BST", "inserción", "nombre", nodos_validos_nombre, duracion_nombre.count()});

        // Mostrar grilla inicial con formato actualizado
        cout << "\n+-----+--------------+---------+-----------------+---------------------+" << endl;
        cout << "| ED  | Operación    | Target  | Nodos afectados | Tiempo (segundos)   |" << endl;
        cout << "+-----+--------------+---------+-----------------+---------------------+" << endl;
        cout << "| " << setw(3) << left << "BST"
             << " | " << setw(13) << left << "inserción"
             << " | " << setw(7) << left << "id"
             << " | " << setw(15) << left << nodos_validos_id
             << " | " << setw(19) << left << fixed << setprecision(6) << duracion_id.count()
             << " |" << endl;
        cout << "| " << setw(3) << left << "BST"
             << " | " << setw(13) << left << "inserción"
             << " | " << setw(7) << left << "nombre"
             << " | " << setw(15) << left << nodos_validos_nombre
             << " | " << setw(19) << left << fixed << setprecision(6) << duracion_nombre.count()
             << " |" << endl;
        cout << "+-----+--------------+---------+-----------------+---------------------+" << endl;

        

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
                auto inicio_busqueda = chrono::high_resolution_clock::now();
                bool encontrado = bst_id.buscar_por_id(id_buscar);
                auto fin_busqueda = chrono::high_resolution_clock::now();
                chrono::duration<double> duracion_busqueda = fin_busqueda - inicio_busqueda;
                resultado_busqueda = {"BST", "búsqueda", "id", encontrado ? 1 : 0, duracion_busqueda.count()};
                hizo_busqueda = true;
                if (encontrado) {
                    cout << "\nEncontrado: SI" << endl;
                } else {
                    cout << "\nEncontrado: NO" << endl;
                }
            } else if (opcion_busqueda == 2) {
                string nombre_buscar;
                cout << "\nIngrese NOMBRE: ";
                cin >> nombre_buscar;
                // Limpiar comillas dobles del nombre a buscar
                nombre_buscar.erase(std::remove(nombre_buscar.begin(), nombre_buscar.end(), '"'), nombre_buscar.end());
                auto inicio_busqueda = chrono::high_resolution_clock::now();
                bool encontrado = bst_nombre.buscar_por_nombre(nombre_buscar);
                auto fin_busqueda = chrono::high_resolution_clock::now();
                chrono::duration<double> duracion_busqueda = fin_busqueda - inicio_busqueda;
                resultado_busqueda = {"BST", "búsqueda", "nombre", encontrado ? 1 : 0, duracion_busqueda.count()};
                hizo_busqueda = true;
                if (encontrado) {
                    cout << "\nEncontrado: SI" << endl;
                } else {
                    cout << "\nEncontrado: NO" << endl;
                }
            } else {
                //cout << "Saliendo del menú de búsqueda." << endl;
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

        // Al final del main, imprimir los primeros 5 registros del BST por ID
        cout << "\nPrimeros 5 registros del BST por ID:" << endl;
        bst_id.imprimir_primeros_n(5);
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
