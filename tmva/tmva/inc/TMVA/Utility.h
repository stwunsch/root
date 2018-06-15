#ifndef ROOT_TMVA_Utility
#define ROOT_TMVA_Utility

#include <vector>
#include "TMVA/Preprocessing.h"
#include "TMVA/Application.h"

namespace TMVA {
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
