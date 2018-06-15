#ifndef ROOT_TMVA_Transformation
#define ROOT_TMVA_Transformation

#include <vector>

namespace TMVA {
namespace Transformation {

/* Preprocessing method base class */
template <typename T>
class IMethod {
public:
   virtual std::vector<std::vector<T>> Transform(const std::vector<std::vector<T>> &inputs) = 0;
   virtual std::vector<T> Transform(const std::vector<T> &inputs) = 0;

   virtual std::vector<T> InverseTransform(const std::vector<T> &inputs) = 0;
   virtual std::vector<std::vector<T>> InverseTransform(const std::vector<std::vector<T>> &inputs) = 0;

   std::vector<T> operator()(std::vector<T> &inputs) { return Transform(inputs); };
};

/* Implementation of some preprocessing method */
template <typename T>
class SomeMethod : public IMethod<T> {
public:
   SomeMethod(std::string file) : fFile(file){};

   std::vector<T> Transform(const std::vector<T> &inputs) { return inputs; };
   std::vector<std::vector<T>> Transform(const std::vector<std::vector<T>> &inputs)
   {
      std::vector<std::vector<T>> results(inputs.size());
      for (unsigned int i = 0; i < inputs.size(); i++)
         results[i] = Transform(inputs[i]);
      return results;
   }

   std::vector<T> InverseTransform(const std::vector<T> &inputs) { return inputs; };
   std::vector<std::vector<T>> InverseTransform(const std::vector<std::vector<T>> &inputs) { return inputs; };

private:
   std::string fFile;
};

} // namespace Transformation
} // namespace TMVA

#endif
