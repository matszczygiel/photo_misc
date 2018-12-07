#include <fstream>
#include <iomanip>
#include <iostream>

#include "in_preparer.h"

using namespace std;

int main() {
    vector<double> kvec = {
        0.592,  
        0.700,  
        0.877,  
        1.338 
        };

    for (const auto& x : kvec) {
        Inputs_preparer preparer(1.4, 1.4, 1, 10);
        preparer.read_basis("/home/mateusz/Documents/cpp/photo/data/h2/dofOpt_long/inputs/basis.inp");
        preparer.set_kval(x);
        std::stringstream stream;
        stream << fixed << setprecision(3) << x;
        string k_str = stream.str();
        string fit   = string("/home/mateusz/workspace/photo_fit/output/") + "fit_z1_k" + k_str + ".dat";
        preparer.read_continuum(fit);
        preparer.prepare("/home/mateusz/workspace/photo_misc/inps/", Shell(6));
    }
}
