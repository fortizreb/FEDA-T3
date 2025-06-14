#include "lecturaData.h"
#include "arbolBinario.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;


/* Función para parsear una línea CSV, considerando comillas y corchetes */
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

vector<Usuario> leer_usuarios_desde_csv(const string& ruta_archivo) {
    vector<Usuario> usuarios;
    ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << ruta_archivo << endl;
        return usuarios;
    }
    // Contar líneas totales (usuarios)
    string tmp;
    getline(archivo, tmp); // Cabecera
    string linea;

    int contador = 0;

    while (getline(archivo, linea) && contador < 10) {
        /* vector campos:
            0: id
            1: screen_name
            2: tags
            3: avatar
            4: followers_count
            5: friends_count    
            6: lang
            7: last_seen
            8: tweet_id
            9: friends
        */
        vector<string> campos = parsear_csv(linea);

        if (campos.size() < 10) continue; //asegurarse de que hay suficientes campos
        Usuario u;
        try { u.id = stoll(campos[0]); } catch (...) { u.id = 0; }
        u.screen_name = campos[1];
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
        try { u.followers_count = stoi(campos[4]); } catch (...) { u.followers_count = 0; }
        try { u.friends_count = stoi(campos[5]); } catch (...) { u.friends_count = 0; }
        u.lang = campos[6];
        try { u.last_seen = stoll(campos[7]); } catch (...) { u.last_seen = 0; }
        try { u.tweet_id = stoll(campos[8]); } catch (...) { u.tweet_id = 0; }
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


void leer_usuarios_en_bst(const string& ruta_archivo, Arbol_Binario& bst) {
    ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo: " << ruta_archivo << endl;
        return;
    }
    string tmp;
    getline(archivo, tmp); // Cabecera
    string linea;
    //int contador = 0;
   // && contador < 10
    while (getline(archivo, linea) ) {
        /* vector campos:
            0: id
            1: screen_name
            2: tags
            3: avatar
            4: followers_count
            5: friends_count    
            6: lang
            7: last_seen
            8: tweet_id
            9: friends
        */
        vector<string> campos = parsear_csv(linea); 
   
        if (campos.size() < 10) continue;
        Usuario u;
        try { u.id = stoll(campos[0]); } catch (...) { u.id = 0; }
        u.screen_name = campos[1];
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
        try { u.followers_count = stoi(campos[4]); } catch (...) { u.followers_count = 0; }
        try { u.friends_count = stoi(campos[5]); } catch (...) { u.friends_count = 0; }
        u.lang = campos[6];
        try { u.last_seen = stoll(campos[7]); } catch (...) { u.last_seen = 0; }
        try { u.tweet_id = stoll(campos[8]); } catch (...) { u.tweet_id = 0; }
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
        bst.insertar(u);
        //contador++;
    }
}
