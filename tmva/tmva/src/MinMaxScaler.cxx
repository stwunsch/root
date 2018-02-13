#include "TMVA/MinMaxScaler.h"
#include <limits>

namespace TMVA{
namespace Preprocessing{

MinMaxScaler::MinMaxScaler(){
    fScaleMin = -1.0;
    fScaleMax = 1.0;
}

MinMaxScaler::MinMaxScaler(Float_t scaleMin, Float_t scaleMax){
    fScaleMin = scaleMin;
    fScaleMax = scaleMax;
}

void MinMaxScaler::Fit(Float_t* inputs, UInt_t numSamples, UInt_t numFeatures){
    fNumFeatures = numFeatures;
    fFeatureMin = std::vector<Float_t>(fNumFeatures, std::numeric_limits<Float_t>::max());
    fFeatureMax = std::vector<Float_t>(fNumFeatures, -std::numeric_limits<Float_t>::max());

    for(UInt_t i=0; i<numSamples; i++){
        for(UInt_t j=0; j<fNumFeatures; j++){
            const UInt_t index = i*fNumFeatures+j;
            if(inputs[index]<fFeatureMin[j]) fFeatureMin[j] = inputs[index];
            if(inputs[index]>fFeatureMax[j]) fFeatureMax[j] = inputs[index];
        }
    }
}

void MinMaxScaler::Transform(Float_t* inputs, UInt_t numSamples){
    for(UInt_t i=0; i<numSamples; i++){
        for(UInt_t j=0; j<fNumFeatures; j++){
            inputs[i*fNumFeatures+j] = (inputs[i*fNumFeatures+j]-fFeatureMin[j])/(fFeatureMax[j]-fFeatureMin[j])*(fScaleMax-fScaleMin)+fScaleMin;
        }
    }
}

void MinMaxScaler::InverseTransform(Float_t* inputs, UInt_t numSamples){
    for(UInt_t i=0; i<numSamples; i++){
        for(UInt_t j=0; j<fNumFeatures; j++){
            inputs[i*fNumFeatures+j] = (inputs[i*fNumFeatures+j]-fScaleMin)*(fFeatureMax[j]-fFeatureMin[j])/(fScaleMax-fScaleMin)+fFeatureMin[j];
        }
    }
}

}
}
