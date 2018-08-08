#ifndef ROOT_TMVA_MINMAXSCALER
#define ROOT_TMVA_MINMAXSCALER

#include "TNamed.h"
#include "TMVA/RTensor.h"
#include "TFile.h"
#include <string>
#include <vector>

namespace TMVA {
namespace Experimental {

namespace Training {

template <typename T>
class MinMaxScaler : public TNamed {
public:
   MinMaxScaler();
   MinMaxScaler(T min, T max);
   void Fit(TMVA::Experimental::RTensor<T> &inputs);

   std::vector<T> GetInputMin() { return fInputMin; };
   std::vector<T> GetInputMax() { return fInputMax; };
   T GetOutputMin() { return fOutputMin; };
   T GetOutputMax() { return fOutputMax; };

private:
   T fOutputMin;
   T fOutputMax;
   std::vector<T> fInputMin;
   std::vector<T> fInputMax;

public:
   ClassDef(MinMaxScaler, 1);
};

template <typename T>
MinMaxScaler<T>::MinMaxScaler() : fOutputMin(0.0), fOutputMax(1.0){};

template <typename T>
MinMaxScaler<T>::MinMaxScaler(T min, T max) : fOutputMin(min), fOutputMax(max){};

template <typename T>
void MinMaxScaler<T>::Fit(TMVA::Experimental::RTensor<T> &inputs)
{
   const auto shape = inputs.GetShape();
   const auto numEvents = shape[0];
   const auto numVariables = shape[1];
   fInputMin.resize(numVariables);
   fInputMax.resize(numVariables);
   for (size_t iVar = 0; iVar < numVariables; iVar++) {
      T min = 9999;
      T max = -9999;
      for (size_t iEvt = 0; iEvt < numEvents; iEvt++) {
         auto v = inputs(iEvt, iVar);
         if (v < min)
            min = v;
         if (v > max)
            max = v;
      }
      fInputMin[iVar] = min;
      fInputMax[iVar] = max;
   }
};

} // namespace Training

namespace Inference {

template <typename T>
class MinMaxScaler {
public:
   MinMaxScaler(TMVA::Experimental::Training::MinMaxScaler<T> &model);
   std::vector<T> Transform(std::vector<T> &inputs);
   TMVA::Experimental::RTensor<T> Transform(TMVA::Experimental::RTensor<T> &inputs);

private:
   T fOutputMin;
   T fOutputMax;
   std::vector<T> fInputMin;
   std::vector<T> fInputMax;
};

template <typename T>
MinMaxScaler<T>::MinMaxScaler(TMVA::Experimental::Training::MinMaxScaler<T> &model)
{
   fInputMin = model.GetInputMin();
   fInputMax = model.GetInputMax();
   fOutputMin = model.GetOutputMin();
   fOutputMax = model.GetOutputMax();
};

template <typename T>
std::vector<T> MinMaxScaler<T>::Transform(std::vector<T> &inputs)
{
   const auto size = inputs.size();
   std::vector<T> outputs(size);
   for (size_t i = 0; i < size; i++)
      outputs[i] = (inputs[i] - fInputMin[i]) / (fInputMax[i] - fInputMin[i]) * (fOutputMax - fOutputMin) + fOutputMin;
   return outputs;
}

template <typename T>
TMVA::Experimental::RTensor<T> MinMaxScaler<T>::Transform(TMVA::Experimental::RTensor<T> &inputs)
{
   const auto shape = inputs.GetShape();
   TMVA::Experimental::RTensor<T> outputs(shape);
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         outputs(i, j) =
            (inputs(i, j) - fInputMin[j]) / (fInputMax[j] - fInputMin[j]) * (fOutputMax - fOutputMin) + fOutputMin;
      }
   }
   return outputs;
}

namespace Sklearn {

template <typename T>
class MinMaxScaler : public TNamed {
public:
   MinMaxScaler(){};
   MinMaxScaler(const std::string &filename, const std::string &name);

   void SetMin(std::vector<T> &min) { fMin = min; };
   void SetScale(std::vector<T> &scale) { fScale = scale; };
   std::vector<T> GetMin() { return fMin; };
   std::vector<T> GetScale() { return fScale; };

   TMVA::Experimental::RTensor<T> Transform(TMVA::Experimental::RTensor<T> &x);

private:
   std::vector<T> fMin;
   std::vector<T> fScale;

public:
   ClassDef(MinMaxScaler, 1);
};

template <typename T>
MinMaxScaler<T>::MinMaxScaler(const std::string &filename, const std::string &name)
{
   auto file = new TFile(filename.c_str());
   auto model = (MinMaxScaler *)file->Get(name.c_str());
   fMin = model->GetMin();
   fScale = model->GetScale();
   file->Close();
   delete file;
}

template <typename T>
TMVA::Experimental::RTensor<T> MinMaxScaler<T>::Transform(TMVA::Experimental::RTensor<T> &x)
{
   const auto shape = x.GetShape();
   TMVA::Experimental::RTensor<T> y(shape);
   for (size_t i = 0; i < shape[0]; i++) {
      for (size_t j = 0; j < shape[1]; j++) {
         y(i, j) = x(i, j) * fScale[j] + fMin[j];
      }
   }
   return y;
}

} // namespace Sklearn

} // namespace Inference

} // namespace Experimental
} // namespace TMVA

#endif
