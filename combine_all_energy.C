#include "TFile.h"
#include "TH1D.h"
#include "TSystem.h"
#include "TString.h"
#include <iostream>
#include <vector>

void combine_all_energy() {
  // ????? ????????? ????? ????? ?? ????????
  std::vector<TString> cats = {"0nu","2nu","208Tl","214Bi"};

  // ????? ???? ? ???'??? (??? ??????? ????????? ?????)
  std::vector<std::pair<TString, TH1D*>> hs; 
  hs.reserve(cats.size());

  for (const auto& c : cats) {
    TString path = c + "/DATA/energy_spectrum_total.root";
    if (gSystem->AccessPathName(path, kFileExists)) {
      std::cerr << "[WARN] not found: " << path << "\n";
      continue;
    }
    TFile f(path, "READ");
    if (f.IsZombie()) {
      std::cerr << "[WARN] zombie: " << path << "\n";
      continue;
    }
    TH1D* h = (TH1D*) f.Get("hEnergySpectrumTotal");
    if (!h) {
      std::cerr << "[WARN] no 'hEnergySpectrumTotal' in " << path << "\n";
      continue;
    }
    TH1D* hc = (TH1D*) h->Clone(Form("h_%s", c.Data())); // ?????: h_0nu, h_2nu, h_208Tl, h_214Bi
    hc->SetDirectory(nullptr);
    hs.push_back({c, hc});
  }

  if (hs.empty()) {
    std::cerr << "[ERROR] nothing collected ? abort\n";
    return;
  }

  // ????? ? ?????? ????
  TFile out("overlay_energy.root", "RECREATE");
  if (out.IsZombie()) {
    std::cerr << "[ERROR] cannot create overlay_energy.root\n";
    return;
  }
  for (auto& p : hs) {
    TH1D* h = p.second;
    h->SetDirectory(&out);
    h->Write(h->GetName());
  }
  out.Close();

  std::cout << "[INFO] wrote overlay_energy.root with " << (int)hs.size() << " histograms\n";
}
