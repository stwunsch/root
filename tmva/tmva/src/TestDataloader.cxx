#include "TMVA/TestDataloader.h"

namespace TMVA{
namespace Test{

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

}
}
