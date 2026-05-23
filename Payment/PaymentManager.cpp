#include "PaymentManager.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "../Core/check.h"

using namespace std;

struct PaymentOrderItemRecord {
    int productId;
    string productName;
    string shopLogin;
    int quantity;
    double price;
};

struct PaymentOrderRecord {
    int id;
    string userLogin;
    vector<PaymentOrderItemRecord> items;
    double total;
    string status;
    string address;
    string paymentStatus;
};

struct PaymentRecord {
    int id;
    int orderId;
    string userLogin;
    double amount;
    string method;
    string status;
};

vector<string> splitPaymentText(const string& text, char delimiter) {
    vector<string> parts;
    string part;
    stringstream stream(text);

    while (getline(stream, part, delimiter)) {
        parts.push_back(part);
    }

    return parts;
}

string cleanPaymentField(string value) {
    replace(value.begin(), value.end(), '|', '/');
    replace(value.begin(), value.end(), ';', ',');
    replace(value.begin(), value.end(), '#', '-');
    return value;
}

vector<PaymentOrderRecord> loadPaymentOrders() {
    vector<PaymentOrderRecord> orders;
    ifstream file("orders.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitPaymentText(line, '|');
        if (parts.size() < 7) continue;

        PaymentOrderRecord order;
        order.id = stoi(parts[0]);
        order.userLogin = parts[1];
        order.total = stod(parts[2]);
        order.status = parts[3];
        order.address = parts[4];
        order.paymentStatus = parts[5];

        vector<string> itemParts = splitPaymentText(parts[6], ';');
        for (const string& itemText : itemParts) {
            if (itemText.empty()) continue;
            vector<string> fields = splitPaymentText(itemText, '#');
            if (fields.size() < 5) continue;

            PaymentOrderItemRecord item;
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

void savePaymentOrders(const vector<PaymentOrderRecord>& orders) {
    ofstream file("orders.txt", ios::trunc);
    for (const PaymentOrderRecord& order : orders) {
        file << order.id << '|'
             << cleanPaymentField(order.userLogin) << '|'
             << fixed << setprecision(2) << order.total << '|'
             << cleanPaymentField(order.status) << '|'
             << cleanPaymentField(order.address) << '|'
             << cleanPaymentField(order.paymentStatus) << '|';

        for (const PaymentOrderItemRecord& item : order.items) {
            file << item.productId << '#'
                 << cleanPaymentField(item.productName) << '#'
                 << cleanPaymentField(item.shopLogin) << '#'
                 << item.quantity << '#'
                 << fixed << setprecision(2) << item.price << ';';
        }

        file << '\n';
    }
}

PaymentOrderRecord* findPaymentOrder(vector<PaymentOrderRecord>& orders, int orderId) {
    for (PaymentOrderRecord& order : orders) {
        if (order.id == orderId) {
            return &order;
        }
    }
    return nullptr;
}

bool showPaymentUserOrders(const vector<PaymentOrderRecord>& orders, string userLogin) {
    bool found = false;

    cout << "\n===== YOUR ORDERS =====\n";
    for (const PaymentOrderRecord& order : orders) {
        if (order.userLogin != userLogin) continue;

        cout << "Order #" << order.id
             << " | " << order.status
             << " | " << order.paymentStatus
             << " | Total: " << order.total << '\n';
        found = true;
    }

    if (!found) {
        cout << "No orders for this user.\n";
    }

    return found;
}

vector<PaymentRecord> loadPaymentList() {
    vector<PaymentRecord> payments;
    ifstream file("payments.txt");
    string line;

    while (getline(file, line)) {
        vector<string> parts = splitPaymentText(line, '|');
        if (parts.size() < 6) continue;

        PaymentRecord payment;
        payment.id = stoi(parts[0]);
        payment.orderId = stoi(parts[1]);
        payment.userLogin = parts[2];
        payment.amount = stod(parts[3]);
        payment.method = parts[4];
        payment.status = parts[5];
        payments.push_back(payment);
    }

    return payments;
}

void savePaymentList(const vector<PaymentRecord>& payments) {
    ofstream file("payments.txt", ios::trunc);
    for (const PaymentRecord& payment : payments) {
        file << payment.id << '|'
             << payment.orderId << '|'
             << cleanPaymentField(payment.userLogin) << '|'
             << fixed << setprecision(2) << payment.amount << '|'
             << cleanPaymentField(payment.method) << '|'
             << cleanPaymentField(payment.status) << '\n';
    }
}

int getNextPaymentId(const vector<PaymentRecord>& payments) {
    int nextId = 1;
    for (const PaymentRecord& payment : payments) {
        if (payment.id >= nextId) {
            nextId = payment.id + 1;
        }
    }
    return nextId;
}

void PaymentManager::makePayment() {
    string userLogin;

    cout << "\nUser login: ";
    getline(cin >> ws, userLogin);
    makePayment(userLogin);
}

void PaymentManager::makePayment(string userLogin) {
    int orderId;
    string method;
    vector<PaymentOrderRecord> orders = loadPaymentOrders();

    if (!showPaymentUserOrders(orders, userLogin)) {
        return;
    }

    cout << "\nOrder ID: ";
    cin.clear();
    orderId = safeInput<int>(1);
    cout << "Payment method: ";
    getline(cin >> ws, method);

    PaymentOrderRecord* order = findPaymentOrder(orders, orderId);

    if (order == nullptr || order->userLogin != userLogin) {
        cout << "Order with this ID was not found.\n";
        return;
    }

    if (order->status == "Cancelled") {
        cout << "Cancelled order cannot be paid.\n";
        return;
    }

    if (order->paymentStatus == "Paid") {
        cout << "Order is already paid.\n";
        return;
    }

    vector<PaymentRecord> payments = loadPaymentList();
    PaymentRecord payment;
    payment.id = getNextPaymentId(payments);
    payment.orderId = order->id;
    payment.userLogin = order->userLogin;
    payment.amount = order->total;
    payment.method = method;
    payment.status = "Paid";
    payments.push_back(payment);

    order->paymentStatus = "Paid";
    if (order->status == "Created") {
        order->status = "Paid";
    }

    savePaymentList(payments);
    savePaymentOrders(orders);
    cout << "Payment successful. Amount: " << payment.amount << '\n';
}

void PaymentManager::refundPayment() {
    int orderId;

    cout << "\nOrder ID: ";
    orderId = safeInput<int>(1);

    vector<PaymentOrderRecord> orders = loadPaymentOrders();
    PaymentOrderRecord* order = findPaymentOrder(orders, orderId);

    if (order == nullptr) {
        cout << "Order not found.\n";
        return;
    }

    if (order->paymentStatus != "Paid") {
        cout << "Order is not paid.\n";
        return;
    }

    vector<PaymentRecord> payments = loadPaymentList();
    PaymentRecord refund;
    refund.id = getNextPaymentId(payments);
    refund.orderId = order->id;
    refund.userLogin = order->userLogin;
    refund.amount = -order->total;
    refund.method = "refund";
    refund.status = "Refunded";
    payments.push_back(refund);

    order->paymentStatus = "Refunded";
    order->status = "Refunded";

    savePaymentList(payments);
    savePaymentOrders(orders);
    cout << "Refund completed. Amount: " << order->total << '\n';
}

void PaymentManager::showPaymentHistory() {
    string userLogin;

    cout << "\nUser login: ";
    getline(cin >> ws, userLogin);
    showPaymentHistory(userLogin);
}

void PaymentManager::showPaymentHistory(string userLogin) {
    vector<PaymentRecord> payments = loadPaymentList();
    bool found = false;

    cout << "\n===== PAYMENT HISTORY =====\n";
    for (const PaymentRecord& payment : payments) {
        if (payment.userLogin != userLogin) continue;

        cout << "Payment #" << payment.id
             << " | Order #" << payment.orderId
             << " | " << payment.status
             << " | " << payment.amount
             << " | " << payment.method << '\n';
        found = true;
    }

    if (!found) cout << "No payments.\n";
}
