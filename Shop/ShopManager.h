#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../Core/check.h"
#include "Shop.h"

using namespace std;

struct ShopProductRecord {
    int id;
    string name;
    string description;
    double price;
    int stock;
    string category;
    string shopLogin;
    string shopName;
    double rating;
};

struct ShopOrderItemRecord {
    int productId;
    string productName;
    string shopLogin;
    int quantity;
    double price;
};

struct ShopOrderRecord {
    int id;
    string userLogin;
    vector<ShopOrderItemRecord> items;
    double total;
    string status;
    string address;
    string paymentStatus;
};

class ShopManager {
private:
    vector<Shop> shops;
    string storagePath;
    int nextShopId;
    string currentShopLogin;

    vector<string> splitText(const string& text, char delimiter) {
        vector<string> parts;
        string part;
        stringstream stream(text);

        while (getline(stream, part, delimiter)) {
            parts.push_back(part);
        }

        return parts;
    }

    string cleanField(string value) {
        replace(value.begin(), value.end(), '|', '/');
        replace(value.begin(), value.end(), ';', ',');
        replace(value.begin(), value.end(), '#', '-');
        return value;
    }

    vector<ShopProductRecord> loadProducts() {
        vector<ShopProductRecord> products;
        ifstream file("products.txt");
        string line;

        while (getline(file, line)) {
            vector<string> parts = splitText(line, '|');
            if (parts.size() < 9) continue;

            ShopProductRecord product;
            product.id = stoi(parts[0]);
            product.name = parts[1];
            product.description = parts[2];
            product.price = stod(parts[3]);
            product.stock = stoi(parts[4]);
            product.category = parts[5];
            product.shopLogin = parts[6];
            product.shopName = parts[7];
            product.rating = stod(parts[8]);
            products.push_back(product);
        }

        return products;
    }

    void saveProducts(const vector<ShopProductRecord>& products) {
        ofstream file("products.txt", ios::trunc);
        for (const ShopProductRecord& product : products) {
            file << product.id << '|'
                 << cleanField(product.name) << '|'
                 << cleanField(product.description) << '|'
                 << fixed << setprecision(2) << product.price << '|'
                 << product.stock << '|'
                 << cleanField(product.category) << '|'
                 << cleanField(product.shopLogin) << '|'
                 << cleanField(product.shopName) << '|'
                 << fixed << setprecision(1) << product.rating << '\n';
        }
    }

    int nextProductId(const vector<ShopProductRecord>& products) {
        int nextId = 1;
        for (const ShopProductRecord& product : products) {
            if (product.id >= nextId) {
                nextId = product.id + 1;
            }
        }
        return nextId;
    }

    ShopProductRecord* findProduct(vector<ShopProductRecord>& products, int productId) {
        for (ShopProductRecord& product : products) {
            if (product.id == productId) {
                return &product;
            }
        }
        return nullptr;
    }

    void printProduct(const ShopProductRecord& product) {
        cout << "ID: " << product.id << '\n';
        cout << "Name: " << product.name << '\n';
        cout << "Description: " << product.description << '\n';
        cout << "Price: " << fixed << setprecision(2) << product.price << '\n';
        cout << "Stock: " << product.stock << '\n';
        cout << "Category: " << product.category << '\n';
        cout << "Shop: " << product.shopName << '\n';
        cout << "Rating: " << fixed << setprecision(1) << product.rating << "\n\n";
    }

    vector<ShopOrderRecord> loadOrders() {
        vector<ShopOrderRecord> orders;
        ifstream file("orders.txt");
        string line;

        while (getline(file, line)) {
            vector<string> parts = splitText(line, '|');
            if (parts.size() < 7) continue;

            ShopOrderRecord order;
            order.id = stoi(parts[0]);
            order.userLogin = parts[1];
            order.total = stod(parts[2]);
            order.status = parts[3];
            order.address = parts[4];
            order.paymentStatus = parts[5];

            vector<string> itemParts = splitText(parts[6], ';');
            for (const string& itemText : itemParts) {
                if (itemText.empty()) continue;
                vector<string> fields = splitText(itemText, '#');
                if (fields.size() < 5) continue;

                ShopOrderItemRecord item;
                item.productId = stoi(fields[0]);
                item.productName = fields[1];
                item.shopLogin = fields[2];
                item.quantity = stoi(fields[3]);
                item.price = stod(fields[4]);
                order.items.push_back(item);
            }

            orders.push_back(order);
        }

        return orders;
    }

