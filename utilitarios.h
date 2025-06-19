#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include "arboles.h"
#include "usuario.h"
#include <vector>
#include <string>
using namespace std;

// Exporta los resultados de inserción a un archivo CSV (guarda los tiempos y datos)
void exportar_resultados_insercion_csv(const vector<resultado_insercion>& grilla_id, const vector<resultado_insercion>& grilla_nombre);
// Exporta los resultados de búsqueda a un archivo CSV
void exportar_resultados_busqueda_csv(const vector<resultado_busqueda>& resultados);

// Hace búsquedas en ambos árboles (por id y por nombre) y guarda los resultados
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const vector<Usuario>& usuarios_validos);

vector<Usuario> leer_usuarios_validos_csv(const string& archivo);// lee usuarios válidos desde un archivo CSV y los retorna en un vector
// Inserta usuarios en el árbol por ID, mide tiempos y guarda resultados
void insertar_bst_id(arbol_binario_id& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);
// Inserta usuarios en el árbol por nombre, mide tiempos y guarda resultados
void insertar_bst_nombre(arbol_binario_nombre& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);

// Separa los campos de una línea CSV en un vector de strings
void parsear_campos_csv(const string& linea, vector<string>& campos, size_t cantidad_campos);
// Crea un usuario a partir de los campos de un CSV
Usuario parsear_usuario(const vector<string>& campos);

#endif
