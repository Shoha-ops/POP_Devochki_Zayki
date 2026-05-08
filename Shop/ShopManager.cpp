#include "ShopManager.h"
#include <iostream>

using namespace std;

void ShopManager::registerShop()
{
    cout << "Shop registration request sent!" << endl;
}

int ShopManager::loginShop(string login,
                           string password)
{
    for(auto& shop : shops)
    {
        if(shop.getLogin() == login &&
           shop.getPassword() == password)
        {
            if(shop.isApproved())
                return 1;

            return 2;
        }
    }

    return 0;
}

void ShopManager::showPendingShops()
{
    cout << "Pending shops..." << endl;
}