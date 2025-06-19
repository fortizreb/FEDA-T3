#include "usuario.h"
#include "arbol/arboles.h"
#include "utilitarios.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

extern int LIMITE_USUARIOS;

// Funciones de parseo y limpieza
string limpiar_campo(const string& s) {
    string out = s;
    out.erase(remove(out.begin(), out.end(), '"'), out.end());
    out.erase(remove(out.begin(), out.end(), ' '), out.end());
    return out;
}

vector<string> parsear_tags(const string& campo) {
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

vector<long long> parsear_friends(const string& campo) {
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

Usuario parsear_usuario(const vector<string>& campos) {
    Usuario usuario;
    string id_limpio = limpiar_campo(campos[0]);
    usuario.id = (!id_limpio.empty() && all_of(id_limpio.begin(), id_limpio.end(), ::isdigit)) ? stoll(id_limpio) : 0;
    usuario.screen_name = limpiar_campo(campos[1]);
    usuario.tags = parsear_tags(campos[2]);
    usuario.avatar = limpiar_campo(campos[3]);
    usuario.followers_count = (campos.size() > 4 && !campos[4].empty() && all_of(campos[4].begin(), campos[4].end(), ::isdigit)) ? stoi(campos[4]) : 0;
    usuario.friends_count = (campos.size() > 5 && !campos[5].empty() && all_of(campos[5].begin(), campos[5].end(), ::isdigit)) ? stoi(campos[5]) : 0;
    usuario.lang = limpiar_campo(campos[6]);
    usuario.last_seen = (campos.size() > 7 && !campos[7].empty() && all_of(campos[7].begin(), campos[7].end(), ::isdigit)) ? stoll(campos[7]) : 0;
    string tweet_id_limpio = limpiar_campo(campos[8]);
    usuario.tweet_id = (!tweet_id_limpio.empty() && all_of(tweet_id_limpio.begin(), tweet_id_limpio.end(), ::isdigit)) ? stoll(tweet_id_limpio) : 0;
    usuario.friends = parsear_friends(campos[9]);
    return usuario;
}

void parsear_campos_csv(const string& linea, vector<string>& campos, size_t esperado) {
    campos.clear();
    campos.reserve(esperado);
    size_t start = 0, end = 0;
    while ((end = linea.find(',', start)) != string::npos && campos.size() < esperado - 1) {
        campos.push_back(linea.substr(start, end - start));
        start = end + 1;
    }
    campos.push_back(linea.substr(start));
}

// Funciones auxiliares para asignar valores si son válidos
void asignar_si_valido_string(string& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx && !campos[idx].empty() && campos[idx] != "\"\"" && campos[idx] != "null") {
        destino = campos[idx];
    } else {
        destino = "";
    }
}
void asignar_si_valido_int(int& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = stoi(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}
void asignar_si_valido_longlong(long long& destino, const vector<string>& campos, size_t idx) {
    if (campos.size() > idx) {
        try { destino = stoll(campos[idx]); } catch (...) { destino = 0; }
    } else {
        destino = 0;
    }
}

vector<Usuario> leer_usuarios_validos_csv(const string& archivo) {
    vector<Usuario> usuarios_validos;
    ifstream archivo_csv(archivo);
    if (!archivo_csv.is_open()) return usuarios_validos;
    string tmp_linea, linea_csv;
    getline(archivo_csv, tmp_linea); // Cabecera
    while (getline(archivo_csv, linea_csv)) {
        vector<string> campos;
        parsear_campos_csv(linea_csv, campos, 10);
        if (campos.size() < 10) continue;
        Usuario usuario = parsear_usuario(campos);
        if (usuario.id > 0 && !usuario.screen_name.empty()) usuarios_validos.push_back(usuario);
        if (static_cast<int>(usuarios_validos.size()) >= LIMITE_USUARIOS) break;
    }
    return usuarios_validos;
}
