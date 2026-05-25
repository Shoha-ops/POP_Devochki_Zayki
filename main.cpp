#include <iostream>
#include <string>

#include "Admin/AdminManager.h"
#include "Admin/HeadAdminManager.h"
#include "Cart/CartManager.h"
#include "Core/check.h"
#include "Favorites/FavoritesManager.h"
#include "Order/OrderManager.h"
#include "Payment/PaymentManager.h"
#include "Shop/ShopManager.h"
#include "User/UserManager.h"
#include "Cart/Cart.cpp"
#include "Cart/CartManager.cpp"
#include "Favorites/FavoritesManager.cpp"
#include "Order/Order.cpp"
#include "Order/OrderManager.cpp"
#include "Payment/PaymentManager.cpp"
#include "Product/Product.cpp"
#include "Product/ProductManager.cpp"
#include "Shop/Shop.cpp"

using namespace std;

UserManager userManager;
ShopManager shopManager;
AdminManager adminManager;
HeadAdminManager headAdminManager;
CartManager cartManager;
FavoritesManager favoritesManager;
OrderManager orderManager;
PaymentManager paymentManager;

void userMenu() {
    int choice;

    do {
        cout << "\n========== USER MENU ==========\n";
        cout << "1. Show Profile\n";
        cout << "2. Edit Profile\n";
        cout << "3. Show Catalog\n";
        cout << "4. Search Products\n";
        cout << "5. Add To Cart\n";
        cout << "6. Remove From Cart\n";
        cout << "7. Show Cart\n";
        cout << "8. Clear Cart\n";
        cout << "9. Create Order\n";
        cout << "10. Cancel Order\n";
        cout << "11. Track Order\n";
        cout << "12. Show Orders\n";
        cout << "13. Pay Order\n";
        cout << "14. Payment History\n";
        cout << "15. Add Favorite\n";
        cout << "16. Remove Favorite\n";
        cout << "17. Show Favorites\n";
        cout << "18. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice) {
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
            userManager.searchProducts();
            break;
        case 5:
            userManager.addToCart();
            break;
        case 6:
            userManager.removeFromCart();
            break;
        case 7:
            userManager.showCart();
            break;
        case 8:
            userManager.clearCart();
            break;
        case 9:
            userManager.createOrder();
            break;
        case 10:
            userManager.cancelOrder();
            break;
        case 11:
            userManager.trackOrder();
            break;
        case 12:
            userManager.showOrders();
            break;
        case 13:
            userManager.makePayment();
            break;
        case 14:
            userManager.showPaymentHistory();
            break;
        case 15:
            userManager.addFavorite();
            break;
        case 16:
            userManager.removeFavorite();
            break;
        case 17:
            userManager.showFavorites();
            break;
        case 18:
            userManager.logoutUser();
            cout << "\nUser logged out successfully!\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 18);
}

void shopMenu() {
    int choice;

    do {
        cout << "\n========== SHOP MENU ==========\n";
        cout << "1. Add Product\n";
        cout << "2. Edit Product\n";
        cout << "3. Delete Product\n";
        cout << "4. Show My Products\n";
        cout << "5. Update Stock\n";
        cout << "6. Show Orders\n";
        cout << "7. Accept Order\n";
        cout << "8. Decline Order\n";
        cout << "9. Complete Order\n";
        cout << "10. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice) {
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
            shopManager.declineOrder();
            break;
        case 9:
            shopManager.completeOrder();
            break;
        case 10:
            shopManager.logoutShop();
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 10);
}

void adminMenu() {
    int choice;

    do {
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

        switch (choice) {
        case 1:
            shopManager.approveShop();
            break;
        case 2:
            shopManager.rejectShop();
            break;
        case 3:
            shopManager.showPendingShops();
            break;
        case 4:
            userManager.showUsers();
            break;
        case 5:
            shopManager.showAllShops();
            break;
        case 6:
            adminManager.deleteUser(userManager);
            break;
        case 7:
            adminManager.removeShop(shopManager);
            break;
        case 8:
            adminManager.logoutAdmin();
            cout << "\nAdmin logged out successfully!\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 8);
}

void headAdminMenu() {
    int choice;

    do {
        cout << "\n========== HEAD ADMIN MENU ==========\n";
        cout << "1. Approve Shop\n";
        cout << "2. Reject Shop\n";
        cout << "3. Show Pending Shops\n";
        cout << "4. Show All Users\n";
        cout << "5. Show All Shops\n";
        cout << "6. Delete User\n";
        cout << "7. Remove Shop\n";
        cout << "8. Add Admin\n";
        cout << "9. Remove Admin\n";
        cout << "10. Show Statistics\n";
        cout << "11. Logout\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice) {
        case 1:
            shopManager.approveShop();
            break;
        case 2:
            shopManager.rejectShop();
            break;
        case 3:
            shopManager.showPendingShops();
            break;
        case 4:
            userManager.showUsers();
            break;
        case 5:
            shopManager.showAllShops();
            break;
        case 6:
            adminManager.deleteUser(userManager);
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
            headAdminManager.logoutHeadAdmin();
            cout << "\nHead admin logged out successfully!\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 11);
}

void loginFlow() {
    int type;
    string login;
    string password;

    cout << "\nLogin as:\n";
    cout << "1. User\n";
    cout << "2. Shop\n";
    cout << "\nChoose: ";
    type = safeInput<int>();

    cout << "\nLogin: ";
    getline(cin >> ws, login);

    cout << "Password: ";
    getline(cin >> ws, password);

    if (type == 1) {
        if (userManager.loginUser(login, password)) {
            cout << "\nLogin successful!\n";
            userMenu();
        }
        else {
            cout << "\nWrong login or password!\n";
        }
    }
    else if (type == 2) {
        int result = shopManager.loginShop(login, password);
        if (result == 1) {
            cout << "\nLogin successful!\n";
            shopMenu();
        }
        else if (result == 2) {
            cout << "\nYour shop is waiting for admin approval.\n";
        }
        else {
            cout << "\nWrong login or password!\n";
        }
    }
    else {
        cout << "\nInvalid account type!\n";
    }
}

void registerFlow() {
    int type;

    cout << "\nRegister as:\n";
    cout << "1. User\n";
    cout << "2. Shop\n";
    cout << "\nChoose: ";
    type = safeInput<int>();

    switch (type) {
    case 1:
        userManager.registerUser();
        break;
    case 2:
        shopManager.registerShop();
        break;
    default:
        cout << "\nInvalid account type!\n";
    }
}

void adminLoginFlow() {
    string login;
    string password;

    cout << "\nAdmin Login: ";
    getline(cin >> ws, login);

    cout << "Password: ";
    getline(cin >> ws, password);

    if (adminManager.loginAdmin(login, password)) {
        adminMenu();
    }
    else if (headAdminManager.loginHeadAdmin(login, password)) {
        headAdminMenu();
    }
    else {
        cout << "\nWrong admin credentials!\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << "         MARKETPLACE SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "\nChoose: ";

        choice = safeInput<int>();

        switch (choice) {
        case 1:
            loginFlow();
            break;
        case 2:
            registerFlow();
            break;
        case 3:
            adminLoginFlow();
            break;
        case 4:
            cout << "\nClosing system...\n";
            break;
        default:
            cout << "\nInvalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
