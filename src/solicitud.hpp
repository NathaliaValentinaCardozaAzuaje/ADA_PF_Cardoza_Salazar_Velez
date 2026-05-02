#ifndef SOLICITUD_HPP
#define SOLICITUD_HPP

#include <string>

using namespace std;

struct Solicitud {
    string customerID;
    int tenure;
    double monthlyCharges;
    double totalCharges;
    string churn;
};

#endif