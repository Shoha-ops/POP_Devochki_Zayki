#include <vector>

#include "../User/User.h"
#include "../Shop/Shop.h"
#include "../Product/Product.h"
#include "../Order/Order.h"

using namespace std;

class Database
{
public:
    vector<User> users;

    vector<Shop> shops;

    vector<Product> products;

    vector<Order> orders;
};