#pragma once

#include <string>

using namespace std;

class FavoritesManager {
public:
    void addFavorite(string userLogin);

    void removeFavorite(string userLogin);

    void showFavorites(string userLogin);
};
