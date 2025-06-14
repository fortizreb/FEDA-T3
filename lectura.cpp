#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Usuario {
    int id; 
    string nombre_usuario;
};

int main() {
    ifstream archivo("data.csv");

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string linea;
    int contador = 0;

    while (getline(archivo, linea) && contador < 5) {
        stringstream ss(linea);
        string campo;

        // id
        getline(ss, campo, ',');
        // Eliminamos comillas
        campo.erase(remove(campo.begin(), campo.end(), '"'), campo.end()); 
        if (!campo.empty() && all_of(campo.begin(), campo.end(), ::isdigit)) {
            int id = stoi(campo);

            // nombre_usuario
            getline(ss, campo, ',');
            campo.erase(remove(campo.begin(), campo.end(), '"'), campo.end());
            string nombre_usuario = campo;

            Usuario u = { id, nombre_usuario };
            cout << "Usuario " << contador + 1 << ": ID = " << u.id << ", nombre_usuario = " << u.nombre_usuario << endl;

            contador++;
        } else {
            // Manejar el error o valor no numérico
            cerr << "ID no válido en la línea: " << linea << endl;
        }
    }

    archivo.close();
    return 0;
}
