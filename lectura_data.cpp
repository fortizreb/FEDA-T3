#include "lectura_data.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// Copia aquí la implementación de tus funciones desde lecturaData.cpp si es necesario.

vector<Usuario> leer_usuarios_desde_csv(const string& rutaArchivo) {
    // Copia aquí la implementación original de leerUsuariosDesdeCsv si es necesario
    // ...
    return {};
}

vector<string> parsear_csv(const string& linea) {
    vector<string> campos;
    string campo;
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
