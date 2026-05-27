#pragma once

#include <string>

using namespace std;

class OrderManager {
public:
    void createOrder(string userLogin);

    void cancelOrder(string userLogin);

    void trackOrder(string userLogin);

    void updateOrderStatus();

    void showUserOrders(string userLogin);

    void showShopOrders();
};
