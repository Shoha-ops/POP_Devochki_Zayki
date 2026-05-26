#pragma once

#include <string>

#include "Cart.h"

using namespace std;

class CartManager {
public:
    void addToCart(string userLogin);

    void removeFromCart(string userLogin);

    void clearCart(string userLogin);

    void showCart(string userLogin);
};
