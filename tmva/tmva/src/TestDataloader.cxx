#include "TMVA/TestDataloader.h"
#include "ROOT/TDataFrame.hxx"

using namespace ROOT::Experimental;

namespace TMVA{
namespace Test{

    template <typename TDF>
    void FillMatrix(float *buf, const std::vector<std::string> &columnNames, TDF &tdf){
        auto fillMatrix = [buf](float a, float b, float c, float d, ULong64_t entry) {
            buf[entry*4] = a;
            buf[entry*4 + 1] = b;
            buf[entry*4 + 2] = c;
            buf[entry*4 + 3] = d;
        };

       auto trueColumnNames = columnNames;
       trueColumnNames.emplace_back("tdfentry_");
       tdf.Foreach(fillMatrix, trueColumnNames);
    }

    Dataloader::Dataloader(){
    }

    Dataloader::Dataloader(TTree* tree){
        fTree = tree;
    }

    void Dataloader::Fill(float* x){
        float vars[4];
        fTree->SetBranchAddress("var1", vars+0);
        fTree->SetBranchAddress("var2", vars+1);
        fTree->SetBranchAddress("var3", vars+2);
        fTree->SetBranchAddress("var4", vars+3);
        for(UInt_t iEvent=0; iEvent<fTree->GetEntries(); iEvent++){
            fTree->GetEntry(iEvent);
            memcpy(x+4*iEvent, vars, 4*4);
        }
    }

    void Dataloader::Fill(float* x, const std::string& file, const std::string& tree){
        TDataFrame tdf(tree, file);
        FillMatrix(x, {"var1", "var2", "var3", "var4"}, tdf);
    }

}
}
