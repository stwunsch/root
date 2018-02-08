#pragma once

#include "TMVA/TransformationBase.h"

namespace TMVA{
namespace Preprocessing{
    class MinMaxScaler : public TransformationBase {
        public:
        MinMaxScaler();
        void Transform(Float_t* inputs, UInt_t numSamples);
        void InverseTransform(Float_t* inputs, UInt_t numSamples);
        void Print();

        private:
        void ProcessFinalize();
        void ProcessInputs(Float_t* inputs, UInt_t numSamples);
        std::vector<Float_t> fFeatureMin;
        std::vector<Float_t> fFeatureMax;
        Float_t fScaleMin;
        Float_t fScaleMax;
    };
}
}
