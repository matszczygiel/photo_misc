#include <fstream>
#include <iomanip>
#include <iostream>

#include "in_preparer.h"

using namespace std;

int main() {
    vector<double> kvec = {0.651, 1.451, 1.650, 0.750, 0.663, 0.624, 0.624,
                           0.540, 0.427, 0.338, 0.338, 0.322, 0.143, 1.850,
                           1.123, 1.067, 1.043, 1.043, 0.995, 0.939, 0.901,
                           0.901, 0.895, 0.848, 0.685, 0.590, 0.590, 0.562,
                           0.562, 0.493, 0.364, 0.364, 0.232, 2.050, 1.428,
                           1.384, 1.366, 1.366, 1.330, 1.288, 1.262, 1.262,
                           1.257, 1.224, 1.118, 1.062, 1.062, 1.046, 1.046,
                           1.011, 0.955, 0.955, 0.913, 0.873, 0.873, 0.864,
                           0.864, 0.862, 0.548};
    for (const auto& x : kvec) {
        Inputs_preparer preparer(1.4, 1.4, 1, 10);
        preparer.read_basis("/home/mateusz/Documents/cpp/photo/data/h2/dofOpt/basis.inp");
        preparer.set_kval(x);
        std::stringstream stream;
        stream << fixed << setprecision(3) << x;
        string k_str = stream.str();
        string fit   = string("/home/mateusz/workspace/photo_fit/output/") + "fit_z1_k" + k_str + ".dat";
        preparer.read_continuum(fit);
        preparer.prepare("/home/mateusz/workspace/photo_misc/inps/", Shell(3));
    }
}
