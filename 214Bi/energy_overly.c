#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <iostream>

void energy_overly()
{
  TString inpath = "DATA/energy_spectrum_totals.root";
  TFile* f = TFile::Open(inpath, "READ");
  if (!f || f->IsZombie()) {
    std::cerr << "[error] cannot open " << inpath << std::endl;
    return;
  }
  TH1D* hSS = (TH1D*)f->Get("hEnergySpectrumTotal_SS");
   TH1D* hSK = (TH1D*)f->Get("hEnergySpectrumTotal_SK");
TH1D* hKK = (TH1D*)f->Get("hEnergySpectrumTotal_KK");

 

  if (!hKK || !hSS || !hSK) {
    std::cerr << "[error] missing one of totals: KK/SS/SK" << std::endl;
    f->Close();
    return;
  }

  TH1D* aKK = (TH1D*)hKK->Clone("hKK_plot");
  TH1D* aSS = (TH1D*)hSS->Clone("hSS_plot");
  TH1D* aSK = (TH1D*)hSK->Clone("hSK_plot");
  aKK->SetDirectory(nullptr);
  aSS->SetDirectory(nullptr);
  aSK->SetDirectory(nullptr);

  // ---- ??????: ?????????? ?? ????????? ??????? (????? ~ 1) ----
  const double nKK = aKK->GetEntries();
  const double nSS = aSS->GetEntries();
  const double nSK = aSK->GetEntries();
   if (nSS > 0) aSS->Scale(1.0 / nSS);
   if (nSK > 0) aSK->Scale(1.0 / nSK);
if (nKK > 0) aKK->Scale(1.0 / nKK);
 
 

  aKK->SetLineColor(kBlue);
  aSS->SetLineColor(kRed);
  aSK->SetLineColor(kGreen+1);

  aKK->SetLineWidth(2);
  aSS->SetLineWidth(2);
  aSK->SetLineWidth(2);

  aKK->SetTitle("Energy spectra overlay (normalized);Energy [keV];Normalized counts");

  double ymax = std::max({aKK->GetMaximum(), aSS->GetMaximum(), aSK->GetMaximum()});
  aKK->SetMaximum(ymax * 1.25);

  TCanvas *c1 = new TCanvas("cEnergyOverlay", "Energy spectra overlay", 1100, 850);
  gStyle->SetOptStat(0);

  aKK->Draw("HIST");
  aSS->Draw("HIST SAME");
  aSK->Draw("HIST SAME");

  auto *leg = new TLegend(0.65, 0.72, 0.88, 0.90);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->AddEntry(aSS, "cutsSS", "l");
 leg->AddEntry(aSK, "cutsSK", "l");
  leg->AddEntry(aKK, "cutsKK", "l");
  
 
  leg->Draw();

  c1->SaveAs("DATA/energy_overlay.png");
  c1->SaveAs("DATA/energy_overlay.pdf");

  f->Close();
  std::cout << "[done] Saved: DATA/energy_overlay.png and .pdf" << std::endl;
double intKK = aKK->Integral();
  double intSS = aSS->Integral();
  double intSK = aSK->Integral();
  std::cout << "KK = " << intKK << std::endl;
  std::cout << "SS = " << intSS << std::endl;
  std::cout << "SK = " << intSK << std::endl;
}
