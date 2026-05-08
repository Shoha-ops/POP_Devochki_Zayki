#include "User.h"
#include <iostream>

using namespace std;

User::User()
{
}

User::User(int id,
           string name,
           string login,
           string password,
           string email)
{
    this->id = id;

    this->name = name;
    this->login = login;
    this->password = password;
    this->email = email;
}

int User::getId()
{
    return id;
}

string User::getName()
{
    return name;
}

string User::getLogin()
{
    return login;
}

string User::getPassword()
{
    return password;
}

string User::getEmail()
{
    return email;
}

void User::setName(string name)
{
    this->name = name;
}

void User::setPassword(string password)
{
    this->password = password;
}

void User::setEmail(string email)
{
    this->email = email;
}

void User::showInfo()
{
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Login: " << login << endl;
    cout << "Email: " << email << endl;
}