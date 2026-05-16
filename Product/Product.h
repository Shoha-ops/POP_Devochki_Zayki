#pragma once

#include <string>

using namespace std;

class Product {
public:
    int id;

    string name;
    string description;

    double price;

    int stock;

    string category;

    int shopId;

    double rating;

    Product();
};
