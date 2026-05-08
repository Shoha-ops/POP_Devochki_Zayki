#pragma once

#include <string>

#include "../Shop/ShopManager.h"

using namespace std;

class UserManager;

// Admin module template: the admin team fills in the privileged actions.
class AdminManager
{
public:
    // Basic admin credential check used by the admin login path.
    bool loginAdmin(string login, string password)
    {
        return login == "admin" && password == "123";
    }

    // Session cleanup for admin flows.
    void logoutAdmin() {}

    // Management actions that other team members can later implement.
    void approveShop(ShopManager& shopManager) {}
    void rejectShop(ShopManager& shopManager) {}
    void showAllUsers() {}
    void showAllShops() {}
    void deleteUser(UserManager& userManager) {}
    void banUser(UserManager& userManager) {}
    void removeShop(ShopManager& shopManager) {}
    void addAdmin() {}
    void removeAdmin() {}
    void showStatistics() {}
};