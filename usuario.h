#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>

using namespace std;

struct Usuario {
    /*
    [0]id: identificador (pk por si sola)
    [1]screen_name: nombre  (pk por si sola)
    [2]tags: etiquetas (array de strings)
    [3]avatar: url del avatar
    [4]followers_count: seguidores
    [5]friends_count: amigos
    [6]lang: idioma
    [7]last_seen: fecha de ultima vez que se vio
    [8]tweet_id: id del ultimo tweet
    [9]friends: array de id de amigos
    */

    long long id;
    string screen_name; 
    vector<string> tags; 
    string avatar; 
    int followers_count; 
    int friends_count; 
    string lang; 
    long long last_seen; 
    long long tweet_id; 
    vector<long long> friends; 
};

inline void limpiar_usuario(Usuario& u) {
    u.id = 0;
    u.screen_name = "";
    u.tags.clear();
    u.avatar = "";
    u.followers_count = 0;
    u.friends_count = 0;
    u.lang = "";
    u.last_seen = 0;
    u.tweet_id = 0;
    u.friends.clear();
}

#endif // USUARIO_H
