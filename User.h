#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class User {
    int id;
    string name;
    vector<int> orderIds;
    vector<int> favoriteItems;

public:
    int createOrder();
    void addToFavorites(int id);
};