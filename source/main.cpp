#include <fstream>
#include <iomanip>
#include <iostream>

#include "in_preparer.h"

using namespace std;

int main(int argc, char* argv[]) {
    const std::string setting = argv[1];

    if (setting == "-gms") {
        if (argc != 4) {
            std::cout << "Usage: ./main -gms <basis_path> <out_path>\n";
            return 0;
        }
        Inputs_preparer preparer(1.4, 1.4, 0, 1, 1);
        preparer.read_basis(argv[2]);
        preparer.prepare_gms(argv[3]);
    } else if (setting == "-xgtopw") {
        if (argc != 5) {
            std::cout << "Usage: ./main -xgtopw <basis_path> <out_path> <k_file>\n";
            return 0;
        }
        std::ifstream k_file(argv[4]);
        if (!k_file.is_open())
            throw std::runtime_error("Cannot open k_file.");

        double kval;
        while (k_file >> kval) {
            Inputs_preparer preparer(1.4, 1.4, 0, 1, 10);
            preparer.read_basis(argv[2]);
            preparer.set_kval(kval);
            std::stringstream stream;
            stream << fixed << setprecision(3) << kval;
            string k_str = stream.str();
            string fit   = string("/home/mateusz/workspace/photo_fit/output/") + "fit_z1_k" + k_str + ".dat";
            preparer.read_continuum(fit);
            preparer.prepare_xgtopw(argv[3], Shell(6));
        }
        k_file.close();
    }
    else
    {
        std::cout << "Usage: ./main -gms <basis_path> <out_path>\n";
        std::cout << "or\n";
        std::cout << "Usage: ./main -xgtopw <basis_path> <out_path> <k_file>\n";
    }
    
}
