#include "ProductManager.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "../Core/check.h"

using namespace std;

struct ProductRecord {
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

vector<string> splitProductLine(const string& line) {
    vector<string> parts;
    string part;
    stringstream stream(line);

    while (getline(stream, part, '|')) {
        parts.push_back(part);
    }

    return parts;
}

string cleanProductField(string value) {
    replace(value.begin(), value.end(), '|', '/');
    return value;
}

vector<ProductRecord> loadProducts() {
    vector<ProductRecord> products;
    ifstream file("products.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitProductLine(line);
        if (parts.size() < 9) continue;

        ProductRecord product;
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

void saveProducts(const vector<ProductRecord>& products) {
    ofstream file("products.txt", ios::trunc);
    for (const ProductRecord& product : products) {
        file << product.id << '|'
             << cleanProductField(product.name) << '|'
             << cleanProductField(product.description) << '|'
             << fixed << setprecision(2) << product.price << '|'
             << product.stock << '|'
             << cleanProductField(product.category) << '|'
             << cleanProductField(product.shopLogin) << '|'
             << cleanProductField(product.shopName) << '|'
             << fixed << setprecision(1) << product.rating << '\n';
    }
}

int getNextProductId(const vector<ProductRecord>& products) {
    int nextId = 1;
    for (const ProductRecord& product : products) {
        if (product.id >= nextId) {
            nextId = product.id + 1;
        }
    }
    return nextId;
}

ProductRecord* findProductById(vector<ProductRecord>& products, int id) {
    for (ProductRecord& product : products) {
        if (product.id == id) {
            return &product;
        }
    }
    return nullptr;
}

void printProductRecord(const ProductRecord& product) {
    cout << "ID: " << product.id << '\n';
    cout << "Name: " << product.name << '\n';
    cout << "Description: " << product.description << '\n';
    cout << "Price: " << fixed << setprecision(2) << product.price << '\n';
    cout << "Stock: " << product.stock << '\n';
    cout << "Category: " << product.category << '\n';
    cout << "Shop: " << product.shopName << '\n';
    cout << "Rating: " << fixed << setprecision(1) << product.rating << "\n\n";
}

void ProductManager::createProduct() {
    vector<ProductRecord> products = loadProducts();
    ProductRecord product;
    product.id = getNextProductId(products);
    product.rating = 0.0;

    cout << "\n===== CREATE PRODUCT =====\n";
    cout << "Shop login: ";
    getline(cin >> ws, product.shopLogin);
    cout << "Shop name: ";
    getline(cin >> ws, product.shopName);
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
    cout << "Product created. ID: " << product.id << '\n';
}

void ProductManager::editProduct() {
    vector<ProductRecord> products = loadProducts();
    int id;

    cout << "\nEnter product ID: ";
    id = safeInput<int>();

    ProductRecord* product = findProductById(products, id);
    if (product == nullptr) {
        cout << "Product not found.\n";
        return;
    }

    cout << "New name: ";
    getline(cin >> ws, product->name);
    cout << "New description: ";
    getline(cin >> ws, product->description);
    cout << "New price: ";
    cin.clear();
    product->price = safeInput<double>(0.01);
    cout << "New stock: ";
    cin.clear();
    product->stock = safeInput<int>(0);
    cout << "New category: ";
    getline(cin >> ws, product->category);

    saveProducts(products);
    cout << "Product updated.\n";
}

void ProductManager::deleteProduct() {
    vector<ProductRecord> products = loadProducts();
    int id;

    cout << "\nEnter product ID: ";
    id = safeInput<int>();

    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->id == id) {
            products.erase(it);
            saveProducts(products);
            cout << "Product deleted.\n";
            return;
        }
    }

    cout << "Product not found.\n";
}

void ProductManager::searchProducts() {
    string query;

    cout << "\nProduct name: ";
    getline(cin >> ws, query);

    vector<ProductRecord> products = loadProducts();
    bool found = false;

    for (const ProductRecord& product : products) {
        if (product.name.find(query) != string::npos) {
            printProductRecord(product);
            found = true;
        }
    }

    if (!found) cout << "No products found.\n";
}

void ProductManager::filterProducts() {
    string category;

    cout << "\nCategory: ";
    getline(cin >> ws, category);

    vector<ProductRecord> products = loadProducts();
    bool found = false;

    for (const ProductRecord& product : products) {
        if (product.category == category) {
            printProductRecord(product);
            found = true;
        }
    }

    if (!found) cout << "No products in this category.\n";
}

void ProductManager::sortProducts() {
    vector<ProductRecord> products = loadProducts();
    sort(products.begin(), products.end(), [](const ProductRecord& left, const ProductRecord& right) {
        return left.price < right.price;
    });

    cout << "\n===== PRODUCTS BY PRICE =====\n";
    for (const ProductRecord& product : products) {
        printProductRecord(product);
    }
}

void ProductManager::showCatalog() {
    string shopName;
    vector<ProductRecord> products = loadProducts();
    bool found = false;

    cout << "\nShop name: ";
    getline(cin >> ws, shopName);

    cout << "\n===== PRODUCT CATALOG =====\n";
    if (products.empty()) {
        cout << "Catalog is empty.\n";
        return;
    }

    for (const ProductRecord& product : products) {
        if (product.shopName == shopName) {
            printProductRecord(product);
            found = true;
        }
    }

    if (!found) cout << "No products found for this shop.\n";
}
