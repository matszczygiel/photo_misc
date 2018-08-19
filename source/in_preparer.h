#ifndef IN_PREPARER_H
#define IN_PREPARER_H

#include <array>
#include <cassert>
#include <complex>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

typedef std::array<double, 3> Vec3d;
typedef std::complex<double> cdouble;

class Shell {
   public:
    static const std::array<int, 11> crt_siz;
    static const std::array<int, 11> labels;
    static const std::map<char, int> charmap;

    enum Shells { S = 0,
                  P = 1,
                  D = 2,
                  F = 3,
                  G = 4,
                  H = 5,
                  J = 6
    };

    friend Shell char2shell(const char &c);
    friend char shell2char(const Shell &shell);
    friend int shell2int(const Shell &shell);

    Shell(const Shell &shell) = default;
    Shell(const int &shell) {
        shl = static_cast<Shells>(shell);
    }

   private:
    Shells shl;
};

class GTOPW {
   public:
    GTOPW(const std::vector<double> &exponents,
          const std::vector<cdouble> &coefficients,
          const Shell &shell,
          const Vec3d kvec)
        : shl(shell) {
        assert(exponents.size() == coefficients.size());

        size  = exponents.size();
        exps  = exponents;
        coefs = coefficients;
        k     = kvec;
    };

    GTOPW() : exps(), coefs(), shl(0), size(0) {
        k = {0, 0, 0};
    }

    friend std::ostream &operator<<(std::ostream &os, const GTOPW &rhs);
    bool read(std::istream &is);
    int functions_number();

   private:
    std::vector<double> exps;
    std::vector<cdouble> coefs;
    Shell shl;
    Vec3d k;
    int size;
};

class Basis {
   public:
    Basis() : gtopws(), atom() {
        position = {0, 0, 0};
    }

    bool read(std::istream &is);

   private:
    std::vector<GTOPW> gtopws;
    std::string atom;
    Vec3d position;
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
* /
#endif
