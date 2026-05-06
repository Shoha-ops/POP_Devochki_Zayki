#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Product {
    int id;
    string name;
    double price;
    int quantity;
};

class Order {
    int id;
    int userId;
    int shopId;
    vector<int> productIds;
    string status;
};

class User {
    int id;
    string name;
    vector<int> orderIds;
    vector<int> favoriteItems;
};

class Shop {
    int id;
    string name;
    vector<Product> products;
    vector<int> activeOrders;
};
