#include "in_preparer.h"

using namespace std;

Inputs_preparer::Inputs_preparer(const double &r_min = 0.8, const double &r_max = 4.0, const int &n_points = 15,
                                 const int &n_kvals = 10)
    : basis(), cont(), kval(0) {
    double step = (r_max - r_min) / (n_points - 1);
    for (int i = 0; i < n_points; ++i)
        rvals.push_back(r_min + i * step);

    double theta_step = (M_PI / 2) / (n_kvals - 1);
     for (int i = 0; i < n_kvals; ++i)
        ktheta.push_back(i * theta_step);   

    path_tmp   = "/home/mateusz/workspace/photo_misc/tmp_data/";
    path_cfits = "/home/mateusz/Documents/cpp/combined_fit/output2/";
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

void Inputs_preparer::punch_gamess_setings(ofstream &ofs) const {
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
    ofs << " NCORE=0 NACT=" << 2 * basis.functions_number() << " NELS=2 SZ=0 NSTATE=1 PRTTOL=0.000000000001";
    ofs << " $END\n";
    ofs << " $CIDRT";
    ofs << " NFZC=0 NDOC=1 NVAL=25 IEXCIT=2";
    ofs << " $END\n";
    ofs << " $DATA\n";
    ofs << " Title\n";
    ofs << " C1\n";
}

void Inputs_preparer::punch_gtopw_setings(ofstream &ofs) const {
    if (!ofs.is_open())
        throw runtime_error("GTOPW input file is not open.");

    ofs << "$INTS\n";
    ofs << "4\nSTVH\nDIPOLE\nVELOCITY\nERI\n";
    ofs << "$END\n";
    ofs << "$POINTS\n";
    ofs << "1\n0.000 0.000 0.000\n";
    ofs << "$END\n";
    ofs << "$BASIS\n";
}