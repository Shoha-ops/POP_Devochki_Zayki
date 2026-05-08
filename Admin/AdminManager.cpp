#include "../User/UserManager.h"
#include "../Shop/ShopManager.h"

class AdminManager
{
public:
    bool loginAdmin(string login, string password);

    void logoutAdmin();

    void approveShop(ShopManager& shopManager);

    void rejectShop(ShopManager& shopManager);

    void showAllUsers();

    void showAllShops();

    void deleteUser(UserManager& userManager);

    void banUser(UserManager& userManager);

    void removeShop(ShopManager& shopManager);

    void addAdmin();

    void removeAdmin();

    void showStatistics();
};