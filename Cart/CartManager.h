#pragma once

#include <string>

#include "Cart.h"

using namespace std;

class CartManager {
public:
    void addToCart();

    void addToCart(string userLogin);

    void removeFromCart();

    void removeFromCart(string userLogin);

    void clearCart();

    void clearCart(string userLogin);

    void showCart();

    void showCart(string userLogin);

    void calculateTotal();
};
