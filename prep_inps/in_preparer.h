#ifndef IN_PREPARER_H
#define IN_PREPARER_H

#include <array>
#include <cassert>
#include <complex>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

typedef std::array<double, 3> Vec3d;
typedef std::complex<double> cdouble;

class Shell {
   public:
    static constexpr int crt_siz[11] = {1, 3, 6, 10, 15, 21, 28, 36, 45, 55};

    static const std::map<int, char> strmap;

    enum Shells { S = 0,
                  P = 1,
                  D = 2,
                  F = 3,
                  G = 4,
                  H = 5,
                  J = 6 };

    friend Shell string2shell(const std::string &str);
    friend char shell2char(const Shell &shell);

    Shell(const Shell &shell) = default;
    Shell(const int &shell) { shl = static_cast<Shells>(shell); }

    int shell_size() const { return crt_siz[shl]; }

   private:
    Shells shl;
};

class GTOPW {
   public:
    GTOPW(const std::vector<double> &exponents,
          const std::vector<cdouble> coefficients,
          const Shell &shell,
          const Vec3d kvec)
        : shl(shell) {
        assert(exponents.size() == coefficients.size());

        size  = exponents.size();
        exps  = exponents;
        coefs = coefficients;
        k     = kvec;
    };

    friend std::ostream &operator<<(std::ostream &os, const GTOPW &rhs);

   private:
    std::vector<double> exps;
    std::vector<cdouble> coefs;
    Shell shl;
    Vec3d k;
    int size;
};
/*
class Basis {
}

class Molecule {
   public:
   private:
}

class Inputs_preparer {
   public:
   private:
    double rmin;
    double rmax;
    int N;
};
*/
#endif