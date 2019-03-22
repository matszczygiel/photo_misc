#include <fstream>
#include <iomanip>
#include <iostream>

#include "basis.h"

using namespace std;

int main(int argc, char* argv[]) {
    constexpr double rval                = 1.4;
    constexpr int r_ind_start            = 0;
    constexpr int n_kvals                = 10;
    const std::string photo_fit_out_path = "/home/mateusz/workspace/photo_fit/output/";

    const std::string setting = argv[1];

    constexpr double theta_step = (M_PI / 2) / (n_kvals - 1);

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

        std::string gamess_neutral = static_cast<std::string>(argv[3]) + "/h2_R" + to_string(r_ind_start) + ".inp";
        ofstream gamess_n(gamess_neutral);

        std::string gamess_ionized = static_cast<std::string>(argv[3]) + "/h2p_R" + to_string(r_ind_start) + ".inp";
        ofstream gamess_i(gamess_ionized);

        punch_gms_neutral_header(gamess_n, 2 * basis.functions_number() < 100 ? 2 * basis.functions_number() : 100);
        punch_gms_ion_header(gamess_i);

        basis.set_position({0, 0, 0.5 * rval});
        gamess_n << basis;
        gamess_i << basis;

        basis.set_position({0, 0, -0.5 * rval});
        gamess_n << basis;
        gamess_i << basis;

        gamess_n << "$END\n";
        gamess_i << "$END\n";
        gamess_n.close();
        gamess_i.close();

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
            std::vector<double> ktheta;

            for (int i = 0; i < n_kvals; ++i)
                ktheta.push_back(i * theta_step);

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

            for (unsigned kind = 0; kind < ktheta.size(); ++kind) {
                std::string gtopw_f = static_cast<std::string>(argv[3]) + "/h2_k" + k_str + "_t" + to_string(kind) + "_R" + to_string(r_ind_start) + ".inp";
                std::ofstream gtopw(gtopw_f);

                punch_xgtopw_header(gtopw);

                cont.set_kvec({kval * std::sin(ktheta[kind]), 0., kval * std::cos(ktheta[kind])});
                basis.set_position({0, 0, 0.5 * rval});
                gtopw << basis;
                basis.set_position({0, 0, -0.5 * rval});
                gtopw << basis;
                gtopw << cont;

                gtopw << "$END\n";
                gtopw.close();
            }
        }
        k_file.close();
    } else {
        std::cout << "Usage: ./main -gms <basis_path> <out_path>\n";
        std::cout << "or\n";
        std::cout << "Usage: ./main -xgtopw <basis_path> <out_path> <k_file>\n";
    }
}
