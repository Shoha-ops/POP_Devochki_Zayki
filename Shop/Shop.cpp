#include "Shop.h"

Shop::Shop() {
    id = 0;

    shopName = "";
    ownerName = "";

    login = "";
    password = "";
    email = "";

    approved = false;
}

string Shop::getLogin() {
    return login;
}

string Shop::getPassword() {
    return password;
}

bool Shop::isApproved() {
    return approved;
}

void Shop::approve() {
    approved = true;
}
