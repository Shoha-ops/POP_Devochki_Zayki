#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "User.h"

using namespace std;

// Persistent user record stored in a flat text file.
struct StoredUser
{
    long long id;
    string name;
    string login;
    string password;
    string email;
};

// User manager template: registration and login are implemented here,
// while other actions stay as placeholders for the user module team.
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

    // Creates a new user entry and saves it to users.txt.
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

    // Verifies credentials against the stored user list.
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

    // Clears the active session state.
    void logoutUser()
    {
        loggedIn = false;
        currentLogin.clear();
    }

    // Prints the currently logged-in user's profile.
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

    // Updates profile fields for the active user and persists the result.
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

    // Template hooks for the rest of the user module.
    void deleteAccount() {}
    void searchProducts() {}
    void addToCart() {}
    void removeFromCart() {}
    void showCart() {}
    void clearCart() {}
    void createOrder() {}
    void cancelOrder() {}
    void showOrders() {}
    void trackOrder() {}
    void addFavorite() {}
    void removeFavorite() {}
    void showFavorites() {}

    // Simple dump of stored users for debugging or admin support.
    void showUsers()
    {
        cout << "\n=== USERS ===\n";
        for (const StoredUser& user : users)
        {
            cout << user.id << " | " << user.name << " | " << user.login << " | " << user.email << '\n';
        }
    }
};