    void saveOrders(const vector<ShopOrderRecord>& orders) {
        ofstream file("orders.txt", ios::trunc);
        for (const ShopOrderRecord& order : orders) {
            file << order.id << '|'
                 << cleanField(order.userLogin) << '|'
                 << fixed << setprecision(2) << order.total << '|'
                 << cleanField(order.status) << '|'
                 << cleanField(order.address) << '|'
                 << cleanField(order.paymentStatus) << '|';

            for (const ShopOrderItemRecord& item : order.items) {
                file << item.productId << '#'
                     << cleanField(item.productName) << '#'
                     << cleanField(item.shopLogin) << '#'
                     << item.quantity << '#'
                     << fixed << setprecision(2) << item.price << ';';
            }

            file << '\n';
        }
    }

    Shop* findShop(const string& login) {
        for (Shop& shop : shops) {
            if (shop.login == login) {
                return &shop;
            }
        }

        return nullptr;
    }

    void loadShops() {
        shops.clear();

        ifstream inFile(storagePath);
        if (!inFile.is_open()) {
            return;
        }

        string line;
        while (getline(inFile, line)) {
            if (line.empty()) {
                continue;
            }

            stringstream ss(line);
            string idText;
            Shop shop;

            if (!getline(ss, idText, '|')) continue;
            if (!getline(ss, shop.shopName, '|')) continue;
            if (!getline(ss, shop.ownerName, '|')) continue;
            if (!getline(ss, shop.login, '|')) continue;
            if (!getline(ss, shop.password, '|')) continue;
            if (!getline(ss, shop.email, '|')) continue;

            string approvedText;
            if (!getline(ss, approvedText, '|')) continue;

            shop.id = stoi(idText);
            shop.approved = (approvedText == "1");
            shops.push_back(shop);

            if (shop.id >= nextShopId) {
                nextShopId = shop.id + 1;
            }
        }
    }

    void saveShops() const {
        ofstream outFile(storagePath, ios::trunc);
        for (const Shop& shop : shops) {
            outFile << shop.id << '|' << shop.shopName << '|' << shop.ownerName << '|'
                    << shop.login << '|' << shop.password << '|' << shop.email << '|'
                    << (shop.approved ? "1" : "0") << '\n';
        }
    }

public:
    explicit ShopManager(const string& filePath = "shops.txt")
        : storagePath(filePath), nextShopId(1) {
        loadShops();
    }

    void reloadShops() {
        loadShops();
    }

    void registerShop() {
        Shop shop;
        shop.id = nextShopId++;

        cout << "\n===== SHOP REGISTRATION =====\n";

        cout << "Shop name: ";
        getline(cin >> ws, shop.shopName);

        cout << "Owner name: ";
        getline(cin >> ws, shop.ownerName);

        cout << "Login: ";
        getline(cin >> ws, shop.login);

        if (findShop(shop.login) != nullptr) {
            cout << "Login already exists!\n";
            --nextShopId;
            return;
        }

        cout << "Password: ";
        getline(cin >> ws, shop.password);

        cout << "Email: ";
        getline(cin >> ws, shop.email);

        shop.approved = false;
        shops.push_back(shop);
        saveShops();

        cout << "\nShop registration request sent!\n";
    }

    int loginShop(string login, string password) {
        Shop* shop = findShop(login);
        if (shop == nullptr || shop->password != password) {
            return 0;
        }

        if (shop->approved) {
            currentShopLogin = login;
            return 1;
        }

        return 2;
    }

    void logoutShop() {
        currentShopLogin.clear();
        cout << "\nShop logged out successfully!\n";
    }

