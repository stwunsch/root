#ifndef ROOT_TMVA_SomeModel
#define ROOT_TMVA_SomeModel

#include <vector>
#include <type_traits>

namespace TMVA {

/* Model base class */
template <typename T>
class Model {
public:
   virtual std::vector<T> Predict(const std::vector<T> &inputs) = 0;
};

/* Implementation of some model */
template <typename T>
class SomeModel : public Model<T> {
public:
   std::string fFile;
   SomeModel(std::string file) : fFile(file){};
   std::vector<T> Predict(const std::vector<T> &inputs) { return std::vector<T>({inputs[0] * inputs[1]}); }
};

/* Interface to RDataFrame */
template <typename T, typename... Inputs>
class MakePredict {
public:
   Model<T> &fModel;

   MakePredict(Model<T> &model) : fModel(model) {}

   std::vector<T> operator()(Inputs... inputs) { return fModel.Predict({inputs...}); }
};

template <int N, typename T, typename... Ts>
struct MakePredictHelper {
   using type = typename MakePredictHelper<N - 1, T, T, Ts...>::type;
};

template <typename T, typename... Ts>
struct MakePredictHelper<0, T, Ts...> {
   using type = MakePredict<T, Ts...>;
};

template <int NumVars, typename T>
struct Predict {
   using type = typename MakePredictHelper<NumVars, T>::type;
};
}
#endif
