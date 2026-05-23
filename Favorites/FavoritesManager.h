#pragma once

#include <string>

using namespace std;

class FavoritesManager {
public:
    void addFavorite();

    void addFavorite(string userLogin);

    void removeFavorite();

    void removeFavorite(string userLogin);

    void showFavorites();

    void showFavorites(string userLogin);
};
