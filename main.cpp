#include <iostream>
#include <string>

#include "Admin/AdminManager.h"
#include "Admin/HeadAdminManager.h"
#include "Core/check.h"
#include "Shop/ShopManager.h"
#include "User/UserManager.h"

using namespace std;

// Global managers keep the project structure simple for the template stage.
UserManager userManager;
ShopManager shopManager;
AdminManager adminManager;
HeadAdminManager headAdminManager;

void userMenu()
{
    int choice;

    do {
        // Customer-facing menu. The user module owns the actual logic behind these actions.
        cout << "\n========== USER MENU ==========\n";
        cout << "1. Show Profile\n";
        cout << "2. Edit Profile\n";
        cout << "3. Show Catalog\n";
        cout << "4. Search Products\n";
        cout << "5. Add To Cart\n";
        cout << "6. Show Cart\n";
        cout << "7. Create Order\n";
        cout << "8. Show Orders\n";
        cout << "9. Add Favorite\n";
        cout << "10. Show Favorites\n";
        cout << "11. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice)
        {
        case 1:
            userManager.showProfile();
            break;
        case 2:
            userManager.editProfile();
            break;
        case 3:
            shopManager.showCatalog();
            break;
        case 4:
            shopManager.searchProducts();
            break;
        case 5:
            userManager.addToCart();
            break;
        case 6:
            userManager.showCart();
            break;
        case 7:
            userManager.createOrder();
            break;
        case 8:
            userManager.showOrders();
            break;
        case 9:
            userManager.addFavorite();
            break;
        case 10:
            userManager.showFavorites();
            break;
        case 11:
            userManager.logoutUser();
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 11);
}

void shopMenu()
{
    int choice;

    do {
        // This is the shop-owner menu. The shop team should implement the actions behind these entries.
        cout << "\n========== SHOP MENU ==========\n";
        cout << "1. Add Product\n";
        cout << "2. Edit Product\n";
        cout << "3. Delete Product\n";
        cout << "4. Show Products\n";
        cout << "5. Update Stock\n";
        cout << "6. Show Orders\n";
        cout << "7. Accept Order\n";
        cout << "8. Complete Order\n";
        cout << "9. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice)
        {
        case 1:
            shopManager.addProduct();
            break;
        case 2:
            shopManager.editProduct();
            break;
        case 3:
            shopManager.deleteProduct();
            break;
        case 4:
            shopManager.showProducts();
            break;
        case 5:
            shopManager.updateStock();
            break;
        case 6:
            shopManager.showShopOrders();
            break;
        case 7:
            shopManager.acceptOrder();
            break;
        case 8:
            shopManager.completeOrder();
            break;
        case 9:
            shopManager.logoutShop();
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 9);
}

void adminMenu()
{
    int choice;

    do {
        // Regular admin menu. Admin-specific behavior is expected to be filled in by the admin module.
        cout << "\n========== ADMIN MENU ==========\n";
        cout << "1. Approve Shop\n";
        cout << "2. Reject Shop\n";
        cout << "3. Show Pending Shops\n";
        cout << "4. Show All Users\n";
        cout << "5. Show All Shops\n";
        cout << "6. Delete User\n";
        cout << "7. Remove Shop\n";
        cout << "8. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice)
        {
        case 1:
            adminManager.approveShop(shopManager);
            break;
        case 2:
            adminManager.rejectShop(shopManager);
            break;
        case 3:
            shopManager.showPendingShops();
            break;
        case 4:
            adminManager.showAllUsers();
            break;
        case 5:
            adminManager.showAllShops();
            break;
        case 6:
            adminManager.deleteUser(userManager);
            break;
        case 7:
            adminManager.removeShop(shopManager);
            break;
        case 8:
            adminManager.logoutAdmin();
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 8);
}

void headAdminMenu()
{
    int choice;

    do {
        // Head admin has the same base access plus higher-privilege management actions.
        cout << "\n========== HEAD ADMIN MENU ==========\n";
        cout << "1. Approve Shop\n";
        cout << "2. Reject Shop\n";
        cout << "3. Show All Users\n";
        cout << "4. Show All Shops\n";
        cout << "5. Delete User\n";
        cout << "6. Ban User\n";
        cout << "7. Remove Shop\n";
        cout << "8. Add Admin\n";
        cout << "9. Remove Admin\n";
        cout << "10. Show Statistics\n";
        cout << "11. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice)
        {
        case 1:
            adminManager.approveShop(shopManager);
            break;
        case 2:
            adminManager.rejectShop(shopManager);
            break;
        case 3:
            adminManager.showAllUsers();
            break;
        case 4:
            adminManager.showAllShops();
            break;
        case 5:
            adminManager.deleteUser(userManager);
            break;
        case 6:
            adminManager.banUser(userManager);
            break;
        case 7:
            adminManager.removeShop(shopManager);
            break;
        case 8:
            adminManager.addAdmin();
            break;
        case 9:
            adminManager.removeAdmin();
            break;
        case 10:
            adminManager.showStatistics();
            break;
        case 11:
            adminManager.logoutAdmin();
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 11);
}

int main()
{
    int choice;

    do {
        // Entry point: choose login, registration, admin access, or exit.
        cout << "\n====================================\n";
        cout << "         MARKETPLACE SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        if (choice == 1)
        {
            int type;
            string login;
            string password;

            cout << "\nLogin as:\n";
            cout << "1. User\n";
            cout << "2. Shop\n";
            cout << "\nChoose: ";
            type = safeInput<int>();

            cout << "\nLogin: ";
            cin >> login;

            cout << "Password: ";
            cin >> password;

            if (type == 1)
            {
                if (userManager.loginUser(login, password))
                {
                    cout << "\nLogin successful!\n";
                    userMenu();
                }
                else
                {
                    cout << "\nWrong login or password!\n";
                }
            }
            else if (type == 2)
            {
                int result = shopManager.loginShop(login, password);
                if (result == 1)
                {
                    cout << "\nLogin successful!\n";
                    shopMenu();
                }
                else if (result == 2)
                {
                    cout << "\nYour shop is waiting for admin approval.\n";
                }
                else
                {
                    cout << "\nWrong login or password!\n";
                }
            }
            else
            {
                cout << "\nInvalid type!\n";
            }
        }
        else if (choice == 2)
        {
            int type;

            cout << "\nRegister as:\n";
            cout << "1. User\n";
            cout << "2. Shop\n";
            cout << "\nChoose: ";
            type = safeInput<int>();

            if (type == 1)
            {
                userManager.registerUser();
            }
            else if (type == 2)
            {
                shopManager.registerShop();
            }
            else
            {
                cout << "\nInvalid type!\n";
            }
        }
        else if (choice == 3)
        {
            string login;
            string password;

            cout << "\nAdmin Login: ";
            cin >> login;

            cout << "Password: ";
            cin >> password;

            if (login == "admin" && password == "123")
            {
                adminMenu();
            }
            else if (headAdminManager.loginHeadAdmin(login, password))
            {
                headAdminMenu();
            }
            else
            {
                cout << "\nWrong admin credentials!\n";
            }
        }
        else if (choice == 4)
        {
            cout << "\nClosing system...\n";
        }
        else
        {
            cout << "\nInvalid choice!\n";
        }

    } while (choice != 4);

    return 0;
}