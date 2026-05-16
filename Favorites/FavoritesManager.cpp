#include "FavoritesManager.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct FavoriteProductRecord
{
    int id;
    string name;
    string description;
    double price;
    int stock;
    string category;
    string shopName;
    double rating;
};

vector<string> splitFavoriteLine(const string& line)
{
    vector<string> parts;
    string part;
    stringstream stream(line);

    while (getline(stream, part, '|'))
    {
        parts.push_back(part);
    }

    return parts;
}

vector<FavoriteProductRecord> loadFavoriteProducts()
{
    vector<FavoriteProductRecord> products;
    ifstream file("products.txt");
    string line;

    while (getline(file, line))
    {
        vector<string> parts = splitFavoriteLine(line);
        if (parts.size() < 9) continue;

        FavoriteProductRecord product;
        product.id = stoi(parts[0]);
        product.name = parts[1];
        product.description = parts[2];
        product.price = stod(parts[3]);
        product.stock = stoi(parts[4]);
        product.category = parts[5];
        product.shopName = parts[7];
        product.rating = stod(parts[8]);
        products.push_back(product);
    }

    return products;
}

const FavoriteProductRecord* findFavoriteProduct(const vector<FavoriteProductRecord>& products, int productId)
{
    for (const FavoriteProductRecord& product : products)
    {
        if (product.id == productId)
        {
            return &product;
        }
    }
    return nullptr;
}

void printFavoriteProduct(const FavoriteProductRecord& product)
{
    cout << "ID: " << product.id << '\n';
    cout << "Name: " << product.name << '\n';
    cout << "Description: " << product.description << '\n';
    cout << "Price: " << fixed << setprecision(2) << product.price << '\n';
    cout << "Stock: " << product.stock << '\n';
    cout << "Category: " << product.category << '\n';
    cout << "Shop: " << product.shopName << '\n';
    cout << "Rating: " << fixed << setprecision(1) << product.rating << "\n\n";
}

vector<pair<string, int>> loadFavoriteList()
{
    vector<pair<string, int>> favorites;
    ifstream file("favorites.txt");
    string line;

    while (getline(file, line))
    {
        vector<string> parts = splitFavoriteLine(line);
        if (parts.size() < 2) continue;
        favorites.push_back(make_pair(parts[0], stoi(parts[1])));
    }

    return favorites;
}

void saveFavoriteList(const vector<pair<string, int>>& favorites)
{
    ofstream file("favorites.txt", ios::trunc);
    for (const pair<string, int>& favorite : favorites)
    {
        file << favorite.first << '|' << favorite.second << '\n';
    }
}

static string askFavoriteUser()
{
    string userLogin;
    cout << "User login: ";
    getline(cin >> ws, userLogin);
    return userLogin;
}

void FavoritesManager::addFavorite()
{
    string userLogin = askFavoriteUser();
    int productId;

    cout << "Product ID: ";
    cin >> productId;

    vector<FavoriteProductRecord> products = loadFavoriteProducts();
    if (findFavoriteProduct(products, productId) == nullptr)
    {
        cout << "Product not found.\n";
        return;
    }

    vector<pair<string, int>> favorites = loadFavoriteList();
    for (const pair<string, int>& favorite : favorites)
    {
        if (favorite.first == userLogin && favorite.second == productId)
        {
            cout << "Product is already in favorites.\n";
            return;
        }
    }

    favorites.push_back(make_pair(userLogin, productId));
    saveFavoriteList(favorites);
    cout << "Added to favorites.\n";
}

void FavoritesManager::removeFavorite()
{
    string userLogin = askFavoriteUser();
    int productId;

    cout << "Product ID: ";
    cin >> productId;

    vector<pair<string, int>> favorites = loadFavoriteList();
    for (auto it = favorites.begin(); it != favorites.end(); ++it)
    {
        if (it->first == userLogin && it->second == productId)
        {
            favorites.erase(it);
            saveFavoriteList(favorites);
            cout << "Removed from favorites.\n";
            return;
        }
    }

    cout << "Favorite not found.\n";
}

void FavoritesManager::showFavorites()
{
    string userLogin = askFavoriteUser();
    vector<pair<string, int>> favorites = loadFavoriteList();
    vector<FavoriteProductRecord> products = loadFavoriteProducts();
    bool found = false;

    cout << "\n===== FAVORITES =====\n";
    for (const pair<string, int>& favorite : favorites)
    {
        if (favorite.first != userLogin) continue;

        const FavoriteProductRecord* product = findFavoriteProduct(products, favorite.second);
        if (product != nullptr)
        {
            printFavoriteProduct(*product);
            found = true;
        }
    }

    if (!found) cout << "Favorites are empty.\n";
}