    void approveShop() {
        string login;

        cout << "\nEnter shop login: ";
        getline(cin >> ws, login);

        Shop* shop = findShop(login);
        if (shop == nullptr) {
            cout << "Shop not found!\n";
            return;
        }

        shop->approve();
        saveShops();
        cout << "Shop approved successfully!\n";
    }

    void rejectShop() {
        string login;

        cout << "\nEnter shop login: ";
        getline(cin >> ws, login);

        for (auto it = shops.begin(); it != shops.end(); ++it) {
            if (it->login == login) {
                shops.erase(it);
                saveShops();
                cout << "Shop rejected and removed!\n";
                return;
            }
        }

        cout << "Shop not found!\n";
    }

    void showPendingShops() {
        cout << "\n===== PENDING SHOPS =====\n";

        for (const Shop& shop : shops) {
            if (!shop.approved) {
                cout << "Shop: " << shop.shopName << '\n';
                cout << "Owner: " << shop.ownerName << '\n';
                cout << "Login: " << shop.login << '\n';
                cout << '\n';
            }
        }
    }

    void showAllShops() {
        cout << "\n===== ALL SHOPS =====\n";

        for (const Shop& shop : shops) {
            cout << shop.shopName << " | " << shop.login << " | "
                 << (shop.approved ? "approved" : "pending") << '\n';
        }
    }

    void addProduct() {
        Shop* shop = findShop(currentShopLogin);
        if (shop == nullptr) {
            cout << "No active shop session.\n";
            return;
        }

        vector<ShopProductRecord> products = loadProducts();
        ShopProductRecord product;
        product.id = nextProductId(products);
        product.shopLogin = shop->login;
        product.shopName = shop->shopName;
        product.rating = 0.0;

        cout << "\n===== ADD PRODUCT =====\n";
        cout << "Name: ";
        getline(cin >> ws, product.name);
        cout << "Description: ";
        getline(cin >> ws, product.description);
        cout << "Price: ";
        cin.clear();
        product.price = safeInput<double>(0.01);
        cout << "Stock: ";
        cin.clear();
        product.stock = safeInput<int>(0);
        cout << "Category: ";
        getline(cin >> ws, product.category);

        products.push_back(product);
        saveProducts(products);
        cout << "Product added. ID: " << product.id << '\n';
    }

    void editProduct() {
        vector<ShopProductRecord> products = loadProducts();
        int id;

        cout << "\nEnter product ID: ";
        cin.clear();
        id = safeInput<int>(1);

        ShopProductRecord* product = findProduct(products, id);
        if (product == nullptr || product->shopLogin != currentShopLogin) {
            cout << "Product not found in your shop.\n";
            return;
        }

        cout << "New name: ";
        getline(cin >> ws, product->name);
        cout << "New description: ";
        getline(cin >> ws, product->description);
        cout << "New price: ";
        cin.clear();
        product->price = safeInput<double>(0.01);
        cout << "New category: ";
        getline(cin >> ws, product->category);

        saveProducts(products);
        cout << "Product updated.\n";
    }

