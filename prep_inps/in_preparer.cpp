#include "in_preparer.h"

using namespace std;

const map<int, char> Shell::strmap = {{0, 'S'},
                                      {1, 'P'},
                                      {2, 'D'},
                                      {3, 'F'},
                                      {4, 'G'},
                                      {5, 'H'},
                                      {6, 'J'}};

Shell string2shell(const string &str) {
    int shlind = Shell::strmap.at(str.at(0));
    return Shell(shlind);
}

char shell2char(const Shell &shell) {
    char c = Shell::strmap.at(shell.shl);
    return c;
}

ostream &operator<<(ostream &os, const GTOPW &rhs) {
    os << shell2char(rhs.shl);
    os << right;
    os.width(3);
    os << rhs.size;
    os << "\n";

    for (int i = 0; i < rhs.size; ++i) {
        os.width(3);
        os << i + 1;
        os << scientific << setprecision(9);
        os.width(20);
        os << rhs.exps[i];
        os << setw(7);
        os.width(19);
        os << rhs.coefs[i].real();
        os.width(19);
        os << rhs.coefs[i].imag();
        os << fixed << setprecision(5);
        os << setw(7);
        os.width(10);
        os << rhs.k[0];
        os.width(10);
        os << rhs.k[1];
        os.width(10);
        os << rhs.k[2];
        os << "\n";
    }
}