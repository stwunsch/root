#pragma once

#include "Rtypes.h"

namespace TMVA{
namespace Preprocessing{
    /*
     * Template this class so that it works with all datatypes
     */
    class TransformationBase {
        public:
        /*
         * Set mutex for this implementations so that this can be done thread safe
         */
        void PresentInputs(Float_t* inputs, UInt_t numSamples, UInt_t numFeatures);
        void Finalize();
        UInt_t GetNumFeatures(){return fNumFeatures;}
        Bool_t IsFinalized(){return fIsFinalized;}

        virtual void Transform(Float_t* inputs, UInt_t numSamples) = 0;
        virtual void InverseTransform(Float_t* inputs, UInt_t numSamples) = 0; // Not needed for this proof of concept
        virtual void Print() = 0;

        protected:
        Bool_t fIsFinalized = false;

        private:
        UInt_t fNumFeatures = 0;

        virtual void ProcessFinalize() = 0;
        virtual void ProcessInputs(Float_t* inputs, UInt_t numSamples) = 0;
    };
}
}
