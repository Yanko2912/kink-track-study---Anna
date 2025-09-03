#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TString.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>

enum NormType { N_NONE=0, N_AREA=1, N_MAX=2 };

static void apply_norm(TH1D* h, NormType nt) {
  if (!h) return;
  if (nt==N_AREA) {
    double I = h->Integral();
    if (I > 0) h->Scale(1.0/I);
  } else if (nt==N_MAX) {
    double m = h->GetMaximum();
    if (m > 0) h->Scale(1.0/m);
  }
}

void show_overlay(const char* norm="raw", int rebin=0, const char* y="lin") {
  // norm: "raw" | "area" | "max"
  // y:    "lin" | "log"
  NormType nt = N_NONE;
  if (TString(norm).EqualTo("area", TString::kIgnoreCase)) nt = N_AREA;
  else if (TString(norm).EqualTo("max", TString::kIgnoreCase)) nt = N_MAX;

  bool logy = TString(y).EqualTo("log", TString::kIgnoreCase);

  TFile f("overlay_energy.root", "READ");
  if (f.IsZombie()) {
    std::cerr << "[ERROR] can't open overlay_energy.root ? run combine_all_energy.C first\n";
    return;
  }

  TH1D* h0nu   = (TH1D*) f.Get("h_0nu");
  TH1D* h2nu   = (TH1D*) f.Get("h_2nu");
  TH1D* h208Tl = (TH1D*) f.Get("h_208Tl");
  TH1D* h214Bi = (TH1D*) f.Get("h_214Bi");

  if (!h0nu && !h2nu && !h208Tl && !h214Bi) {
    std::cerr << "[ERROR] no histograms in overlay_energy.root\n";
    return;
  }

  auto clone = [](TH1D* h, const char* name)->TH1D* {
    if (!h) return nullptr;
    TH1D* c = (TH1D*) h->Clone(name);
    c->SetDirectory(nullptr);
    return c;
  };

  TH1D* H0  = clone(h0nu,   "H0nu");
  TH1D* H2  = clone(h2nu,   "H2nu");
  TH1D* HTl = clone(h208Tl, "H208Tl");
  TH1D* HBi = clone(h214Bi, "H214Bi");

  if (rebin > 1) {
    if (H0)  H0->Rebin(rebin);
    if (H2)  H2->Rebin(rebin);
    if (HTl) HTl->Rebin(rebin);
    if (HBi) HBi->Rebin(rebin);
  }

  apply_norm(H0,  nt); apply_norm(H2,  nt);
  apply_norm(HTl, nt); apply_norm(HBi, nt);

  // === ????-??????? ?? X (?????????? ??????) ===
  std::vector<TH1D*> allH = {H0,H2,HTl,HBi};
  int first = INT_MAX, last = -1;
  TH1D* ref = nullptr;
  for (auto* h: allH) { if (h) { ref = h; break; } }
  if (ref) {
    for (auto* h: allH) {
      if (!h) continue;
      int nb = h->GetNbinsX();
      for (int i=1;i<=nb;++i) {
        if (h->GetBinContent(i) > 0.0) {
          if (i < first) first = i;
          if (i > last)  last  = i;
        }
      }
    }
    if (last >= first) {
      double xmin = ref->GetXaxis()->GetBinLowEdge(first);
      double xmax = ref->GetXaxis()->GetBinUpEdge(last);
      double pad  = 0.05*(xmax - xmin);     // 5% ?????
      double xlo = xmin - pad;
      double xhi = xmax + pad;
      for (auto* h: allH) if (h) h->GetXaxis()->SetRangeUser(xlo, xhi);
    }
  }
  // =============================================

  gStyle->SetOptStat(0);
  TCanvas* c = new TCanvas("c_overlay_show", "Overlay", 1100, 850);
  if (logy) c->SetLogy();

  if (H0)  { H0->SetLineColor(kBlue+1);    H0->SetLineWidth(2); }
  if (H2)  { H2->SetLineColor(kRed+1);     H2->SetLineWidth(2); }
  if (HTl) { HTl->SetLineColor(kGreen+2);  HTl->SetLineWidth(2); }
  if (HBi) { HBi->SetLineColor(kMagenta+1); HBi->SetLineWidth(2); }

  double ymax = 0.0;
  if (H0)  ymax = std::max(ymax, H0->GetMaximum());
  if (H2)  ymax = std::max(ymax, H2->GetMaximum());
  if (HTl) ymax = std::max(ymax, HTl->GetMaximum());
  if (HBi) ymax = std::max(ymax, HBi->GetMaximum());
  if (ymax <= 0) ymax = 1.0;

  TH1D* firstH = H0 ? H0 : (H2 ? H2 : (HTl ? HTl : HBi));
  firstH->SetTitle(
    nt==N_AREA ? "Energy spectra (overlay, normalized to area);Energy [keV];Normalized counts" :
    nt==N_MAX  ? "Energy spectra (overlay, normalized to max);Energy [keV];Counts / max" :
                 "Energy spectra (overlay);Energy [keV];Counts"
  );
  firstH->SetMaximum(logy ? ymax*10.0 : ymax*1.20);
  firstH->Draw("HIST");
  if (H2)  H2->Draw("HIST SAME");
  if (HTl) HTl->Draw("HIST SAME");
  if (HBi) HBi->Draw("HIST SAME");

  TLegend* leg = new TLegend(0.68, 0.75, 0.93, 0.93);
  leg->SetBorderSize(0); leg->SetFillStyle(0);
  if (H0)  leg->AddEntry(H0,  "0nu",   "l");
  if (H2)  leg->AddEntry(H2,  "2nu",   "l");
  if (HTl) leg->AddEntry(HTl, "208Tl", "l");
  if (HBi) leg->AddEntry(HBi, "214Bi", "l");
  leg->Draw();

  TString tag = nt==N_AREA ? "_norm" : (nt==N_MAX ? "_max" : "");
  TString ytag = logy ? "_logy" : "";
  TString png = TString::Format("overlay_energy%s%s.png", tag.Data(), ytag.Data());
  c->SaveAs(png);
  std::cout << "[INFO] saved " << png << "\n";
}
