#include "OrderManager.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "../Core/check.h"

using namespace std;

struct OrderProductRecord {
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

struct OrderCartItemRecord {
    string userLogin;
    int productId;
    int quantity;
};

struct OrderItemRecord {
    int productId;
    string productName;
    string shopLogin;
    int quantity;
    double price;
};

struct OrderRecord {
    int id;
    string userLogin;
    vector<OrderItemRecord> items;
    double total;
    string status;
    string address;
    string paymentStatus;
};

vector<string> splitOrderText(const string& text, char delimiter) {
    vector<string> parts;
    string part;
    stringstream stream(text);

    while (getline(stream, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}

string cleanOrderField(string value) {
    replace(value.begin(), value.end(), '|', '/');
    replace(value.begin(), value.end(), ';', ',');
    replace(value.begin(), value.end(), '#', '-');
    return value;
}

vector<OrderProductRecord> loadOrderProducts() {
    vector<OrderProductRecord> products;
    ifstream file("products.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitOrderText(line, '|');
        if (parts.size() < 9) continue;

        OrderProductRecord product;
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

void saveOrderProducts(const vector<OrderProductRecord>& products) {
    ofstream file("products.txt", ios::trunc);
    for (const OrderProductRecord& product : products) {
        file << product.id << '|'
             << cleanOrderField(product.name) << '|'
             << cleanOrderField(product.description) << '|'
             << fixed << setprecision(2) << product.price << '|'
             << product.stock << '|'
             << cleanOrderField(product.category) << '|'
             << cleanOrderField(product.shopLogin) << '|'
             << cleanOrderField(product.shopName) << '|'
             << fixed << setprecision(1) << product.rating << '\n';
    }
}

OrderProductRecord* findOrderProduct(vector<OrderProductRecord>& products, int productId) {
    for (OrderProductRecord& product : products) {
        if (product.id == productId) {
            return &product;
        }
    }
    return nullptr;
}

vector<OrderCartItemRecord> loadOrderCart() {
    vector<OrderCartItemRecord> cart;
    ifstream file("cart.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitOrderText(line, '|');
        if (parts.size() < 3) continue;

        OrderCartItemRecord item;
        item.userLogin = parts[0];
        item.productId = stoi(parts[1]);
        item.quantity = stoi(parts[2]);
        cart.push_back(item);
    }

    return cart;
}

void saveOrderCart(const vector<OrderCartItemRecord>& cart) {
    ofstream file("cart.txt", ios::trunc);
    for (const OrderCartItemRecord& item : cart) {
        file << item.userLogin << '|' << item.productId << '|' << item.quantity << '\n';
    }
}

vector<OrderRecord> loadOrders() {
    vector<OrderRecord> orders;
    ifstream file("orders.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitOrderText(line, '|');
        if (parts.size() < 7) continue;

        OrderRecord order;
        order.id = stoi(parts[0]);
        order.userLogin = parts[1];
        order.total = stod(parts[2]);
        order.status = parts[3];
        order.address = parts[4];
        order.paymentStatus = parts[5];

        vector<string> itemParts = splitOrderText(parts[6], ';');
        for (const string& itemText : itemParts) {
            if (itemText.empty()) continue;
            vector<string> fields = splitOrderText(itemText, '#');
            if (fields.size() < 5) continue;

            OrderItemRecord item;
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

void saveOrders(const vector<OrderRecord>& orders) {
    ofstream file("orders.txt", ios::trunc);
    for (const OrderRecord& order : orders) {
        file << order.id << '|'
             << cleanOrderField(order.userLogin) << '|'
             << fixed << setprecision(2) << order.total << '|'
             << cleanOrderField(order.status) << '|'
             << cleanOrderField(order.address) << '|'
             << cleanOrderField(order.paymentStatus) << '|';

        for (const OrderItemRecord& item : order.items) {
            file << item.productId << '#'
                 << cleanOrderField(item.productName) << '#'
                 << cleanOrderField(item.shopLogin) << '#'
                 << item.quantity << '#'
                 << fixed << setprecision(2) << item.price << ';';
        }

        file << '\n';
    }
}

int getNextOrderId(const vector<OrderRecord>& orders) {
    int nextId = 1;
    for (const OrderRecord& order : orders) {
        if (order.id >= nextId) {
            nextId = order.id + 1;
        }
    }
    return nextId;
}

OrderRecord* findOrder(vector<OrderRecord>& orders, int orderId) {
    for (OrderRecord& order : orders) {
        if (order.id == orderId) {
            return &order;
        }
    }
    return nullptr;
}

string askOrderUser() {
    string userLogin;
    cout << "User login: ";
    getline(cin >> ws, userLogin);
    return userLogin;
}

void OrderManager::createOrder() {
    string userLogin = askOrderUser();
    createOrder(userLogin);
}

void OrderManager::createOrder(string userLogin) {
    string address;

    cout << "Delivery address: ";
    getline(cin >> ws, address);

    vector<OrderCartItemRecord> cart = loadOrderCart();
    vector<OrderProductRecord> products = loadOrderProducts();
    vector<OrderRecord> orders = loadOrders();
    OrderRecord order;
    order.id = getNextOrderId(orders);
    order.userLogin = userLogin;
    order.address = address;
    order.status = "Created";
    order.paymentStatus = "Unpaid";
    order.total = 0.0;

    bool hasItems = false;
    for (const OrderCartItemRecord& cartItem : cart) {
        if (cartItem.userLogin != userLogin) continue;

        OrderProductRecord* product = findOrderProduct(products, cartItem.productId);
        if (product == nullptr) {
            cout << "A product from cart no longer exists. Order cancelled.\n";
            return;
        }

        if (cartItem.quantity <= 0 || cartItem.quantity > product->stock) {
            cout << "Not enough stock for " << product->name << ". Available: " << product->stock << '\n';
            return;
        }

        OrderItemRecord item;
        item.productId = product->id;
        item.productName = product->name;
        item.shopLogin = product->shopLogin;
        item.quantity = cartItem.quantity;
        item.price = product->price;
        order.items.push_back(item);
        order.total += product->price * cartItem.quantity;
        product->stock -= cartItem.quantity;
        hasItems = true;
    }

    if (!hasItems) {
        cout << "Cart is empty.\n";
        return;
    }

    cart.erase(remove_if(cart.begin(), cart.end(), [&](const OrderCartItemRecord& item) {
        return item.userLogin == userLogin;
    }), cart.end());

    orders.push_back(order);
    saveOrders(orders);
    saveOrderProducts(products);
    saveOrderCart(cart);

    cout << "Order created. ID: " << order.id << ", total: " << order.total << '\n';
}

void OrderManager::cancelOrder() {
    string userLogin = askOrderUser();
    cancelOrder(userLogin);
}

void OrderManager::cancelOrder(string userLogin) {
    int orderId;

    cout << "Order ID: ";
    cin.clear();
    orderId = safeInput<int>(1);

    vector<OrderRecord> orders = loadOrders();
    vector<OrderProductRecord> products = loadOrderProducts();

    OrderRecord* order = findOrder(orders, orderId);
    if (order == nullptr || order->userLogin != userLogin) {
        cout << "Order not found.\n";
        return;
    }

    if (order->status == "Completed" || order->status == "Cancelled") {
        cout << "Order cannot be cancelled.\n";
        return;
    }

    for (const OrderItemRecord& item : order->items) {
        OrderProductRecord* product = findOrderProduct(products, item.productId);
        if (product != nullptr) {
            product->stock += item.quantity;
        }
    }

    order->status = "Cancelled";
    saveOrders(orders);
    saveOrderProducts(products);
    cout << "Order cancelled.\n";
}

void OrderManager::trackOrder() {
    string userLogin = askOrderUser();
    trackOrder(userLogin);
}

void OrderManager::trackOrder(string userLogin) {
    int orderId;

    cout << "\nOrder ID: ";
    cin.clear();
    orderId = safeInput<int>(1);

    vector<OrderRecord> orders = loadOrders();
    OrderRecord* order = findOrder(orders, orderId);

    if (order == nullptr || order->userLogin != userLogin) {
        cout << "Order not found.\n";
        return;
    }

    cout << "Order #" << order->id << '\n'
         << "Status: " << order->status << '\n'
         << "Payment: " << order->paymentStatus << '\n'
         << "Address: " << order->address << '\n'
         << "Total: " << order->total << '\n';
}

void OrderManager::updateOrderStatus() {
    int orderId;
    string status;

    cout << "\nOrder ID: ";
    orderId = safeInput<int>(1);
    cout << "New status: ";
    getline(cin >> ws, status);

    vector<OrderRecord> orders = loadOrders();
    OrderRecord* order = findOrder(orders, orderId);

    if (order == nullptr) {
        cout << "Order not found.\n";
        return;
    }

    order->status = status;
    saveOrders(orders);
    cout << "Order status updated.\n";
}

void OrderManager::showUserOrders() {
    string userLogin = askOrderUser();
    showUserOrders(userLogin);
}

void OrderManager::showUserOrders(string userLogin) {
    vector<OrderRecord> orders = loadOrders();
    bool found = false;

    cout << "\n===== USER ORDERS =====\n";
    for (const OrderRecord& order : orders) {
        if (order.userLogin != userLogin) continue;

        cout << "Order #" << order.id << " | " << order.status
             << " | " << order.paymentStatus
             << " | Total: " << order.total << '\n';
        for (const OrderItemRecord& item : order.items) {
            cout << "  " << item.productName << " x" << item.quantity << '\n';
        }
        found = true;
    }

    if (!found) cout << "No orders.\n";
}

void OrderManager::showShopOrders() {
    string shopLogin;

    cout << "Shop login: ";
    getline(cin >> ws, shopLogin);

    vector<OrderRecord> orders = loadOrders();
    bool found = false;

    cout << "\n===== SHOP ORDERS =====\n";
    for (const OrderRecord& order : orders) {
        bool printedHeader = false;
        for (const OrderItemRecord& item : order.items) {
            if (item.shopLogin != shopLogin) continue;

            if (!printedHeader) {
                cout << "Order #" << order.id << " | User: " << order.userLogin
                     << " | Status: " << order.status
                     << " | Payment: " << order.paymentStatus << '\n';
                printedHeader = true;
                found = true;
            }

            cout << "  " << item.productName << " x" << item.quantity << '\n';
        }
    }

    if (!found) cout << "No shop orders.\n";
}
