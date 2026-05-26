#pragma once

#include <string>

using namespace std;

class PaymentManager {
public:
    void makePayment(string userLogin);

    void refundPayment();

    void showPaymentHistory(string userLogin);
};
