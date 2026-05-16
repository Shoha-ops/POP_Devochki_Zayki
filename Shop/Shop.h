#pragma once

#include <string>
#include <vector>

using namespace std;

// Shop entity used by the shop manager and future admin/user integrations.
class Shop {
public:
    // Public fields are kept simple for the template stage.
    int id;

    string shopName;
    string ownerName;

    string login;
    string password;
    string email;

    // The admin flow can flip this when the shop is accepted.
    bool approved;

    // Product and order identifiers are stored as lightweight references.
    vector<int> products;
    vector<int> orders;

    Shop();

    string getLogin();
    string getPassword();
    bool isApproved();
    void approve();
};
