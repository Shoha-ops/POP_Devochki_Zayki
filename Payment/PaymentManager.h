#pragma once

#include <string>

using namespace std;

class PaymentManager {
public:
    void makePayment();

    void makePayment(string userLogin);

    void refundPayment();

    void showPaymentHistory();

    void showPaymentHistory(string userLogin);
};
