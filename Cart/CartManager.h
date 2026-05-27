#pragma once

#include <string>

using namespace std;

class CartManager {
public:
    void addToCart(string userLogin);

    void removeFromCart(string userLogin);

    void clearCart(string userLogin);

    void showCart(string userLogin);
};
