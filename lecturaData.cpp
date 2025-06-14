#include "lecturaData.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// Función auxiliar para parsear una línea CSV respetando comillas y corchetes
static vector<string> parsear_csv(const string& linea) {
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

vector<Usuario> leerUsuariosDesdeCsv(const string& rutaArchivo) {
    vector<Usuario> usuarios;
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << rutaArchivo << endl;
        return usuarios;
    }
    // Contar líneas totales (usuarios)
    string tmp;
    getline(archivo, tmp); // Cabecera
    string linea;

    int contador = 0;

    while (getline(archivo, linea) && contador < 10) {
        vector<string> campos = parsear_csv(linea);
        if (campos.size() != 10) continue;
        Usuario u;
        try { u.id = stoll(campos[0]); } catch (...) { u.id = 0; }
        u.screenName = campos[1];
        // tags
        u.tags.clear();
        if (campos[2].size() > 2) {
            string tags = campos[2].substr(1, campos[2].size()-2);
            stringstream sstags(tags);
            string tag;
            while (getline(sstags, tag, ',')) {
                tag.erase(remove(tag.begin(), tag.end(), '"'), tag.end());
                tag.erase(0, tag.find_first_not_of(" "));
                tag.erase(tag.find_last_not_of(" ")+1);
                if (!tag.empty()) u.tags.push_back(tag);
            }
        }
        u.avatar = campos[3];
        try { u.followersCount = stoi(campos[4]); } catch (...) { u.followersCount = 0; }
        try { u.friendsCount = stoi(campos[5]); } catch (...) { u.friendsCount = 0; }
        u.lang = campos[6];
        try { u.lastSeen = stoll(campos[7]); } catch (...) { u.lastSeen = 0; }
        try { u.tweetId = stoll(campos[8]); } catch (...) { u.tweetId = 0; }
        // friends
        u.friends.clear();
        if (campos[9].size() > 2) {
            string friends = campos[9].substr(1, campos[9].size()-2);
            stringstream ssfriends(friends);
            string fid;
            while (getline(ssfriends, fid, ',')) {
                fid.erase(remove(fid.begin(), fid.end(), '"'), fid.end());
                fid.erase(0, fid.find_first_not_of(" "));
                fid.erase(fid.find_last_not_of(" ")+1);
                try { u.friends.push_back(stoll(fid)); } catch (...) {}
            }
        }
        usuarios.push_back(u);

        contador++;
    }
    return usuarios;
}
