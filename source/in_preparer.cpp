#include "in_preparer.h"

#include <cmath>

using namespace std;

Inputs_preparer::Inputs_preparer(const double &r_min, const double &r_max, const int &rind_start, const int &n_points,
                                 const int &n_kvals)
    : basis(), cont(), kval(0), r_ind_start(rind_start) {
    if (n_points == 1)
        rvals = {r_min};
    else {
        double step = (r_max - r_min) / (n_points - 1);
        for (int i = 0; i < n_points; ++i)
            rvals.push_back(r_min + i * step);
    }
    double theta_step = (M_PI / 2) / (n_kvals - 1);
    for (int i = 0; i < n_kvals; ++i)
        ktheta.push_back(i * theta_step);
}

void Inputs_preparer::read_basis(const string &path) {
    ifstream file(path);
    if (!file.is_open())
        throw runtime_error("Invalid input basis file.");

    basis.read(file);
    file.close();
}

void Inputs_preparer::read_continuum(const string &path) {
    ifstream file(path);
    if (!file.is_open())
        throw runtime_error("Invalid continuum file.");

    cont.read(file);
    file.close();
}

void Inputs_preparer::punch_gamess_setings_neutral(ofstream &ofs) const {
    if (!ofs.is_open())
        throw runtime_error("Gamess input file is not open.");

    ofs << " $CONTRL";
    ofs << " SCFTYP=RHF RUNTYP=ENERGY ICHARG=0 MAXIT=200 MULT=1\n  COORD=UNIQUE EXETYP=RUN ICUT=12 ISPHER=-1 QMTTOL=1e-8\n  UNITS=BOHR NPRINT=0 CITYP=ALDET";
    ofs << " $END\n";
    ofs << " $SCF";
    ofs << " FDIFF=.F. CONV=1.0D-8 NPUNCH=0 DIIS=.T.\n  SOSCF=.F. DIRSCF=.F. DAMP=.F. NOCONV=.F. EXTRAP=.F.";
    ofs << " $END\n";
    ofs << " $SYSTEM";
    ofs << " MWORDS=200 KDIAG=0";
    ofs << " $END\n";
    ofs << " $TRANS";
    ofs << " CUTTRF=1.0D-14";
    ofs << " $END\n";
    ofs << " $GUESS";
    ofs << " GUESS=HCORE";
    ofs << " $END\n";
    ofs << " $CIDET";
    ofs << " NCORE=0 NACT=" << (2 * basis.functions_number() < 100 ? 2 * basis.functions_number() : 100) << " NELS=2 SZ=0 NSTATE=1 PRTTOL=0.000000000001";
    ofs << " $END\n";
    ofs << " $CIDRT";
    ofs << " NFZC=0 NDOC=1 NVAL=25 IEXCIT=2";
    ofs << " $END\n";
    ofs << " $DATA\n";
    ofs << " Title\n";
    ofs << " C1\n";
}

void Inputs_preparer::punch_gamess_setings_ionized(std::ofstream &ofs) const {
    if (!ofs.is_open())
        throw runtime_error("Gamess input file is not open.");

    ofs << " $CONTRL";
    ofs << " SCFTYP=UHF RUNTYP=ENERGY ICHARG=1 MAXIT=200 MULT=2\n  COORD=UNIQUE EXETYP=RUN ICUT=12 ISPHER=-1 QMTTOL=1e-8\n  UNITS=BOHR NPRINT=0";
    ofs << " $END\n";
    ofs << " $SCF";
    ofs << " FDIFF=.F. CONV=1.0D-8 NPUNCH=0 DIIS=.T.\n  SOSCF=.F. DIRSCF=.F. DAMP=.F. NOCONV=.F. EXTRAP=.F.";
    ofs << " $END\n";
    ofs << " $SYSTEM";
    ofs << " MWORDS=200 KDIAG=0";
    ofs << " $END\n";
    ofs << " $TRANS";
    ofs << " CUTTRF=1.0D-14";
    ofs << " $END\n";
    ofs << " $GUESS";
    ofs << " GUESS=HCORE";
    ofs << " $END\n";
    ofs << " $DATA\n";
    ofs << " Title\n";
    ofs << " C1\n";
}

void Inputs_preparer::punch_gtopw_setings(ofstream &ofs) const {
    if (!ofs.is_open())
        throw runtime_error("GTOPW input file is not open.");

    ofs << "$INTS\n";
    //    ofs << "4\nSTVH\nDIPOLE\nVELOCITY\nERI\n";
    ofs << "3\nSTVH\nDIPOLE\nVELOCITY\n";
    ofs << "$END\n";
    ofs << "$POINTS\n";
    ofs << "1\n0.000 0.000 0.000\n";
    ofs << "$END\n";
    ofs << "$BASIS\n";
}

void Inputs_preparer::prepare_xgtopw(const std::string &out_path, const Shell &cont_max_l) {
    cont.truncate_at(cont_max_l);
    cont.set_position({0., 0., 0.});
    cont.set_label("CONT");

    std::stringstream stream;
    stream.precision(3);
    stream << std::fixed << kval;
    std::string kstr = stream.str();
    for (unsigned rind = 0; rind < rvals.size(); ++rind) {
        for (unsigned kind = 0; kind < ktheta.size(); ++kind) {
            std::string gtopw_f = out_path + "/h2_k" + kstr + "_t" + to_string(kind) + "_R" + to_string(r_ind_start + rind) + ".inp";
            std::ofstream gtopw(gtopw_f);
            if (!gtopw.is_open())
                throw std::runtime_error("Cannot create gtopw input file.");

            punch_gtopw_setings(gtopw);
            cont.set_kvec({kval * std::sin(ktheta[kind]), 0., kval * std::cos(ktheta[kind])});
            basis.set_position({0, 0, 0.5 * rvals[rind]});
            gtopw << basis;
            basis.set_position({0, 0, -0.5 * rvals[rind]});
            gtopw << basis;
            gtopw << cont;

            gtopw << "$END\n";
            gtopw.close();
        }
    }
}

void Inputs_preparer::prepare_gms(const std::string &out_path) {
    for (unsigned rind = 0; rind < rvals.size(); ++rind) {
        std::string gamess_neutral = out_path + "/h2_R" + to_string(r_ind_start + rind) + ".inp";
        ofstream gamess_n(gamess_neutral);
        if (!gamess_n.is_open())
            throw std::runtime_error("Cannot create gamess_n input file.");

        std::string gamess_ionized = out_path + "/h2p_R" + to_string(r_ind_start + rind) + ".inp";
        ofstream gamess_i(gamess_ionized);
        if (!gamess_i.is_open())
            throw std::runtime_error("Cannot create gamess_i input file.");

        punch_gamess_setings_neutral(gamess_n);
        punch_gamess_setings_ionized(gamess_i);
        basis.set_position({0, 0, 0.5 * rvals[rind]});
        gamess_n << basis;
        gamess_i << basis;

        basis.set_position({0, 0, -0.5 * rvals[rind]});
        gamess_n << basis;
        gamess_i << basis;

        gamess_n << "$END\n";
        gamess_i << "$END\n";
        gamess_n.close();
        gamess_i.close();
    }
}