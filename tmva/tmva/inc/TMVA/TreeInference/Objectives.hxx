#ifndef TMVA_TREEINFERENCE_OBJECTIVES
#define TMVA_TREEINFERENCE_OBJECTIVES

#include <cmath>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace TMVA {
namespace Experimental {
namespace Objectives {

/// Logistic function f(x) = 1 / (1 + exp(-x))
template <typename T>
inline T Logistic(T value)
{
   return 1.0 / (1.0 + std::exp(-1.0 * value));
}

/// Identity function f(x) = x
template <typename T>
inline T Identity(T value)
{
   return value;
}

template <typename T>
std::function<T(T)> GetFunction(const std::string &name)
{
   if (name.compare("identity") == 0)
      return std::function<T(T)>(Identity<T>);
   else if (name.compare("logistic") == 0)
      return std::function<T(T)>(Logistic<T>);
   else
      throw std::runtime_error("Objective function with name \"" + name + "\" is not implemented.");
}

} // namespace Objectives
} // namespace Experimental
} // namespace TMVA

#endif // TMVA_TREEINFERENCE_OBJECTIVES
