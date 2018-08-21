#include <fstream>
#include <iostream>

#include "in_preparer.h"

using namespace std;

int main() {
    ifstream bfile("/home/mateusz/workspace/photo_misc/basis.inp");
    if (!bfile.is_open())
        throw runtime_error("Invalid input basis file.");

    Basis bas;
    bas.read(bfile);
    bfile.close();

    ofstream log;
    log.open("/home/mateusz/workspace/photo_misc/logs/log.dat");
    log << "Basis\n";
    log << bas;

    double rmin = 0.4;
    double rmax = 8.0;
    int npoints = 30;

    double step = (rmax - rmin) / (npoints - 1);

    for (int i = 0; i < npoints; ++i) {
        double r = rmin + i * step;

        ofstream file;
        file.open("/home/mateusz/workspace/photo_misc/h2.inp");

        file << " $CONTRL";
        file << " SCFTYP=RHF RUNTYP=ENERGY ICHARG=0 MAXIT=200 MULT=1\n  COORD=UNIQUE EXETYP=RUN ICUT=12 ISPHER=-1 QMTTOL=1e-8\n  UNITS=BOHR NPRINT=0 CITYP=ALDET";
        file << " $END\n";
        file << " $SCF";
        file << " FDIFF=.F. CONV=1.0D-8 NPUNCH=0 DIIS=.T.\n  SOSCF=.F. DIRSCF=.F. DAMP=.F. NOCONV=.F. EXTRAP=.F.";
        file << " $END\n";
        file << " $SYSTEM";
        file << " MWORDS=200 KDIAG=0";
        file << " $END\n";
        file << " $TRANS";
        file << " CUTTRF=1.0D-14";
        file << " $END\n";
        file << " $GUESS";
        file << " GUESS=HCORE";
        file << " $END\n";
        file << " $CIDET";
        file << " NCORE=0 NACT=" << 2 * bas.functions_number() << " NELS=2 SZ=0 NSTATE=1 PRTTOL=0.000000000001";
        file << " $END\n";
        file << " $CIDRT";
        file << " NFZC=0 NDOC=1 NVAL=25 IEXCIT=2";
        file << " $END\n";
        file << " $DATA\n";
        file << " Title\n";
        file << " C1\n";

        bas.set_position({0, 0, 0.5 * r});
        file << bas;

        bas.set_position({0, 0, -0.5 * r});
        file << bas;

        file << "$END\n";

        file.close();

        system("./run_gamess.sh");

        ifstream enfile("/home/mateusz/workspace/photo_misc/tmp_data/en.dat");
        string dum;
        enfile >> dum >> dum >> dum;
        double energy;
        enfile >> energy;
        enfile.close();

        vector<string> cidat, cdat, edat;
        string line;

        ifstream cifile("/home/mateusz/workspace/photo_misc/tmp_data/ci.dat");
        while (getline(cifile, line))
            cidat.push_back(line);
        cifile.close();

        ifstream cfile("/home/mateusz/workspace/photo_misc/tmp_data/c.dat");
        while (getline(cfile, line))
            cdat.push_back(line);
        cfile.close();

        ifstream efile("/home/mateusz/workspace/photo_misc/tmp_data/e.dat");
        while (getline(efile, line))
            edat.push_back(line);
        efile.close();

        log << "********* Data for photoionization computation *********\n";
        log << "R    " << r << "\n";
        log << "E    " << energy << "\n";
        log << "\n";
        log << "HF_ENERGIES\n";
        for (const auto& x : edat)
            log << x << "\n";
        log << "\n";
        log << "HF_ORBITALS\n";
        for (const auto& x : cdat)
            log << x << "\n";
        log << "\n";
        log << "CI_ORBITALS\n";
        for (const auto& x : cidat)
            log << x << "\n";
        log << "\n";
    }

    log.close();

    //  cin.get();
}