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
        if (!gamess_n.is_open())
            throw std::runtime_error("Cannot create gamess_n input file.");

        std::string gamess_ionized = static_cast<std::string>(argv[3]) + "/h2p_R" + to_string(r_ind_start) + ".inp";
        ofstream gamess_i(gamess_ionized);
        if (!gamess_i.is_open())
            throw std::runtime_error("Cannot create gamess_i input file.");

        gamess_n << " $CONTRL";
        gamess_n << " SCFTYP=RHF RUNTYP=ENERGY ICHARG=0 MAXIT=200 MULT=1\n  COORD=UNIQUE EXETYP=RUN ICUT=12 ISPHER=-1 QMTTOL=1e-8\n  UNITS=BOHR NPRINT=0 CITYP=ALDET";
        gamess_n << " $END\n";
        gamess_n << " $SCF";
        gamess_n << " FDIFF=.F. CONV=1.0D-8 NPUNCH=0 DIIS=.T.\n  SOSCF=.F. DIRSCF=.F. DAMP=.F. NOCONV=.F. EXTRAP=.F.";
        gamess_n << " $END\n";
        gamess_n << " $SYSTEM";
        gamess_n << " MWORDS=200 KDIAG=0";
        gamess_n << " $END\n";
        gamess_n << " $TRANS";
        gamess_n << " CUTTRF=1.0D-14";
        gamess_n << " $END\n";
        gamess_n << " $GUESS";
        gamess_n << " GUESS=HCORE";
        gamess_n << " $END\n";
        gamess_n << " $CIDET";
        gamess_n << " NCORE=0 NACT=" << (2 * basis.functions_number() < 100 ? 2 * basis.functions_number() : 100) << " NELS=2 SZ=0 NSTATE=1 PRTTOL=0.000000000001";
        gamess_n << " $END\n";
        gamess_n << " $CIDRT";
        gamess_n << " NFZC=0 NDOC=1 NVAL=25 IEXCIT=2";
        gamess_n << " $END\n";
        gamess_n << " $DATA\n";
        gamess_n << " Title\n";
        gamess_n << " C1\n";

        gamess_i << " $CONTRL";
        gamess_i << " SCFTYP=UHF RUNTYP=ENERGY ICHARG=1 MAXIT=200 MULT=2\n  COORD=UNIQUE EXETYP=RUN ICUT=12 ISPHER=-1 QMTTOL=1e-8\n  UNITS=BOHR NPRINT=0";
        gamess_i << " $END\n";
        gamess_i << " $SCF";
        gamess_i << " FDIFF=.F. CONV=1.0D-8 NPUNCH=0 DIIS=.T.\n  SOSCF=.F. DIRSCF=.F. DAMP=.F. NOCONV=.F. EXTRAP=.F.";
        gamess_i << " $END\n";
        gamess_i << " $SYSTEM";
        gamess_i << " MWORDS=200 KDIAG=0";
        gamess_i << " $END\n";
        gamess_i << " $TRANS";
        gamess_i << " CUTTRF=1.0D-14";
        gamess_i << " $END\n";
        gamess_i << " $GUESS";
        gamess_i << " GUESS=HCORE";
        gamess_i << " $END\n";
        gamess_i << " $DATA\n";
        gamess_i << " Title\n";
        gamess_i << " C1\n";

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
                if (!gtopw.is_open())
                    throw std::runtime_error("Cannot create gtopw input file.");

                gtopw << "$INTS\n";
                //    ofs << "4\nSTVH\nDIPOLE\nVELOCITY\nERI\n";
                gtopw << "3\nSTVH\nDIPOLE\nVELOCITY\n";
                gtopw << "$END\n";
                gtopw << "$POINTS\n";
                gtopw << "1\n0.000 0.000 0.000\n";
                gtopw << "$END\n";
                gtopw << "$BASIS\n";

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
