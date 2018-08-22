#include <fstream>
#include <iostream>

#include "in_preparer.h"

using namespace std;

int main() {
    Inputs_preparer preparer(1.4, 1.4, 1, 10);
    preparer.read_basis("/home/mateusz/workspace/photo_misc/basis.inp");
    preparer.set_kval(0.45);
    string fit = string("/home/mateusz/Documents/cpp/combined_fit/output2/") + "fit_z1_k0.450.dat";
    preparer.read_continuum(fit);
    preparer.prepare("/home/mateusz/workspace/photo_misc/inps/");
    
}