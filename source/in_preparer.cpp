#include "in_preparer.h"

using namespace std;

const map<int, char> Shell::strmap = {{0, 'S'},
                                      {1, 'P'},
                                      {2, 'D'},
                                      {3, 'F'},
                                      {4, 'G'},
                                      {5, 'H'},
                                      {6, 'J'}};

Shell char2shell(const char &c) {
    int shlind = Shell::strmap.at(c);
    return Shell(shlind);
}

char shell2char(const Shell &shell) {
    char c = Shell::strmap.at(shell.shl);
    return c;
}

ostream &operator<<(ostream &os, const GTOPW &rhs) {
    os << shell2char(rhs.shl);
    os.right;
    os.width(3);
    os << rhs.size;
    os << "\n";

    for (int i = 0; i < rhs.size; ++i) {
        string spaces = "          ";
        os.width(3);
        os << i + 1;
        os << scientific;
        os.precision(9);
        os << spaces;
        os.width(18);
        os << rhs.exps[i];
        os << spaces;
        os.width(18);
        os << rhs.coefs[i].real();
        os.width(18);
        os << rhs.coefs[i].imag();
        os << spaces;
        os << fixed;
        os.precision(5);
        os.width(10);
        os << rhs.k[0];
        os.width(10);
        os << rhs.k[1];
        os.width(10);
        os << rhs.k[2];
        os << "\n";
    }
}

bool read(std::istream &is, GTOPW &out_gtopw) {
    string line;
    if (!getline(is, line))
        return false;

    stringstream ss;
    ss << line;
    char moment   = ss.get();
    cerr << moment << endl;
    out_gtopw.shl = char2shell(moment);
    ss >> out_gtopw.size;
    out_gtopw.exps.empty();
    out_gtopw.coefs.empty();
    out_gtopw.exps.reserve(out_gtopw.size);
    out_gtopw.coefs.reserve(out_gtopw.size);

    for (int i = 0; i < out_gtopw.size; ++i) {
        ss.clear();
        getline(is, line);
        ss << line;
        double exp, re, im;
        int gnum;
        ss >> gnum;
        if (gnum != i + 1)
            throw runtime_error("Invalind gtopw contraction read.");

        ss >> exp >> re >> im;
        out_gtopw.exps.push_back(exp);
        out_gtopw.coefs.push_back(cdouble(re, im));

        double k0, k1, k2;
        ss >> k0 >> k1 >> k2;
        if (i == 0)
            out_gtopw.k = {k0, k1, k2};
        else if (k0 != out_gtopw.k[0] || k1 != out_gtopw.k[1] || k2 != out_gtopw.k[2])
            throw runtime_error("Invalind gtopw contraction read - check k.");
    }
}