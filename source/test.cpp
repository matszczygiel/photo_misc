#include <iostream>

#include "in_preparer.h"

using namespace std;

int main() {
    std::vector<double> exp                 = {3.45544, 56.4554, 2234.45};
    std::vector<std::complex<double>> coefs = {
        cdouble(2.343, 23.4324),
        cdouble(0.32424, -435.345),
        cdouble(-0.00234, 0.0234023)};

    GTOPW c(exp, coefs, Shell::P, {0.65, 0., 0.34});

    cout << c;

    GTOPW r;
    read(cin, r);

    cout << r;
}