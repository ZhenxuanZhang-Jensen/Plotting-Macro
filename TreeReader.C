#include "controlplotvars_CHS.h"
#include "utils.C" // Tokenize

void TreeReader(){

  // Create a canvas
  auto c1 = new TCanvas("c1");

  // Open the file containing the tree.
  auto myFile = TFile::Open("/uscms/home/rasharma/nobackup/double-higgs/GenSIMAnalyzer/CMSSW_10_2_22/src/GEN-SIM-analyzer/GenAnalyzer/Radion_hh_narrow_M270_LHEBqrk.root");
  if (myFile->IsZombie()) 
  {
    std::cout << "Seems like input file is corrupt. Please check intput file." << std::endl;
    exit(0);    
  }

  // Create a TTreeReader for the tree.
  TTreeReader myReader("otree", myFile);

  std::vector<const char*> variable;
  std::vector<TH1F*> vectorOfTH1F;
  std::vector<TTreeReaderValue<double>> vtree;
  std::vector<TTreeReaderValue<double>> cutBranches;
  std::vector<TString> outFileName;

  for (int ivar=0; ; ivar++)
  {
    plotVar_t pv = commonplotvars_chs[ivar];
    if ( !pv.plotvar.Length() ) break;

    // std::cout << "plotvar: " << pv.plotvar << ",\t" << pv.xlabel << ",\t" << pv.NBINS << ",\t" << pv.MINRange << ",\t" << pv.MAXRange << ", slog:" << pv.slog << std::endl;

    variable.push_back(pv.plotvar);

    // TH1F* tempHist = new TH1F(pv.plotvar, pv.plotvar, pv.NBINS, pv.MINRange, pv.MAXRange);
    TH1F* tempHist = new TH1F(pv.plotvar, ";"+pv.xlabel+";", pv.NBINS, pv.MINRange, pv.MAXRange);
    tempHist->SetStats(0);

    outFileName.push_back(pv.outfile);

    vectorOfTH1F.push_back(tempHist);

    TTreeReaderValue<double> tempTreeBranch = {myReader,pv.plotvar};
    vtree.push_back(tempTreeBranch);
  }


  // Loop over all entries of the TTree or TChain.
  while (myReader.Next()) {
    {
      for (int i = 0; i < vectorOfTH1F.size(); ++i)
      {
        // if (*vtree[i]<100)
        vectorOfTH1F[i]->Fill(*vtree[i]);
      }
    }
  }

   for (int i = 0; i < vectorOfTH1F.size(); ++i)
   {
      vectorOfTH1F[i]->Draw();

      c1->SaveAs(outFileName[i]+".png");
      c1->SaveAs(outFileName[i]+".pdf");
      c1->Clear();
   }

}