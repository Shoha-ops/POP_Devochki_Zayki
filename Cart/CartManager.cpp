#include "CartManager.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../Core/check.h"

using namespace std;

struct CartProductRecord {
    int id;
    string name;
    double price;
    int stock;
};

struct CartItemRecord {
    string userLogin;
    int productId;
    int quantity;
};

vector<string> splitCartLine(const string& line) {
    vector<string> parts;
    string part;
    stringstream stream(line);

    while (getline(stream, part, '|')) {
        parts.push_back(part);
    }

    return parts;
}

vector<CartProductRecord> loadCartProducts() {
    vector<CartProductRecord> products;
    ifstream file("products.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitCartLine(line);
        if (parts.size() < 9) continue;

        CartProductRecord product;
        product.id = stoi(parts[0]);
        product.name = parts[1];
        product.price = stod(parts[3]);
        product.stock = stoi(parts[4]);
        products.push_back(product);
    }

    return products;
}

CartProductRecord* findCartProduct(vector<CartProductRecord>& products, int productId) {
    for (CartProductRecord& product : products) {
        if (product.id == productId) {
            return &product;
        }
    }
    return nullptr;
}

const CartProductRecord* findCartProduct(const vector<CartProductRecord>& products, int productId) {
    for (const CartProductRecord& product : products) {
        if (product.id == productId) {
            return &product;
        }
    }
    return nullptr;
}

vector<CartItemRecord> loadCart() {
    vector<CartItemRecord> cart;
    ifstream file("cart.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitCartLine(line);
        if (parts.size() < 3) continue;

        CartItemRecord item;
        item.userLogin = parts[0];
        item.productId = stoi(parts[1]);
        item.quantity = stoi(parts[2]);
        cart.push_back(item);
    }

    return cart;
}

void saveCart(const vector<CartItemRecord>& cart) {
    ofstream file("cart.txt", ios::trunc);
    for (const CartItemRecord& item : cart) {
        file << item.userLogin << '|' << item.productId << '|' << item.quantity << '\n';
    }
}

static string askCartUser() {
    string userLogin;
    cout << "User login: ";
    getline(cin >> ws, userLogin);
    return userLogin;
}

void CartManager::addToCart() {
    string userLogin = askCartUser();
    int productId;
    int quantity;

    cout << "Product ID: ";
    cin.clear();
    productId = safeInput<int>(1);
    cout << "Quantity: ";
    cin.clear();
    quantity = safeInput<int>(1);

    vector<CartProductRecord> products = loadCartProducts();
    CartProductRecord* product = findCartProduct(products, productId);

    if (product == nullptr || product->stock <= 0) {
        cout << "Product is not available.\n";
        return;
    }

    if (quantity > product->stock) {
        cout << "Invalid quantity. Available: " << product->stock << '\n';
        return;
    }

    vector<CartItemRecord> cart = loadCart();
    for (CartItemRecord& item : cart) {
        if (item.userLogin == userLogin && item.productId == productId) {
            if (item.quantity + quantity > product->stock) {
                cout << "Not enough stock.\n";
                return;
            }

            item.quantity += quantity;
            saveCart(cart);
            cout << "Cart updated.\n";
            return;
        }
    }

    CartItemRecord item;
    item.userLogin = userLogin;
    item.productId = productId;
    item.quantity = quantity;
    cart.push_back(item);
    saveCart(cart);

    cout << "Added to cart.\n";
}

void CartManager::removeFromCart() {
    string userLogin = askCartUser();
    int productId;

    cout << "Product ID: ";
    cin.clear();
    productId = safeInput<int>(1);

    vector<CartItemRecord> cart = loadCart();
    for (auto it = cart.begin(); it != cart.end(); ++it) {
        if (it->userLogin == userLogin && it->productId == productId) {
            cart.erase(it);
            saveCart(cart);
            cout << "Removed from cart.\n";
            return;
        }
    }

    cout << "Product not found in cart.\n";
}

void CartManager::clearCart() {
    string userLogin = askCartUser();
    vector<CartItemRecord> cart = loadCart();

    cart.erase(remove_if(cart.begin(), cart.end(), [&](const CartItemRecord& item) {
        return item.userLogin == userLogin;
    }), cart.end());

    saveCart(cart);
    cout << "Cart cleared.\n";
}

void CartManager::showCart() {
    string userLogin = askCartUser();
    vector<CartItemRecord> cart = loadCart();
    vector<CartProductRecord> products = loadCartProducts();
    double total = 0.0;
    bool found = false;

    cout << "\n===== CART =====\n";
    for (const CartItemRecord& item : cart) {
        if (item.userLogin != userLogin) continue;

        const CartProductRecord* product = findCartProduct(products, item.productId);
        if (product == nullptr) continue;

        double lineTotal = product->price * item.quantity;
        total += lineTotal;
        cout << product->name << " x" << item.quantity
             << " = " << lineTotal << '\n';
        found = true;
    }

    if (!found) {
        cout << "Cart is empty.\n";
        return;
    }

    cout << "Total: " << total << '\n';
}

void CartManager::calculateTotal() {
    showCart();
}
