#pragma once

#include <string>
#include <vector>

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
