#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>

struct Usuario {
    long long id;
    std::string screen_name;
    std::vector<std::string> tags;
    std::string avatar;
    int followers_count;
    int friends_count;
    std::string lang;
    long long last_seen;
    long long tweet_id;
    std::vector<long long> friends;
};

struct resultado_insercion {
    std::string estructura_datos;
    std::string operacion;
    std::string objetivo;
    int cantidad_nodos;
    double segundos;
    long long milisegundos;
    long long microsegundos;
    long long nanosegundos;
};

struct resultado_busqueda {
    int cantidad_usuarios;
    std::string clave;
    std::string tipo_busqueda;
    double tiempo_ns;
};

#endif 