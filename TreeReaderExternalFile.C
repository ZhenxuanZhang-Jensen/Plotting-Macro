#include "controlplotvars_CHS.h"
#include "utils.C" // Tokenize


//======================================================================

void loadCutString(const char *filename, TString& cutstring, std::vector<TTreeReaderValue<double>> &cutBranches, TTreeReader &myReader)
// void loadCutString(const char *filename, TString& cutstring, std::vector<TTreeReaderValue<double>> cutBranches, TTreeReader myReader)
{
  FILE *fp = fopen(filename,"r");
  if (!fp) {
    cout << "Error, file " << TString(filename) << " not found." << endl;
    exit(-1);
  }

  char line[512];

  for (int i=0; !feof(fp) && fgets(line,512,fp); i++) {
    if (!strlen(line) || line[0]=='#') continue; // comments are welcome

    if (cutstring.Length()) cutstring += " && ";

    string strline(line);
    std::cout << "LINE: " << line << std::endl;
    strline.pop_back();     // shed the \n
    vector<string> fields;

    // expect columns with fields cutname, cutvalue, possible embedded spaces both
    // within and between, so " " or "\t" cannot be used as delimiters. Require quotes
    // instead.
    //
    Tokenize(strline,fields, "\"");
// 
    for (size_t j=0; j<fields.size(); j++)
      cout << j << ": \"" << fields[j] << "\"" << endl;

    TTreeReaderValue<double> tempTreeBranch = {myReader,(TString)fields.at(2)};
    cutBranches.push_back(tempTreeBranch);

    assert (fields.size()==7);

    // cutstring += TString(fields.at(2));
    cutstring += TString(*cutBranches[i]+fields.at(4)+fields.at(6));
  }
  std::cout << "cutstring: " << cutstring << std::endl;
}                                                       // loadCutString

//======================================================================

void TreeReaderExternalFile(){

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

  TString CutString;
  // loadCutString("cutString.txt", CutString, cutBranches);
  loadCutString("cutString.txt", CutString, cutBranches, myReader);

  std::cout << "Test: " << CutString << std::endl;

// Loop over all entries of the TTree or TChain.
  while (myReader.Next()) {
    {
      for (int i = 0; i < vectorOfTH1F.size(); ++i)
      {
        if (*vtree[i]<100)
        vectorOfTH1F[i]->Fill(*vtree[i]);
      }
    }
  }

   for (int i = 0; i < vectorOfTH1F.size(); ++i)
   {
      vectorOfTH1F[i]->Draw();

      c1->SaveAs(outFileName[i]+".png");
      // c1->SaveAs(outFileName[i]+".pdf");
      c1->Clear();
   }

}
