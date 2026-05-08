#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Shop {
    int id;
    string name;
    vector<Product> products;

public:
    void addProduct(Product p);
    void updateStock(int id, int qty);
};