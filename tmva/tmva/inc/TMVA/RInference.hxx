#ifndef TMVA_RINFERENCE
#define TMVA_RINFERENCE

#include "TMVA/RTensor.hxx"
#include "TString.h"
#include <memory>
#include "ROOT/RVec.hxx"

namespace TMVA {
namespace Experimental {

class RInference {
private:
   std::unique_ptr<Reader> fReader;
   std::vector<float> fValues;

public:
   RInference(const std::string &path);
   RTensor<float> Predict(RTensor<float> &x);
};

RInference::RInference(const std::string &path)
{
   fReader = std::make_unique<Reader>("Silent");
   const auto numVars = 4;
   fValues = std::vector<float>(numVars); // TODO: Determine this at construction time
   for (unsigned int i = 0; i < numVars; i++) {
      fReader->AddVariable(TString::Format("var%u", i), &fValues[i]);
   }
   fReader->BookMVA("TMVAInference", path.c_str());
}

RTensor<float> RInference::Predict(RTensor<float> &x)
{
   const auto shape = x.GetShape();
   const auto numEntries = shape[0];
   const auto numVars = shape[1];
   RTensor<float> y({numEntries});
   for (std::size_t i = 0; i < numEntries; i++) {
      for (std::size_t j = 0; j < numVars; j++) {
         fValues[j] = x(i, j);
      }
      y(i) = fReader->EvaluateMVA("TMVAInference");
   }
   return y;
}

} // namespace Experimental
} // namespace TMVA

#endif // TMVA_RINFERENCE
