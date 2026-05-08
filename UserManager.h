#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct StoredUser {
    int id;
    string username;
    string password;
};

class UserManager {
    vector<StoredUser> users;
    string storagePath;
    int nextUserId;

public:
    explicit UserManager(const string& filePath = "users.txt")
        : storagePath(filePath), nextUserId(700000000) {
        loadUsers();
    }

    bool registerUser(const string& username, const string& password, string& message, int roleChoice) {
        if (username.empty() || password.empty()) {
            message = "Username and password cannot be empty.";
            return false;
        }

        if (findUser(username) != nullptr) {
            message = "User with this username already exists.";
            return false;
        }

        StoredUser newUser{ nextUserId++, username, password };
        users.push_back(newUser);
        saveUsers();
        message = "Registration successful.";
        return true;
    }

    bool login(const string& username, const string& password) const {
        for (const StoredUser& user : users) {
            if (user.username == username && user.password == password) {
                return true;
            }
        }
        return false;
    }

private:
    void loadUsers() {
        users.clear();
        ifstream inFile(storagePath);
        if (!inFile.is_open()) {
            return;
        }

        string line;
        while (getline(inFile, line)) {
            if (line.empty()) {
                continue;
            }

            stringstream ss(line);
            string idStr;
            string username;
            string password;

            if (!getline(ss, idStr, '|')) {
                continue;
            }
            if (!getline(ss, username, '|')) {
                continue;
            }
            if (!getline(ss, password, '|')) {
                continue;
            }

            StoredUser user;
            user.id = stoi(idStr);
            user.username = username;
            user.password = password;

            users.push_back(user);
            if (user.id >= nextUserId) {
                nextUserId = user.id + 1;
            }
        }
    }

    void saveUsers() const {
        ofstream outFile(storagePath, ios::trunc);
        for (const StoredUser& user : users) {
            outFile << user.id << '|' << user.username << '|' << user.password << '\n';
        }
    }

    const StoredUser* findUser(const string& username) const {
        for (const StoredUser& user : users) {
            if (user.username == username) {
                return &user;
            }
        }
        return nullptr;
    }
};

