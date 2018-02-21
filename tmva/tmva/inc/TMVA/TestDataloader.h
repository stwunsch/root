#pragma once

#include "TTree.h"

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
