#include "in_preparer.h"

using namespace std;

const std::map<char, int> Shell::charmap = {{'S', 0},
                                            {'P', 1},
                                            {'D', 2},
                                            {'F', 3},
                                            {'G', 4},
                                            {'H', 5},
                                            {'J', 6}};

const std::array<int, 11> Shell::crt_siz = {1, 3, 6, 10, 15, 21, 28, 36, 45, 55};
const std::array<int, 11> Shell::labels  = {'S', 'P', 'D', 'F', 'G', 'H', 'I'};

Shell char2shell(const char &c) {
    int shlind = Shell::charmap.at(c);
    return Shell(shlind);
}

char shell2char(const Shell &shell) {
    return Shell::labels.at(static_cast<int>(shell.shl));
}

int shell2int(const Shell &shell) {
    return static_cast<int>(shell.shl);
}

ostream &operator<<(ostream &os, const GTOPW &rhs) {
    os << shell2char(rhs.shl);
    os.width(3);
    os << rhs.size;
    os << "\n";
    string spaces = "          ";

    for (int i = 0; i < rhs.size; ++i) {
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
    return os;
}

bool GTOPW::read(std::istream &is) {
    string line;
    if (!getline(is, line))
        return false;

    istringstream ss(line);
    char moment = ss.get();
    shl         = char2shell(moment);
    ss >> size;
    exps.empty();
    coefs.empty();
    exps.reserve(size);
    coefs.reserve(size);

    for (int i = 0; i < size; ++i) {
        getline(is, line);
        istringstream ssl(line);
        double exp, re, im;
        int gnum;
        ssl >> gnum;
        if (gnum != i + 1)
            throw runtime_error("Invalind gtopw contraction read.");

        ssl >> exp >> re >> im;
        exps.push_back(exp);
        coefs.push_back(cdouble(re, im));

        double k0, k1, k2;
        ssl >> k0 >> k1 >> k2;
        if (i == 0)
            k = {k0, k1, k2};
        else if (k0 != k[0] || k1 != k[1] || k2 != k[2])
            throw runtime_error("Invalind gtopw contraction read - check k.");
    }

    return true;
}

int GTOPW::functions_number() {
    return Shell::crt_siz.at(shell2int(shl)) * size;
}

bool Basis::read(istream &is) {
    Basis();
    
    string line;
    if (!getline(is, line))
        return false;

    istringstream ss(line);
    string token;
    ss >> token;
    if (token == "$END")
        return false;

    atom = token;
    ss >> position[0] >> position[1] >> position[2];
    GTOPW g;
    while(g.read()) 
        gtopws.push_back(g);
    
    return true;
}
