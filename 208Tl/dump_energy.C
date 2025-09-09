#include "../../software/MiModule/include/MiEvent.h"
#include "../../software/MiModule/include/MiSDCaloHit.h"
#include "../../software/MiModule/include/MiPTD.h"

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TString.h>

#include <memory>
#include <vector>
#include <iostream>

R__LOAD_LIBRARY(../../software/MiModule/lib/libMiModule.so)

void dump_energy()
{
  std::vector<TString> procs = {"cutsKK", "cutsSS", "cutsSK"};

  TH1D* hTotalKK = new TH1D("hEnergySpectrumTotal_KK", "Energy spectrum (cutsKK);Energy [keV];Counts", 100, 0, 5000);
  TH1D* hTotalSS = new TH1D("hEnergySpectrumTotal_SS", "Energy spectrum (cutsSS);Energy [keV];Counts", 100, 0, 5000);
  TH1D* hTotalSK = new TH1D("hEnergySpectrumTotal_SK", "Energy spectrum (cutsSK);Energy [keV];Counts", 100, 0, 5000);

  for (TH1D* h : {hTotalKK, hTotalSS, hTotalSK}) {
    h->SetDirectory(nullptr);
  }

 
  Long64_t nEventsKK = 0, nEventsSS = 0, nEventsSK = 0;

  for (int i = 0; i <= 49; ++i) {   // ?????? DATA/0..4
    TString folderBase = Form("DATA/%d", i);

    for (const auto& P : procs) {
      TString folder   = folderBase + "/" + P;
      TString infile   = folder + "/Default.root";
      TString outfile  = folder + "/energy_spectrum.root";

      if (gSystem->AccessPathName(infile, kFileExists)) {
        std::cout << "[skip] missing " << infile << std::endl;
        continue;
      }

      std::unique_ptr<TFile> f(TFile::Open(infile, "READ"));
      if (!f || f->IsZombie()) {
        std::cout << "[warn] bad file " << infile << std::endl;
        continue;
      }

      TTree* t = (TTree*) f->Get("Event");
      if (!t) {
        std::cout << "[warn] no TTree 'Event' in " << infile << std::endl;
        continue;
      }

      MiEvent* Eve = nullptr;
      t->SetBranchAddress("Eventdata", &Eve);

      TH1D* hFolder = new TH1D(Form("hEnergySpectrum_%s_%d", P.Data(), i),
                               Form("Energy spectrum %s folder %d;Energy [keV];Counts", P.Data(), i),
                               10000, 0, 5000);
      hFolder->SetDirectory(nullptr);

      const Long64_t nentries = t->GetEntries();
      for (Long64_t ie = 0; ie < nentries; ++ie) {
        t->GetEntry(ie);
        double e = Eve->gettotE();
        hFolder->Fill(e);

        if (P=="cutsKK") { hTotalKK->Fill(e); ++nEventsKK; }
        else if (P=="cutsSS") { hTotalSS->Fill(e); ++nEventsSS; }
        else if (P=="cutsSK") { hTotalSK->Fill(e); ++nEventsSK; }
      }

    
      TFile fOut(outfile, "RECREATE");
      if (!fOut.IsZombie()) {
        TH1D* hToWrite = (TH1D*) hFolder->Clone("hEnergySpectrum");
        hToWrite->SetDirectory(&fOut);
        hToWrite->Write();
        fOut.Close();
      }
      delete hFolder;
    }
  }


  TFile fAll("DATA/energy_spectrum_totals.root", "RECREATE");
  hTotalKK->Write();
  hTotalSS->Write();
  hTotalSK->Write();
  fAll.Close();

  std::cout << "[done] Results saved to DATA/energy_spectrum_totals.root" << std::endl;
 
  std::cout << "[info] counts: KK=" << nEventsKK
            << " SS=" << nEventsSS
            << " SK=" << nEventsSK << std::endl;
}
