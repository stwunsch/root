#ifndef ROOT_TMVA_UTILITY
#define ROOT_TMVA_UTILITY

#include "TMVA/RTensor.h"
#include "ROOT/RDataFrame.hxx"
#include <iostream>

namespace TMVA {
namespace Experimental {
namespace Utility {

struct TargetType {
   enum Types : int { Labeled = 0, Onehot };
};

template <typename T>
class DataLoader {
public:
   DataLoader(std::vector<std::string> variables) : fInputs(variables){};

   void AddDataFrame(ROOT::RDataFrame &dataframe, const std::string &name);
   TMVA::Experimental::RTensor<T> GetInputs(const std::string &name);
   TMVA::Experimental::RTensor<T> GetTargets(const std::string &name, int type);

private:
   std::vector<ROOT::RDataFrame *> fDataFrames;
   std::vector<std::string> fInputs;
   std::vector<std::string> fTargets;
};

template <typename T>
void DataLoader<T>::AddDataFrame(ROOT::RDataFrame &dataframe, const std::string &name)
{
   fDataFrames.push_back(&dataframe);
   fTargets.push_back(name);
}

template <typename T>
TMVA::Experimental::RTensor<T> DataLoader<T>::GetInputs(const std::string &name)
{
   size_t idx = 0;
   for (size_t i = 0; i < fTargets.size(); i++) {
      if (fTargets[i] == name) {
         idx = i;
      }
   }

   std::vector<std::vector<T>> x(fInputs.size());
   for (size_t i = 0; i < fInputs.size(); i++) {
      std::string tmpVar = "_tmp_";
      tmpVar.append(fInputs[i]);
      auto tmp = fDataFrames[idx]->Define(tmpVar, [](T a) { return (T)a; }, {fInputs[i]});
      auto tmpVals = tmp.template Take<T>(tmpVar).GetValue();
      x[i].resize(tmpVals.size());
      for (size_t j = 0; j < tmpVals.size(); j++)
         x[i][j] = tmpVals[j];
   }

   TMVA::Experimental::RTensor<T> r({x[0].size(), x.size()});
   for (size_t i = 0; i < x[0].size(); i++) {
      for (size_t j = 0; j < x.size(); j++) {
         r(i, j) = x[j][i];
      }
   }

   return r;
}

template <typename T>
TMVA::Experimental::RTensor<T> DataLoader<T>::GetTargets(const std::string &name, int type)
{
   size_t idx = 0;
   for (size_t i = 0; i < fTargets.size(); i++) {
      if (fTargets[i] == name) {
         idx = i;
      }
   }
   auto count = fDataFrames[idx]->Count().GetValue();

   if (type == 0) {
      TMVA::Experimental::RTensor<T> r({count});
      for (size_t i = 0; i < count; i++) {
         r(i) = idx;
      }

      return r;
   } else {
      TMVA::Experimental::RTensor<T> r({count, fTargets.size()});
      for (size_t i = 0; i < count; i++) {
         r(i, idx) = 1;
      }
      return r;
   }
}

} // namespace Utility
} // namespace Experimental
} // namespace TMVA

#endif
