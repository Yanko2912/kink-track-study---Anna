#include "../../software/MiModule/include/MiEvent.h"
#include "../../software/MiModule/include/MiSDCaloHit.h"
#include "../../software/MiModule/include/MiPTD.h"

#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TSystem.h>
#include <TROOT.h>

#include <vector>
#include <iostream>

R__LOAD_LIBRARY(../../software/MiModule/lib/libMiModule.so)

void dump_energy()
{
  
  TH1D* hEnergySpectrumTotal = new TH1D("hEnergySpectrumTotal",
                                        "Total energy spectrum;Energy [keV];Counts",
                                        100, 0, 5000);
  hEnergySpectrumTotal->SetDirectory(nullptr);

  for (int i = 0; i < 50; ++i) {   // ???? 50 ?????
    TString folder   = Form("DATA/%d", i);
    TString infile   = folder + "/Default.root";
    TString outfile  = folder + "/energy_spectrum.root";

   
    if (gSystem->AccessPathName(infile, kFileExists)) {
      continue;
    }

    
    TFile* f = TFile::Open(infile, "READ");
    if (!f || f->IsZombie()) {
      if (f) f->Close();
      continue;
    }

    TTree* t = (TTree*) f->Get("Event");
    if (!t) {
      f->Close();
      continue;
    }

    MiEvent* Eve = nullptr;
    t->SetBranchAddress("Eventdata", &Eve);

   
    TH1D* hEnergySpectrumFolder = new TH1D(Form("hEnergySpectrumFolder_%d", i),
                                           Form("Energy spectrum folder %d;Energy [keV];Counts", i),
                                           100, 0, 5000);
    hEnergySpectrumFolder->SetDirectory(nullptr);

    const Long64_t nentries = t->GetEntries();
    for (Long64_t ie = 0; ie < nentries; ++ie) {
      t->GetEntry(ie);

      double e = Eve->gettotE();
      hEnergySpectrumFolder->Fill(e);
      hEnergySpectrumTotal->Fill(e);
    }

  
    {
      TFile f_output(outfile, "RECREATE");
      if (!f_output.IsZombie()) {
        TH1D* hToWrite = (TH1D*) hEnergySpectrumFolder->Clone("hEnergySpectrumFolder");
        hToWrite->SetDirectory(&f_output);
        hToWrite->Write();
        f_output.Close();
      }
    }

    f->Close();
  }

  
  {
    TFile f_output_all("DATA/energy_spectrum_total.root", "RECREATE");
    if (!f_output_all.IsZombie()) {
      TH1D* hTotalToWrite = (TH1D*) hEnergySpectrumTotal->Clone("hEnergySpectrumTotal");
      hTotalToWrite->SetDirectory(&f_output_all);
      hTotalToWrite->Write();
      f_output_all.Close();
    }
  }
}
