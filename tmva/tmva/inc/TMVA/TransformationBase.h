#pragma once

#include "Rtypes.h"
#include "TNamed.h"

namespace TMVA{
namespace Preprocessing{
    /*
     * Template this class so that it works with all datatypes
     */
    class TransformationBase : public TNamed {
        public:
        /*
         * How to enable running this on very large datasets?
         * Set mutex for this implementations so that this can be done thread safe?
         */
        virtual void Fit(Float_t* inputs, UInt_t numSamples, UInt_t numFeatures) = 0;
        virtual void Transform(Float_t* inputs, UInt_t numSamples) = 0;
        virtual void InverseTransform(Float_t* inputs, UInt_t numSamples) = 0;

        UInt_t GetNumFeatures(){return fNumFeatures;}

        protected:
        UInt_t fNumFeatures = 0;

        public:
        ClassDef(TransformationBase, 1);
    };
}
}
