#include <iostream>
#include <fstream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TString.h"

void get_eps(const char* base_dir = ".", Long64_t total_events = 200000) {
   
    TString search_dir = base_dir;
    {
        TString candidate = TString::Format("%s/DATA", base_dir);
        if (!gSystem->AccessPathName(candidate, kFileExists)) {
            search_dir = candidate; 
        }
    }


    TString table_path = TString::Format("%s/efficiencies.txt", search_dir.Data());
    gSystem->Unlink(table_path);

    auto write_one = [&](const TString& rootpath, const TString& outpath) {
        TFile f(rootpath, "READ");
        if (f.IsZombie()) return;
        TTree* s = (TTree*) f.Get("Event");
        if (!s) return;

        Long64_t N = s->GetEntries();
        double eps = (total_events > 0) ? 100.0 * double(N) / double(total_events) : 0.0;
        (void)eps;

        TString dir    = gSystem->DirName(rootpath.Data());
        TString folder = gSystem->BaseName(dir.Data());

        std::ofstream out(outpath.Data(), std::ios::app);
        if (!out) { std::cerr << "[ERR] cannot write " << outpath << "\n"; return; }
        out << folder << "\t" << N << "\n";
        out.close();

        std::cout << "[OK] " << rootpath << " -> " << outpath
                  << "  (folder=" << folder << ", N=" << N << ")\n";
    };

    {
        TString root0 = TString::Format("%s/Default.root", search_dir.Data());
        if (!gSystem->AccessPathName(root0, kFileExists)) {
            write_one(root0, table_path);
        }
    }

   
    void* dirp = gSystem->OpenDirectory(search_dir.Data());
    if (!dirp) { std::cerr << "[ERR] cannot open dir " << search_dir << "\n"; return; }

    const char* ent = nullptr;
    while ((ent = gSystem->GetDirEntry(dirp))) {
        if (!ent || ent[0] == '.') continue;
        TString rootpath = TString::Format("%s/%s/Default.root", search_dir.Data(), ent);
        if (gSystem->AccessPathName(rootpath, kFileExists)) continue;
        write_one(rootpath, table_path);
    }
    gSystem->FreeDirectory(dirp);
}
