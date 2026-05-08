#include <string>

using namespace std;

class AuthManager
{
public:
    void login();

    void registerAccount();

    void logout();

    bool validatePassword(string password);

    bool checkLoginExists(string login);
};
