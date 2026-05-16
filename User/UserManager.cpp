#include "User.h"

class UserManager
{
public:
    void registerUser();

    bool loginUser(string login, string password);

    void logoutUser();
    
    void showProfile();

    void editProfile();

    void deleteAccount();

    void searchProducts();

    void addToCart();

    void removeFromCart();

    void showCart();

    void clearCart();

    void createOrder();

    void cancelOrder();

    void showOrders();

    void trackOrder();

    void addFavorite();

    void removeFavorite();

    void showFavorites();
};