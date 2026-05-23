#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../Cart/CartManager.h"
#include "../Favorites/FavoritesManager.h"
#include "../Order/OrderManager.h"
#include "../Payment/PaymentManager.h"
#include "../Shop/ShopManager.h"
#include "User.h"

using namespace std;

struct StoredUser
{
    long long id;
    string name;
    string login;
    string password;
    string email;
};

class UserManager
{
private:
    vector<StoredUser> users;
    string storagePath;
    long long nextUserId;
    bool loggedIn;
    string currentLogin;

    const StoredUser* findUser(const string& login) const
    {
        for (const StoredUser& user : users)
        {
            if (user.login == login)
            {
                return &user;
            }
        }

        return nullptr;
    }

    StoredUser* findUser(const string& login)
    {
        for (StoredUser& user : users)
        {
            if (user.login == login)
            {
                return &user;
            }
        }

        return nullptr;
    }

    bool hasSession() const
    {
        if (!loggedIn || findUser(currentLogin) == nullptr)
        {
            cout << "No active user session.\n";
            return false;
        }

        return true;
    }

    void loadUsers()
    {
        users.clear();

        ifstream inFile(storagePath);
        if (!inFile.is_open())
        {
            return;
        }

        string line;
        while (getline(inFile, line))
        {
            if (line.empty())
            {
                continue;
            }

            stringstream ss(line);
            string idText;
            StoredUser user;

            if (!getline(ss, idText, '|')) continue;
            if (!getline(ss, user.name, '|')) continue;
            if (!getline(ss, user.login, '|')) continue;
            if (!getline(ss, user.password, '|')) continue;
            if (!getline(ss, user.email, '|')) continue;

            user.id = stoll(idText);
            users.push_back(user);

            if (user.id >= nextUserId)
            {
                nextUserId = user.id + 1;
            }
        }
    }

    void saveUsers() const
    {
        ofstream outFile(storagePath, ios::trunc);
        for (const StoredUser& user : users)
        {
            outFile << user.id << '|' << user.name << '|' << user.login << '|' << user.password << '|' << user.email << '\n';
        }
    }

public:
    explicit UserManager(const string& filePath = "users.txt")
        : storagePath(filePath), nextUserId(7000000000LL), loggedIn(false)
    {
        loadUsers();
    }

    void registerUser()
    {
        StoredUser user;
        user.id = nextUserId++;

        cout << "Enter name: ";
        getline(cin >> ws, user.name);

        cout << "Enter login: ";
        getline(cin >> ws, user.login);

        if (findUser(user.login) != nullptr)
        {
            cout << "Login already exists!\n";
            --nextUserId;
            return;
        }

        cout << "Enter password: ";
        getline(cin >> ws, user.password);

        cout << "Enter email: ";
        getline(cin >> ws, user.email);

        users.push_back(user);
        saveUsers();

        cout << "Registration successful!\n";
    }

    bool loginUser(string login, string password)
    {
        const StoredUser* user = findUser(login);
        if (user != nullptr && user->password == password)
        {
            loggedIn = true;
            currentLogin = login;
            return true;
        }

        return false;
    }

    void logoutUser()
    {
        loggedIn = false;
        currentLogin.clear();
    }

    string getCurrentLogin() const
    {
        return currentLogin;
    }

    void showProfile()
    {
        const StoredUser* user = findUser(currentLogin);
        if (!loggedIn || user == nullptr)
        {
            cout << "No active user session.\n";
            return;
        }

        cout << "\n=== PROFILE ===\n";
        cout << "ID: " << user->id << '\n';
        cout << "Name: " << user->name << '\n';
        cout << "Login: " << user->login << '\n';
        cout << "Email: " << user->email << '\n';
    }

    void editProfile()
    {
        StoredUser* user = findUser(currentLogin);
        if (!loggedIn || user == nullptr)
        {
            cout << "No active user session.\n";
            return;
        }

        cout << "Enter new name: ";
        getline(cin >> ws, user->name);

        cout << "Enter new password: ";
        getline(cin >> ws, user->password);

        cout << "Enter new email: ";
        getline(cin >> ws, user->email);

        saveUsers();
        cout << "Profile updated.\n";
    }

    void deleteAccount()
    {
        if (!hasSession())
        {
            return;
        }

        for (int i = 0; i < users.size(); i++)
        {
            if (users[i].login == currentLogin)
            {
                users.erase(users.begin() + i);
                saveUsers();
                logoutUser();
                cout << "Account deleted.\n";
                return;
            }
        }

        cout << "User not found.\n";
    }

    void searchProducts()
    {
        ShopManager shopManager;
        shopManager.searchProducts();
    }

    void addToCart()
    {
        if (!hasSession()) return;

        CartManager cartManager;
        cartManager.addToCart(currentLogin);
    }

    void removeFromCart()
    {
        if (!hasSession()) return;

        CartManager cartManager;
        cartManager.removeFromCart(currentLogin);
    }

    void showCart()
    {
        if (!hasSession()) return;

        CartManager cartManager;
        cartManager.showCart(currentLogin);
    }

    void clearCart()
    {
        if (!hasSession()) return;

        CartManager cartManager;
        cartManager.clearCart(currentLogin);
    }

    void createOrder()
    {
        if (!hasSession()) return;

        OrderManager orderManager;
        orderManager.createOrder(currentLogin);
    }

    void cancelOrder()
    {
        if (!hasSession()) return;

        OrderManager orderManager;
        orderManager.cancelOrder(currentLogin);
    }

    void showOrders()
    {
        if (!hasSession()) return;

        OrderManager orderManager;
        orderManager.showUserOrders(currentLogin);
    }

    void trackOrder()
    {
        if (!hasSession()) return;

        OrderManager orderManager;
        orderManager.trackOrder(currentLogin);
    }

    void addFavorite()
    {
        if (!hasSession()) return;

        FavoritesManager favoritesManager;
        favoritesManager.addFavorite(currentLogin);
    }

    void removeFavorite()
    {
        if (!hasSession()) return;

        FavoritesManager favoritesManager;
        favoritesManager.removeFavorite(currentLogin);
    }

    void showFavorites()
    {
        if (!hasSession()) return;

        FavoritesManager favoritesManager;
        favoritesManager.showFavorites(currentLogin);
    }

    void makePayment()
    {
        if (!hasSession()) return;

        PaymentManager paymentManager;
        paymentManager.makePayment(currentLogin);
    }

    void showPaymentHistory()
    {
        if (!hasSession()) return;

        PaymentManager paymentManager;
        paymentManager.showPaymentHistory(currentLogin);
    }

    void showUsers() {
        cout << "\n=== USERS ===\n";
        for (const StoredUser& user : users)
        {
            cout << user.id << " | " << user.name << " | " << user.login << " | " << user.email << '\n';
        }
    }
};
