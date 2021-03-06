#include <fstream>
#include <iomanip>
#include <iostream>

#include "basis.h"

using namespace std;

int main(int argc, char* argv[]) {
    const std::string photo_fit_out_path = "/home/mateusz/workspace/photo_fit/output/";

    const std::string setting = argv[1];

    if (setting == "-gms") {
        if (argc != 4) {
            std::cout << "Usage: ./main -gms <basis_path> <out_path>\n";
            return 0;
        }

        Basis basis;

        ifstream file(argv[2]);
        if (!file.is_open())
            throw runtime_error("Invalid input basis file.");

        basis.read(file);
        file.close();

        std::string gamess_neutral = static_cast<std::string>(argv[3]) + "/h.inp";
        ofstream gamess_n(gamess_neutral);

        punch_gms_neutral_header_one_electron(gamess_n);

        basis.set_position({0, 0, 0});
        gamess_n << basis;
        gamess_n << "$END\n";
        gamess_n.close();

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
            Basis basis;
            Basis cont;

            ifstream file(argv[2]);
            if (!file.is_open())
                throw runtime_error("Invalid input basis file.");

            basis.read(file);
            file.close();

            std::stringstream stream;
            stream << fixed << setprecision(3) << kval;
            string k_str = stream.str();
            string fit   = photo_fit_out_path + "fit_z1_k" + k_str + ".dat";
            std::cout << k_str << "\n";

            file.open(fit);
            if (!file.is_open())
                throw runtime_error("Invalid continuum file.");

            cont.read(file);
            file.close();

            cont.truncate_at(Shells::I);
            cont.set_position({0., 0., 0.});
            cont.set_label("CONT");

            std::string gtopw_f = static_cast<std::string>(argv[3]) + "/h_k" + k_str + ".inp";
            std::ofstream gtopw(gtopw_f);

            punch_xgtopw_header(gtopw);

            cont.set_kvec({kval, 0., 0});
            basis.set_position({0, 0, 0});
            gtopw << basis;
            gtopw << cont;

            gtopw << "$END\n";
            gtopw.close();
        }
        k_file.close();
    } else {
        std::cout << "Usage: ./main -gms <basis_path> <out_path>\n";
        std::cout << "or\n";
        std::cout << "Usage: ./main -xgtopw <basis_path> <out_path> <k_file>\n";
    }
}
