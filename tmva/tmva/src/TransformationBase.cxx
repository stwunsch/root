#include "TMVA/TransformationBase.h"
#include <iostream>

/*
 * TODO:
 * - Replace cerr with proper errors
 */

namespace TMVA{
namespace Preprocessing{

void TransformationBase::Finalize(){
    if(fIsFinalized==true){
        std::cerr << "[ERROR] Transformation has already been finalized." << std::endl;
    }
    ProcessFinalize();
    fIsFinalized = true;
}

void TransformationBase::PresentInputs(Float_t* inputs, UInt_t numSamples, UInt_t numFeatures){
    if(numFeatures<1) std::cerr << "[ERROR] The inputs has to contain at least one feature." << std::endl;
    if(numSamples<1) std::cerr << "[ERROR] The number of samples has to be at least one." << std::endl;
    if((fNumFeatures!=0)&&(numFeatures!=fNumFeatures)) std::cerr << "[ERROR] The function has been called before with a different number of features." << std::endl;
    fNumFeatures=numFeatures;
    ProcessInputs(inputs, numSamples);
}

}
}
