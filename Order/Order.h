#pragma once

#include <vector>
#include <string>

using namespace std;

class Order {
public:
    int id;

    int userId;

    int shopId;

    vector<int> products;

    double totalPrice;

    string status;

    Order();
};
