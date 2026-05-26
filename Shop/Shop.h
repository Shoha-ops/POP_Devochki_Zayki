#pragma once

#include <string>
#include <vector>

using namespace std;

class Shop {
public:
    int id;

    string shopName;
    string ownerName;

    string login;
    string password;
    string email;

    bool approved;

    vector<int> products;
    vector<int> orders;

    Shop();

    string getLogin();
    string getPassword();
    bool isApproved();
    void approve();
};
