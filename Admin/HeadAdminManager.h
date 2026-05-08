#pragma once

#include <string>

using namespace std;

class HeadAdminManager
{
public:
    bool loginHeadAdmin(string login, string password)
    {
        return login == "headadmin" && password == "123";
    }

    void logoutHeadAdmin() {}
};