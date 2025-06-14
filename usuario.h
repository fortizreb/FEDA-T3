#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include <vector>

using namespace std;

struct Usuario {
    long long id;
    string screenName;
    vector<string> tags;
    string avatar;
    int followersCount;
    int friendsCount;
    string lang;
    long long lastSeen;
    long long tweetId;
    vector<long long> friends;
};

#endif // USUARIO_H
