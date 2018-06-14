#ifndef ROOT_TMVA_SomeModel
#define ROOT_TMVA_SomeModel

#include <vector>
#include <type_traits>
#include "TMVA/Reader.h"

namespace TMVA {

namespace Preprocessing {

/* Preprocessing method base class */
template <typename T>
class IMethod {
public:
   virtual std::vector<T> Transform(const std::vector<T> &inputs) = 0;
   virtual std::vector<T> InverseTransform(const std::vector<T> &inputs) = 0;
   std::vector<T> operator()(std::vector<T> &inputs) { return Transform(inputs); };
};

/* Implementation of some preprocessing method */
template <typename T>
class SomeMethod : public IMethod<T> {
public:
   SomeMethod(std::string file) : fFile(file){};
   std::vector<T> Transform(const std::vector<T> &inputs) { return inputs; };
   std::vector<T> InverseTransform(const std::vector<T> &inputs) { return inputs; };

private:
   std::string fFile;
};

} // namespace Preprocessing

namespace Application {

/* MVA method base class */
template <typename T>
class IMethod {
public:
   virtual std::vector<T> Predict(const std::vector<T> &inputs) = 0;
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

private:
   const char *fTag = "SomeMethod";
   std::string fFile;
   TMVA::Reader *fReader;
};

} // namespace Application

namespace Utility {

/* Make vector */
template <typename T, typename... Inputs>
struct MakeVectorImpl {
   std::vector<T> operator()(Inputs... inputs) { return std::vector<T>({inputs...}); };
};

template <int N, typename T, typename... Ts>
struct MakeVectorHelper {
   using type = typename MakeVectorHelper<N - 1, T, T, Ts...>::type;
};

template <typename T, typename... Ts>
struct MakeVectorHelper<0, T, Ts...> {
   using type = MakeVectorImpl<T, Ts...>;
};

template <int N, typename T>
struct MakeVector {
   using type = typename MakeVectorHelper<N, T>::type;
};

/* Combine methods */
template <typename T>
class Chain {
public:
   Chain(Preprocessing::IMethod<T> &transform, Application::IMethod<T> &model)
      : fTransform(&transform), fModel(&model){};
   std::vector<T> operator()(std::vector<T> &inputs) { return Process(inputs); };
   std::vector<T> Process(std::vector<T> &inputs) { return fModel->Predict(fTransform->Transform(inputs)); };

private:
   Preprocessing::IMethod<T> *fTransform = 0;
   // Preprocessing::IMethod<T> *fInverseTransform = 0; // TODO: To be included
   Application::IMethod<T> *fModel = 0;
};

} // namespace Utility
} // namespace TMVA
#endif
