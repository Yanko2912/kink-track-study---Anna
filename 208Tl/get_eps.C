
#include "../../software/MiModule/include/MiEvent.h"
#include "../../software/MiModule/include/MiVertex.h"
#include "../../software/MiModule/include/MiVector3D.h"
#include "../../software/MiModule/include/MiPTD.h"
#include "../../software/MiModule/include/MiCD.h"

#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TString.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>

R__LOAD_LIBRARY(../../software/MiModule/lib/libMiModule.so);

void get_eps()
{
    const char* cuts[3] = {"cutsSS", "cutsSK", "cutsKK"};

    // ???? ??? ??????
    std::ofstream ofs("DATA/get_eps_summary.txt");
    if (!ofs) {
        std::cerr << "[ERR] cannot open output file DATA/get_eps_summary.txt" << std::endl;
        return;
    }

    ofs << "# get_eps summary, totE in [2700, 3500] keV\n";
    ofs << std::left << std::setw(8) << "DATA"
        << std::setw(8) << "cut"
        << std::setw(12) << "passed"
        << std::setw(12) << "entries"
        << std::setw(12) << "eps(%)"
        << "\n";
    ofs << "-------------------------------------------------------------\n";

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            TString folder = Form("DATA/%d/%s", i, cuts[j]);
            TString infile = folder + "/Default.root";
            std::cout << "Processing folder " << folder << std::endl;

            TFile* f = TFile::Open(infile, "READ");
            if (!f || f->IsZombie()) {
                std::cout << "  -> cannot open file " << infile << std::endl;
                if (f) { f->Close(); delete f; }
                continue;
            }

            TTree* t = (TTree*)f->Get("Event");
            if (!t) {
                std::cout << "  -> no tree 'Event' in " << infile << std::endl;
                f->Close(); delete f;
                continue;
            }

            MiEvent* Eve = new MiEvent();
            t->SetBranchAddress("Eventdata", &Eve);

            Long64_t nentries = t->GetEntries();
            Long64_t passed = 0;

            for (Long64_t ie = 0; ie < nentries; ie++)
            {
                t->GetEntry(ie);
                double totE = Eve->gettotE(); // ? MeV
                if (totE >= 2700.0 && totE <= 3500.0) {
                    passed++;
                }
            }

            double eps = (double)passed / 200000.0 * 100.0;

            std::cout << "Folder " << i << ", " << cuts[j]
                      << ": eps = " << eps << " %  (passed=" << passed
                      << ", entries=" << nentries << ")\n";

            ofs << std::left << std::setw(8) << i
                << std::setw(8) << cuts[j]
                << std::setw(12) << passed
                << std::setw(12) << nentries
                << std::setw(12) << std::fixed << std::setprecision(6) << eps
                << "\n";

            f->Close();
            delete f;
        }
    }

    ofs.close();
    std::cout << "[done] results saved to DATA/get_eps_summary.txt" << std::endl;
}
