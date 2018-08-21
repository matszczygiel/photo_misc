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
    Inputs_preparer(const double &r_min, const double &r_max, const int &n_points,
                    const int &n_kvals);

    void read_basis(const std::string &path);
    void punch_gamess_setings(std::ofstream &ofs) const;
    void punch_gtopw_setings(std::ofstream &ofs) const;
    void set_kval(const double &k) { kval = k; };

    void read_continuum(const std::string &path);

   private:
    std::string path_tmp;
    std::string path_cfits;

    Basis basis;
    Basis cont;
    std::vector<double> rvals;
    std::vector<double> ktheta;
    double kval;
};

#endif
