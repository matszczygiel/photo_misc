#ifndef IN_PREPARER_H
#define IN_PREPARER_H

#include <array>
#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "basis.h"

class Inputs_preparer {
   public:
    Inputs_preparer(const double &r_min = 0.8, const double &r_max = 4.0, const int &n_points = 15,
                    const int &n_kvals = 10);

    void read_basis(const std::string &path);
    void punch_gamess_setings_neutral(std::ofstream &ofs) const;
    void punch_gamess_setings_ionized(std::ofstream &ofs) const;
    void punch_gtopw_setings(std::ofstream &ofs) const;
    void set_kval(const double &k) { kval = k; };
    void read_continuum(const std::string &path);

    void prepare(const std::string &out_path, const Shell& cont_max_l);

   private:
    Basis basis;
    Basis cont;
    std::vector<double> rvals;
    std::vector<double> ktheta;
    double kval;
};

#endif
