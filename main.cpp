#include "UserManager.h"
#include "check.h"
#include "Core Classes.h"
#include "Data Storage.h"
#include "User/user.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    string username, password;
    UserManager userManager;

    cout << "Welcome to the Shopping System!" << endl;

    while (true) {
        int choice;
        string message;
        cout << "\nMenu:" << endl;
        cout << "1 - Register" << endl;
        cout << "2 - Log in" << endl;
        cout << "0 - Exit" << endl;
        cout << "Choose an option: ";
        choice = safeInput<int>();

        if (choice == 0) {
            cout << "Goodbye!" << endl;
            return 0;
        }

        if (choice == 1) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Choose the role (1 - Customer, 2 - Seller): ";
            int roleChoice = safeInput<int>();


            bool registered = userManager.registerUser(username, password, message, roleChoice);
            cout << message << endl;

            if (registered) {
                cout << "Now you can log in using your credentials." << endl;
            }
        } else if (choice == 2) {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (userManager.login(username, password)) {
                cout << "Login successful. Welcome, " << username << "!" << endl;
            } else {
                cout << "Invalid username or password." << endl;
            }
        } else {
            cout << "Invalid option. Try again." << endl;
        }
    }

    return 0;
}
