#pragma once

#include "TTree.h"

class TTensor : public TNamed {
    public:
        TTensor(){};
        TTensor(std::vector<UInt_t> dims, float* data){
            fDims = dims;
            fData = data;
        }

    std::vector<UInt_t> GetDims(){return fDims;};
    UInt_t GetNumDims(){return fDims.size();};
    UInt_t GetDim(UInt_t idx){return fDims[idx];};
    float* GetData(){return fData;};
    long GetPointer(){return reinterpret_cast<long>(fData);};

    private:
        std::vector<UInt_t> fDims;
        float* fData = 0;

    ClassDef(TTensor, 1);
};

namespace TMVA{
namespace Test{

    class Dataloader {
        public:
        Dataloader();
        Dataloader(TTree* tree);
        void Fill(float*);
        void Fill(float*, const std::string&, const std::string&);

        private:
        TTree* fTree;
    };

}
}

class TestFunction {
    public:
    void Process(float* x, int len){
        for(auto i=0; i<len; i++) x[i] +=1;
    }

    TTensor GenerateTensor(){
        std::vector<UInt_t> d = {4, 2};
        float* data = new float[8];
        for(auto i=0; i<8; i++) data[i] = float(i);
        TTensor tensor(d, data);
        return tensor;
    }
};
