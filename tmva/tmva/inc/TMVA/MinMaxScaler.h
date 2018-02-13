#pragma once

#include "TMVA/TransformationBase.h"

namespace TMVA{
namespace Preprocessing{
    class MinMaxScaler : public TransformationBase {
        public:
        MinMaxScaler();
        void Fit(Float_t* inputs, UInt_t numSamples, UInt_t numFeatures);
        void Transform(Float_t* inputs, UInt_t numSamples);
        void InverseTransform(Float_t* inputs, UInt_t numSamples);
        void Print();

        private:
        std::vector<Float_t> fFeatureMin;
        std::vector<Float_t> fFeatureMax;
        Float_t fScaleMin;
        Float_t fScaleMax;
    };
}
}
