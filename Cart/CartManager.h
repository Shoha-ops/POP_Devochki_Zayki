#pragma once

#include "Cart.h"

class CartManager
{
public:
    void addToCart();

    void removeFromCart();

    void clearCart();

    void showCart();

    void calculateTotal();
};
