#include "Shop.h"

Shop::Shop()
{
    approved = false;
}

string Shop::getLogin()
{
    return login;
}

string Shop::getPassword()
{
    return password;
}

bool Shop::isApproved()
{
    return approved;
}

void Shop::approve()
{
    approved = true;
}