    void deleteProduct() {
        vector<ShopProductRecord> products = loadProducts();
        int id;

        cout << "\nEnter product ID: ";
        cin.clear();
        id = safeInput<int>(1);

        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->id == id && it->shopLogin == currentShopLogin) {
                products.erase(it);
                saveProducts(products);
                cout << "Product deleted.\n";
                return;
            }
        }

        cout << "Product not found in your shop.\n";
    }

    void showProducts() {
        cout << "\n===== YOUR PRODUCTS =====\n";
        vector<ShopProductRecord> products = loadProducts();
        bool found = false;

        for (const ShopProductRecord& product : products) {
            if (product.shopLogin == currentShopLogin) {
                printProduct(product);
                found = true;
            }
        }

        if (!found) cout << "No products yet.\n";
    }

    void showCatalog() {
        vector<Shop> availableShops;
        int choice;

        cout << "\n===== SHOPS =====\n";
        for (const Shop& shop : shops) {
            if (shop.approved) {
                availableShops.push_back(shop);
                cout << availableShops.size() << ". " << shop.shopName << '\n';
            }
        }

        if (availableShops.empty()) {
            cout << "No approved shops.\n";
            return;
        }

        cout << "\nChoose shop: ";
        cin.clear();
        choice = safeInput<int>(1, availableShops.size());

        string shopName = availableShops[choice - 1].shopName;

        cout << "\n===== PRODUCT CATALOG =====\n";
        vector<ShopProductRecord> products = loadProducts();
        bool found = false;

        for (const ShopProductRecord& product : products) {
            if (product.shopName == shopName && product.stock > 0) {
                printProduct(product);
                found = true;
            }
        }

        if (!found) cout << "No products found for this shop.\n";
    }

    void searchProducts() {
        string query;
        cout << "\nProduct name: ";
        getline(cin >> ws, query);

        vector<ShopProductRecord> products = loadProducts();
        bool found = false;
        for (const ShopProductRecord& product : products) {
            if (product.name.find(query) != string::npos && product.stock > 0) {
                printProduct(product);
                found = true;
            }
        }

        if (!found) cout << "No products found.\n";
    }

    void filterProducts() {
        string category;
        cout << "\nCategory: ";
        getline(cin >> ws, category);

        vector<ShopProductRecord> products = loadProducts();
        bool found = false;
        for (const ShopProductRecord& product : products) {
            if (product.category == category && product.stock > 0) {
                printProduct(product);
                found = true;
            }
        }

        if (!found) cout << "No products in this category.\n";
    }

    void sortProducts() {
        vector<ShopProductRecord> products = loadProducts();
        sort(products.begin(), products.end(), [](const ShopProductRecord& left, const ShopProductRecord& right) {
            return left.price < right.price;
        });

        cout << "\n===== PRODUCTS BY PRICE =====\n";
        for (const ShopProductRecord& product : products) {
            if (product.stock > 0) printProduct(product);
        }
    }

    void updateStock() {
        vector<ShopProductRecord> products = loadProducts();
        int id;
        int stock;

        cout << "\nEnter product ID: ";
        cin.clear();
        id = safeInput<int>(1);
        cout << "New stock: ";
        cin.clear();
        stock = safeInput<int>(0);

        ShopProductRecord* product = findProduct(products, id);
        if (product == nullptr || product->shopLogin != currentShopLogin) {
            cout << "Product not found in your shop.\n";
            return;
        }

        product->stock = stock;
        saveProducts(products);
        cout << "Stock updated.\n";
    }

    void showShopOrders() {
        cout << "\n===== SHOP ORDERS =====\n";
        vector<ShopOrderRecord> orders = loadOrders();
        bool found = false;

        for (const ShopOrderRecord& order : orders) {
            bool hasShopItem = false;
            for (const ShopOrderItemRecord& item : order.items) {
                if (item.shopLogin == currentShopLogin) hasShopItem = true;
            }

            if (hasShopItem) {
                cout << "Order #" << order.id << " | User: " << order.userLogin
                     << " | Status: " << order.status
                     << " | Payment: " << order.paymentStatus << '\n';
                for (const ShopOrderItemRecord& item : order.items) {
                    if (item.shopLogin == currentShopLogin) {
                        cout << "  " << item.productName << " x" << item.quantity << '\n';
                    }
                }
                found = true;
            }
        }

        if (!found) cout << "No orders for your shop.\n";
    }

    void acceptOrder() {
        updateShopOrderStatus("Accepted");
    }

    void declineOrder() {
        updateShopOrderStatus("Declined");
    }

    void completeOrder() {
        updateShopOrderStatus("Completed");
    }

    void updateShopOrderStatus(const string& status) {
        vector<ShopOrderRecord> orders = loadOrders();
        int id;

        cout << "\nEnter order ID: ";
        cin.clear();
        id = safeInput<int>(1);

        for (ShopOrderRecord& order : orders) {
            bool hasShopItem = false;
            for (const ShopOrderItemRecord& item : order.items) {
                if (item.shopLogin == currentShopLogin) hasShopItem = true;
            }

            if (order.id == id && hasShopItem) {
                order.status = status;
                saveOrders(orders);
                cout << "Order status updated to " << status << ".\n";
                return;
            }
        }

        cout << "Order not found for your shop.\n";
    }
};
