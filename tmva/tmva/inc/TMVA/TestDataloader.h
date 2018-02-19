#pragma once

#include "TTree.h"

namespace TMVA{
namespace Test{

    class Dataloader {
        public:
        Dataloader();
        Dataloader(TTree* tree);
        void Fill(float* x);

        private:
        TTree* fTree;
    };

}
}
