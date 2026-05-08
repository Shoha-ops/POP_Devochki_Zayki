#pragma once

#include <string>
#include <vector>
#include "Core Classes.h"

using namespace std;

class OrderManager {
    vector<Order> orders;

public:
    int createOrder(int userId);
    void assignShop(int orderId, int shopId);
    void updateStatus(int orderId, string status);
};
