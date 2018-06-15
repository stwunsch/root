#ifndef ROOT_TMVA_Application
#define ROOT_TMVA_Application

#include <vector>
#include "TMVA/Reader.h"

namespace TMVA {
namespace Application {

/* MVA method base class */
template <typename T>
class IMethod {
public:
   virtual std::vector<T> Predict(const std::vector<T> &inputs) = 0;
   virtual std::vector<std::vector<T>> Predict(const std::vector<std::vector<T>> &inputs) = 0;
   std::vector<T> operator()(std::vector<T> &inputs) { return Predict(inputs); };
};

/* Implementation of some MVA method */
template <typename T>
class SomeMethod : public IMethod<T> {
public:
   SomeMethod(std::string file) : fFile(file)
   {
      std::vector<std::string> variables = {"var1", "var2", "var3", "var4"}; // TODO: extract from xml
      fReader = new TMVA::Reader("!Color:Silent");
      T dummy;
      for (auto &var : variables)
         fReader->AddVariable(var, &dummy);
      fReader->BookMVA(fTag, fFile);
   };

   std::vector<T> Predict(const std::vector<T> &inputs)
   {
      return std::vector<float>({static_cast<T>(fReader->EvaluateMVA(inputs, fTag))});
   }
   std::vector<std::vector<T>> Predict(const std::vector<std::vector<T>> &inputs)
   {
      std::vector<std::vector<T>> results(inputs.size());
      for (unsigned int i = 0; i < inputs.size(); i++)
         results[i] = Predict(inputs[i]);
      return results;
   }

private:
   const char *fTag = "SomeMethod";
   std::string fFile;
   TMVA::Reader *fReader;
};

} // namespace Application
} // namespace TMVA

#endif
