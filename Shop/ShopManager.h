#pragma once

#include <string>

#include "Shop.h"

using namespace std;

// Shop module template: this is where the shop team's actions are declared.
class ShopManager
{
public:
    // Shop registration/login are kept minimal here so the other modules can build around the interface.
    void registerShop() {}

    int loginShop(string login, string password)
    {
        if (login.empty() || password.empty())
            return 0;

        return 1;
    }

    void logoutShop() {}

    // Administrative shop state actions.
    void approveShop() {}
    void rejectShop() {}
    void showPendingShops() {}
    void showAllShops() {}

    // Product management hooks for the shop team.
    void addProduct() {}
    void editProduct() {}
    void deleteProduct() {}
    void showProducts() {}
    void showCatalog() {}
    void searchProducts() {}
    void filterProducts() {}
    void sortProducts() {}
    void updateStock() {}

    // Order handling hooks for the shop owner.
    void showShopOrders() {}
    void acceptOrder() {}
    void declineOrder() {}
    void completeOrder() {}
};