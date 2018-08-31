#include <fstream>
#include <iomanip>
#include <iostream>

#include "in_preparer.h"

using namespace std;

int main() {
    vector<double> kvec = {1.626, 0.694, 0.599, 0.552, 0.452, 0.280, 0.141, 0.109};
    for (const auto& x : kvec) {
        Inputs_preparer preparer(1.4, 1.4, 1, 10);
        preparer.read_basis("/home/mateusz/workspace/photo_misc/basis.inp");
        preparer.set_kval(x);
        std::stringstream stream;
        stream << fixed << setprecision(3) << x;
        string k_str = stream.str();
        string fit   = string("/home/mateusz/Documents/cpp/combined_fit/output2/") + "fit_z1_k" + k_str + ".dat";
        preparer.read_continuum(fit);
        preparer.prepare("/home/mateusz/workspace/photo_misc/inps/");
    }
}
