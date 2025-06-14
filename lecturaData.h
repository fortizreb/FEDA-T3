#ifndef LECTURA_DATA_H
#define LECTURA_DATA_H

#include <vector>
#include "usuario.h"
using namespace std;

vector<Usuario> leerUsuariosDesdeCsv(const string& rutaArchivo);

#endif // LECTURA_DATA_H
