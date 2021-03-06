#pragma once

#include <array>
#include <cassert>
#include <complex>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using Vec3d   = std::array<double, 3>;
using cdouble = std::complex<double>;

enum class Shells { S = 0,
                    P = 1,
                    D = 2,
                    F = 3,
                    G = 4,
                    H = 5,
                    I = 6,
                    K = 7,
                    L = 8
};

class Shell {
   public:
    static const std::array<int, 11> crt_siz;
    static const std::array<int, 11> labels;
    static const std::map<char, int> charmap;

    friend Shell char2shell(const char &c);
    friend char shell2char(const Shell &shell);
    friend int shell2int(const Shell &shell);

    Shell(const Shell &shell) = default;
    Shell(const int &shell) {
        shl = static_cast<Shells>(shell);
    }
    Shell(const Shells &shell) : shl(shell) {}

   private:
    Shells shl;
};

class GTOPW {
   public:
    GTOPW(const std::vector<double> &exponents,
          const std::vector<cdouble> &coefficients,
          const Shell &shell,
          const Vec3d kvec = {0, 0, 0})
        : shl(shell), size(exponents.size()), exps(exponents), coefs(coefficients), k(kvec) {
        assert(exponents.size() == coefficients.size());
    };

    GTOPW() : exps(), coefs(), shl(0), size(0) {
        k = {0, 0, 0};
    };

    friend std::ostream &operator<<(std::ostream &os, const GTOPW &rhs);
    bool read(std::istream &is);
    int functions_number() const;
    void set_kvec(const Vec3d &kvec) { k = kvec; };
    Shell get_shell() const { return shl; };

   private:
    std::vector<double> exps;
    std::vector<cdouble> coefs;
    Shell shl;
    Vec3d k;
    int size;
};

class Basis {
   public:
    Basis() : gtopws(), label(), charge(0.) {
        position = {0, 0, 0};
    };

    bool read(std::istream &is);
    friend std::ostream &operator<<(std::ostream &os, const Basis &rhs);
    int functions_number() const;
    void set_position(const Vec3d &pos) { position = pos; };
    void set_label(const std::string &lb) { label = lb; };
    Shell get_max_shell() const;
    void truncate_at(const Shell &shl);
    void set_kvec(const Vec3d &kvec);

   private:
    std::vector<GTOPW> gtopws;
    std::string label;
    double charge;
    Vec3d position;
};


void punch_xgtopw_header(std::ofstream &ofs);
void punch_gms_ion_header(std::ofstream &ofs);
void punch_gms_neutral_header(std::ofstream &ofs, const int& active_orbs_ci);
void punch_gms_neutral_header_one_electron(std::ofstream &ofs);