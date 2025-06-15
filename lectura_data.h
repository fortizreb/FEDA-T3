#ifndef LECTURA_DATA_H
#define LECTURA_DATA_H

#include <vector>
#include <string>
#include "usuario.h"
using namespace std;

vector<Usuario> leer_usuarios_desde_csv(const string& rutaArchivo);
vector<string> parsear_csv(const string& linea);

#endif 
