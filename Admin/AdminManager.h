#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../Core/check.h" //не пригодился тк не использовала ввод чисел
#include "../User/UserManager.h"
#include "../Shop/ShopManager.h"
#include "Admin.h"

using namespace std;

class AdminManager {
protected: // протектед чтобы хедадмин мог видеть
    vector<StoredAdmin> admins;
    string storagePath;
    int nextAdminId;
    bool loggedIn;
    string currentLogin;
    string currentRole;

    void loadAdmins() {
        admins.clear();
        ifstream file(storagePath);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idText;
            StoredAdmin admin;
            if (!getline(ss, idText, '|')) continue;
            if (!getline(ss, admin.login, '|')) continue;
            if (!getline(ss, admin.password, '|')) continue;
            if (!getline(ss, admin.role, '|')) continue;
            admin.id = stoi(idText);
            admins.push_back(admin);
            if (admin.id >= nextAdminId)
                nextAdminId = admin.id + 1;
        }
    }

    void saveAdmins() {
        ofstream fout(storagePath, ios::trunc);
        for (const StoredAdmin& admin : admins)
            fout << admin.id       << '|'
                 << admin.login    << '|'
                 << admin.password << '|'
                 << admin.role     << '\n';
    }

    StoredAdmin* findAdmin(const string& login) {
        for (StoredAdmin& admin : admins)
            if (admin.login == login) return &admin;
        return nullptr;
    }

    const StoredAdmin* findAdmin(const string& login) const {
        for (const StoredAdmin& admin : admins)
            if (admin.login == login) return &admin;
        return nullptr;
    }

    vector<string> readUserLines() const {
        vector<string> lines;
        ifstream inFile("users.txt");
        string line;
        while (getline(inFile, line))
            if (!line.empty()) lines.push_back(line);
        return lines;
    }

    void rewriteUsers(const vector<string>& lines) const {
        ofstream outFile("users.txt", ios::trunc);
        for (const string& line : lines)
            outFile << line << '\n';
    }

    vector<string> readShopLines() const {
    vector<string> lines;
    ifstream inFile("shops.txt");
    string line;
    while (getline(inFile, line))
        if (!line.empty()) lines.push_back(line);
    return lines;
}

void rewriteShops(const vector<string>& lines) const {
    ofstream outFile("shops.txt", ios::trunc);
    for (const string& line : lines)
        outFile << line << '\n';
}

    vector<string> splitLine(const string& line, char delim) const {
        vector<string> parts;
        string part;
        stringstream ss(line);
        while (getline(ss, part, delim))
            parts.push_back(part);
        return parts;
    }

    int countLines(const string& path) const {
        ifstream f(path);
        int count = 0;
        string line;
        while (getline(f, line))
            if (!line.empty()) ++count;
        return count;
    }

public:
    explicit AdminManager(const string& filePath = "admins.txt")
        : storagePath(filePath), nextAdminId(1), loggedIn(false)
    {
        loadAdmins();
    }

    bool loginAdmin(const string& login, const string& password) {
        const StoredAdmin* admin = findAdmin(login);
        if (admin != nullptr &&
            admin->password == password &&
            admin->role == "admin") {
            loggedIn     = true;
            currentLogin = login;
            currentRole  = "admin";
            return true;
        }
        return false;
    }

    void logoutAdmin() {
        loggedIn = false;
        currentLogin.clear();
        currentRole.clear();
    }

    void deleteUser(UserManager& userManager) {
        string login;
        cout << "\nUser login to delete: ";
        getline(cin >> ws, login);

        vector<string> lines = readUserLines();
        bool found = false;

        for (auto it = lines.begin(); it != lines.end(); ++it) {
            vector<string> parts = splitLine(*it, '|');
            if (parts.size() >= 3 && parts[2] == login) {
                lines.erase(it);
                found = true;
                break;
            }
        }

        if (found) {
            rewriteUsers(lines);
            userManager.reloadUsers();
            cout << "User deleted.\n"; }
        else {
            cout << "User not found.\n"; }
    }

    void removeShop(ShopManager& shopManager) {
    string login;
    cout << "\nShop login to remove: ";
    getline(cin >> ws, login);

    vector<string> lines = readShopLines();
    bool found = false;

    for (auto it = lines.begin(); it != lines.end(); ++it) {
        vector<string> parts = splitLine(*it, '|');
        if (parts.size() >= 4 && parts[3] == login) {
            lines.erase(it);
            found = true;
            break;
        }
    }

    if (found) {
        rewriteShops(lines); 
        shopManager.reloadShops();
        cout << "Shop removed.\n"; }
    else {
        cout << "Shop not found.\n"; }
}

    void addAdmin() {
        StoredAdmin admin;
        admin.id = nextAdminId++;

        cout << "\nNew admin login: ";
        getline(cin >> ws, admin.login);

        if (findAdmin(admin.login) != nullptr) {
            cout << "Login already exists!\n";
            --nextAdminId;
            return;
        }

        cout << "New admin password: ";
        getline(cin >> ws, admin.password);

        admin.role = "admin";
        admins.push_back(admin);
        saveAdmins();
        cout << "Admin added.\n";
    }

    void removeAdmin() {
        string login;
        cout << "\nAdmin login to remove: ";
        getline(cin >> ws, login);

        for (auto it = admins.begin(); it != admins.end(); ++it) {
            if (it->login == login && it->role == "admin") {
                admins.erase(it);
                saveAdmins();
                cout << "Admin removed.\n";
                return;
            }
        }

        cout << "Admin not found.\n";
    }

    void showStatistics() {
        cout << "\n=== STATISTICS ===\n";
        cout << "Users:    " << countLines("users.txt")    << '\n';
        cout << "Shops:    " << countLines("shops.txt")    << '\n';
        cout << "Products: " << countLines("products.txt") << '\n';
        cout << "Orders:   " << countLines("orders.txt")   << '\n';
        cout << "Payments: " << countLines("payments.txt") << '\n';
        cout << "Admins:   " << countLines("admins.txt")   << '\n';
    }
};