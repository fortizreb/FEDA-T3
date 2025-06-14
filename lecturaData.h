#ifndef LECTURA_DATA_H
#define LECTURA_DATA_H

#include <vector>
#include <string>
#include <fstream>
#include "usuario.h"
#include "arbolBinario.h"
using namespace std;

std::vector<Usuario> leer_usuarios_desde_csv(const string& ruta_archivo);
void leer_usuarios_en_bst(const string& ruta_archivo, Arbol_Binario& bst);
std::vector<string> parsear_csv(const string& linea);

#endif // LECTURA_DATA_H
