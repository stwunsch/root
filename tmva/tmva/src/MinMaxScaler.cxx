#include "TMVA/MinMaxScaler.h"
#include <iostream>
#include <limits>

namespace TMVA{
namespace Preprocessing{

MinMaxScaler::MinMaxScaler(){
    fScaleMin = -1.0;
    fScaleMax = 1.0;
}

void MinMaxScaler::ProcessFinalize(){
}

void MinMaxScaler::ProcessInputs(Float_t* inputs, UInt_t numSamples){
    const UInt_t numFeatures = GetNumFeatures();
    if(fFeatureMin.size()!=numFeatures){
        fFeatureMin = std::vector<Float_t>(numFeatures, std::numeric_limits<Float_t>::max());
    }
    if(fFeatureMax.size()!=numFeatures){
        fFeatureMax = std::vector<Float_t>(numFeatures, -std::numeric_limits<Float_t>::max());
    }

    for(UInt_t i=0; i<numSamples; i++){
        for(UInt_t j=0; j<numFeatures; j++){
            const UInt_t index = i*numFeatures+j;
            if(inputs[index]<fFeatureMin[j]) fFeatureMin[j] = inputs[index];
            if(inputs[index]>fFeatureMax[j]) fFeatureMax[j] = inputs[index];
        }
    }
}

void MinMaxScaler::Print(){
    if(!fIsFinalized) std::cerr << "Finalize the transformation first." << std::endl;
    for(UInt_t i=0; i<GetNumFeatures(); i++){
        std::cout << "Feature: " << i+1 << ", Min/Max: " << fFeatureMin[i] << "/" << fFeatureMax[i] << std::endl;
    }
}

void MinMaxScaler::Transform(Float_t* inputs, UInt_t numSamples){
    const UInt_t numFeatures = GetNumFeatures();
    for(UInt_t i=0; i<numSamples; i++){
        for(UInt_t j=0; j<numFeatures; j++){
            inputs[i*numFeatures+j] = (inputs[i*numFeatures+j]-fFeatureMin[j])/(fFeatureMax[j]-fFeatureMin[j])*(fScaleMax-fScaleMin)+fScaleMin;
        }
    }
}

void MinMaxScaler::InverseTransform(Float_t* inputs, UInt_t numSamples){
    const UInt_t numFeatures = GetNumFeatures();
    for(UInt_t i=0; i<numSamples; i++){
        for(UInt_t j=0; j<numFeatures; j++){
            inputs[i*numFeatures+j] = (inputs[i*numFeatures+j]-fScaleMin)*(fFeatureMax[j]-fFeatureMin[j])/(fScaleMax-fScaleMin)+fFeatureMin[j];
        }
    }
}

}
}
