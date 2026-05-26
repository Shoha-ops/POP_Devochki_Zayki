#pragma once

#include <string>

using namespace std;

class User
{
private:
    int id;

    string name;
    string login;
    string password;
    string email;

public:
    User();

    User(int id,
         string name,
         string login,
         string password,
         string email);

    int getId();

    string getName();
    string getLogin();
    string getPassword();
    string getEmail();

    void setName(string name);
    void setPassword(string password);
    void setEmail(string email);

    void showInfo();
};
