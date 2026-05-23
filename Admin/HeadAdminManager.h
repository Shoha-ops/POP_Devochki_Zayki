#pragma once

#include "AdminManager.h"

class HeadAdminManager : public AdminManager{
    string head_login="headadmin";
    string head_password="676767";
    public:
    explicit HeadAdminManager(const string& filePath = "admins.txt")
        : AdminManager(filePath)
    {}
    bool loginHeadAdmin(const string& login, const string& password) {
        if (login == head_login && password == head_password) {
            loggedIn     = true;
            currentLogin = head_login;
            currentRole  = "headadmin";
            return true;
        }
        return false;
    }
    void logoutHeadAdmin() {
        logoutAdmin();}
};