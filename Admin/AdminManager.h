#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "../User/UserManager.h"
#include "../Shop/ShopManager.h"
#include "Admin.h"

using namespace std;

class AdminManager {
protected:
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
            stringstream stream(line);
            string idText;
            StoredAdmin admin;
            if (!getline(stream, idText, '|')) continue;
            if (!getline(stream, admin.login, '|')) continue;
            if (!getline(stream, admin.password, '|')) continue;
            if (!getline(stream, admin.role, '|')) continue;
            try {
                admin.id = stoi(idText);
                admins.push_back(admin);
                if (admin.id >= nextAdminId)
                    nextAdminId = admin.id + 1;
            }
            catch (...) {
                continue;
            }
        }
    }

    void saveAdmins() {
        ofstream fout(storagePath, ios::trunc);
        for (const StoredAdmin& admin : admins)
            fout << admin.id       << '|'
                 << cleanAdminField(admin.login)    << '|'
                 << cleanAdminField(admin.password) << '|'
                 << cleanAdminField(admin.role)     << '\n';
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
        stringstream stream(line);
        while (getline(stream, part, delim))
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

    string cleanAdminField(string value) const {
        replace(value.begin(), value.end(), '|', '/');
        replace(value.begin(), value.end(), ';', ',');
        replace(value.begin(), value.end(), '#', '-');
        return value;
    }

    bool removeLinesByField(const string& path, size_t fieldIndex, const string& value) const {
        vector<string> lines;
        ifstream inFile(path);
        string line;
        bool removed = false;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            if (parts.size() > fieldIndex && parts[fieldIndex] == value) {
                removed = true;
                continue;
            }
            lines.push_back(line);
        }

        ofstream outFile(path, ios::trunc);
        for (const string& keptLine : lines)
            outFile << keptLine << '\n';

        return removed;
    }

    set<int> removeOrdersByUser(const string& login) const {
        vector<string> lines;
        set<int> removedOrderIds;
        ifstream inFile("orders.txt");
        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            if (parts.size() >= 2 && parts[1] == login) {
                try {
                    removedOrderIds.insert(stoi(parts[0]));
                }
                catch (...) {
                }
                continue;
            }
            lines.push_back(line);
        }

        ofstream outFile("orders.txt", ios::trunc);
        for (const string& keptLine : lines)
            outFile << keptLine << '\n';

        return removedOrderIds;
    }

    set<int> collectProductIdsByShop(const string& shopLogin) const {
        set<int> productIds;
        ifstream inFile("products.txt");
        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            if (parts.size() >= 7 && parts[6] == shopLogin) {
                try {
                    productIds.insert(stoi(parts[0]));
                }
                catch (...) {
                }
            }
        }

        return productIds;
    }

    void removeProductsByShop(const string& shopLogin) const {
        vector<string> lines;
        ifstream inFile("products.txt");
        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            if (parts.size() >= 7 && parts[6] == shopLogin) continue;
            lines.push_back(line);
        }

        ofstream outFile("products.txt", ios::trunc);
        for (const string& keptLine : lines)
            outFile << keptLine << '\n';
    }

    void removeLinesByProductIds(const string& path, size_t productFieldIndex, const set<int>& productIds) const {
        if (productIds.empty()) return;

        vector<string> lines;
        ifstream inFile(path);
        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            bool remove = false;

            if (parts.size() > productFieldIndex) {
                try {
                    remove = productIds.count(stoi(parts[productFieldIndex])) > 0;
                }
                catch (...) {
                    remove = false;
                }
            }

            if (!remove) lines.push_back(line);
        }

        ofstream outFile(path, ios::trunc);
        for (const string& keptLine : lines)
            outFile << keptLine << '\n';
    }

    set<int> removeOrdersByShop(const string& shopLogin) const {
        vector<string> lines;
        set<int> removedOrderIds;
        ifstream inFile("orders.txt");
        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            bool hasShopItem = false;

            if (parts.size() >= 7) {
                vector<string> items = splitLine(parts[6], ';');
                for (const string& itemText : items) {
                    if (itemText.empty()) continue;
                    vector<string> fields = splitLine(itemText, '#');
                    if (fields.size() >= 3 && fields[2] == shopLogin) {
                        hasShopItem = true;
                        break;
                    }
                }
            }

            if (hasShopItem) {
                try {
                    removedOrderIds.insert(stoi(parts[0]));
                }
                catch (...) {
                }
                continue;
            }

            lines.push_back(line);
        }

        ofstream outFile("orders.txt", ios::trunc);
        for (const string& keptLine : lines)
            outFile << keptLine << '\n';

        return removedOrderIds;
    }

    void removePaymentsByOrderIds(const set<int>& orderIds) const {
        if (orderIds.empty()) return;

        vector<string> lines;
        ifstream inFile("payments.txt");
        string line;

        while (getline(inFile, line)) {
            if (line.empty()) continue;
            vector<string> parts = splitLine(line, '|');
            bool remove = false;

            if (parts.size() >= 2) {
                try {
                    remove = orderIds.count(stoi(parts[1])) > 0;
                }
                catch (...) {
                    remove = false;
                }
            }

            if (!remove) lines.push_back(line);
        }

        ofstream outFile("payments.txt", ios::trunc);
        for (const string& keptLine : lines)
            outFile << keptLine << '\n';
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
            removeLinesByField("cart.txt", 0, login);
            removeLinesByField("favorites.txt", 0, login);
            removeLinesByField("payments.txt", 2, login);
            removePaymentsByOrderIds(removeOrdersByUser(login));
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
        set<int> productIds = collectProductIdsByShop(login);
        rewriteShops(lines); 
        removeProductsByShop(login);
        removeLinesByProductIds("cart.txt", 1, productIds);
        removeLinesByProductIds("favorites.txt", 1, productIds);
        removePaymentsByOrderIds(removeOrdersByShop(login));
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
        admin.login = cleanAdminField(admin.login);

        if (findAdmin(admin.login) != nullptr) {
            cout << "Login already exists!\n";
            --nextAdminId;
            return;
        }

        cout << "New admin password: ";
        getline(cin >> ws, admin.password);
        admin.password = cleanAdminField(admin.password);

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
