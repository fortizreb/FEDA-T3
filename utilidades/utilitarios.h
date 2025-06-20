#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include "arbol/arboles.h"
#include "utilidades/usuario.h"
#include "hash/hash_abierto.h"
#include "hash/hash_cerrado.h"
#include <vector>
#include <string>
using namespace std;

// Exporta los resultados de inserción a un archivo CSV (guarda los tiempos y datos)
void exportar_resultados_insercion_csv(const vector<resultado_insercion>& grilla_id, const vector<resultado_insercion>& grilla_nombre);
// Exporta los resultados de búsqueda a un archivo CSV
void exportar_resultados_busqueda_csv(const vector<resultado_busqueda>& resultados);
// Exporta resultados de inserción o búsqueda con prefijo personalizado y tipo (para bst y hash, con variante opcional)
void exportar_resultados_csv(const std::vector<resultado_insercion>& resultados, const std::string& estructura, const std::string& tipo, const std::string& variante = "");

// Hace búsquedas en ambos árboles (por id y por nombre) y guarda los resultados
void busqueda_bst(const arbol_binario_id& bst_id, const arbol_binario_nombre& bst_nombre, const vector<Usuario>& usuarios_validos);

vector<Usuario> leer_usuarios_validos_csv(const string& archivo);// lee usuarios válidos desde un archivo CSV y los retorna en un vector
// Inserta usuarios en el árbol por ID, mide tiempos y guarda resultados
void insertar_bst_id(arbol_binario_id& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);
// Inserta usuarios en el árbol por nombre, mide tiempos y guarda resultados
void insertar_bst_nombre(arbol_binario_nombre& arbol, const string& archivo, vector<resultado_insercion>& grilla, double& tiempo_lectura, double& tiempo_insercion);

// Inserta usuarios en hash abierto y mide tiempos
void insertar_hash_abierto(HashAbierto& hash, const std::vector<Usuario>& usuarios, std::vector<resultado_insercion>& grilla, double& tiempo_insercion);
// Inserta usuarios en hash cerrado y mide tiempos
void insertar_hash_cerrado(HashCerrado& hash, const std::vector<Usuario>& usuarios, std::vector<resultado_insercion>& grilla, double& tiempo_insercion);

// Busca usuario por id en hash abierto
bool buscar_hash_abierto_id(const HashAbierto& hash, long long id);
// Busca usuario por id en hash cerrado
bool buscar_hash_cerrado_id(const HashCerrado& hash, long long id);
// Busca usuario por nombre en hash abierto
bool buscar_hash_abierto_nombre(const HashAbierto& hash, const std::string& nombre);
// Busca usuario por nombre en hash cerrado
bool buscar_hash_cerrado_nombre(const HashCerrado& hash, const std::string& nombre);

// Separa los campos de una línea CSV en un vector de strings
void parsear_campos_csv(const string& linea, vector<string>& campos, size_t cantidad_campos);
// Crea un usuario a partir de los campos de un CSV
Usuario parsear_usuario(const vector<string>& campos);

// Exporta resultados de inserción en hash a un CSV
void exportar_resultados_insercion_hash_csv(const std::vector<resultado_insercion>& grilla, const std::string& tipo_hash);
// Exporta resultados de búsqueda en hash a un CSV
void exportar_resultados_busqueda_hash_csv(const std::vector<resultado_busqueda>& resultados, const std::string& tipo_hash);

#endif
