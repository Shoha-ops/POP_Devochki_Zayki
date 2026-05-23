#pragma once

#include <string>

#include "Order.h"

using namespace std;

class OrderManager {
public:
    void createOrder();

    void createOrder(string userLogin);

    void cancelOrder();

    void cancelOrder(string userLogin);

    void trackOrder();

    void trackOrder(string userLogin);

    void updateOrderStatus();

    void showUserOrders();

    void showUserOrders(string userLogin);

    void showShopOrders();
};
