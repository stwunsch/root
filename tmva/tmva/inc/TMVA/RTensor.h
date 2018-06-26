#ifndef ROOT_TMVA_RTensor
#define ROOT_TMVA_RTensor

#include <vector>
#include <iostream>
#include <stdexcept>
#include <sstream>

namespace TMVA {
namespace Experimental {

template <typename T>
class RTensor {
public:
   RTensor(const std::vector<size_t> &shape);
   RTensor(T *data, const std::vector<size_t> &shape);

   std::vector<size_t> Shape() const;
   bool OwnsData() const;
   T *Data() const;
   void Reshape(const std::vector<size_t> &shape);
   void ExpandDims(int axis);
   void Squeeze();

   template <typename... Args>
   T At(Args... args) const;

   template <typename... Args>
   T operator()(Args... args) const;

private:
   size_t Size();
   T *fExternalData = 0;
   std::vector<T> fData;
   std::vector<size_t> fShape;
};

// Constructor with memory adoption
template <typename T>
RTensor<T>::RTensor(T *data, const std::vector<size_t> &shape) : fExternalData(data), fShape(shape)
{
}

// Constructor with owned memory
template <typename T>
RTensor<T>::RTensor(const std::vector<size_t> &shape) : fShape(shape)
{
   const size_t length = Size();
   fData.resize(length);
}

// Getter data pointer
template <typename T>
T *RTensor<T>::Data() const
{
   if (OwnsData()) {
      return fData.data();
   } else {
      return fExternalData;
   }
}

// Getter shape
template <typename T>
std::vector<size_t> RTensor<T>::Shape() const
{
   return fShape;
}

// Status of data ownership
template <typename T>
bool RTensor<T>::OwnsData() const
{
   if (fExternalData == 0) {
      return true;
   } else {
      return false;
   }
}

// Getter flattened size
template <typename T>
size_t RTensor<T>::Size()
{
   size_t length = 1;
   for (const auto &d : fShape)
      length *= d;
   return length;
}

// Reshape
template <typename T>
void RTensor<T>::Reshape(const std::vector<size_t> &shape)
{
   fShape = shape;
}

// Expand dimensions
template <typename T>
void RTensor<T>::ExpandDims(int axis)
{
   if (axis == -1) {
      fShape.push_back(1);
   } else {
      fShape.insert(fShape.begin() + axis, 1);
   }
}

// Squeeze dimensions
template <typename T>
void RTensor<T>::Squeeze()
{
   std::vector<size_t> newShape;
   for (const auto &d : fShape) {
      if (d != 1)
         newShape.push_back(d);
   }
   fShape = newShape;
}

// Access elements
template <typename T>
template <typename... I>
T RTensor<T>::At(I... idx) const
{
   std::vector<size_t> indices({static_cast<size_t>(idx)...});
   size_t globalIndex = 0;
   for (size_t i = 0; i < fShape.size() - 1; i++)
      globalIndex += fShape[i + 1] * indices[i];
   globalIndex += indices[sizeof...(idx)-1];
   return globalIndex;
}

// Access elements with call operator
template <typename T>
template <typename... I>
T RTensor<T>::operator()(I... idx) const
{
   return this->At(idx...);
}

// Pretty printing
template <typename T>
std::ostream &operator<<(std::ostream &os, const RTensor<T> &t)
{
   auto shape = t.Shape();
   if (shape.size() == 1) {
      os << "{ ";
      for (unsigned int i = 0; i < shape[0]; i++) {
         os << t.At(i);
         if (i != shape[0] - 1)
            os << ", ";
      }
      os << " }";
   } else if (shape.size() == 2) {
      os << "{ ";
      for (unsigned int i = 0; i < shape[0]; i++) {
         if (i == 0)
            os << "{ ";
         else
            os << "  { ";
         for (unsigned int j = 0; j < shape[1]; j++) {
            os << t.At(i, j);
            if (j != shape[1] - 1)
               os << ", ";
            else
               os << " }";
         }
         if (i != shape[0] - 1)
            os << "\n";
         else
            os << " }";
      }
   } else {
      os << "Not yet implemented.";
   }
   return os;
}

} // namespace Experimental
} // namespace TMVA

namespace cling {

// cling pretty printing
template <typename T>
inline std::string printValue(TMVA::Experimental::RTensor<T> *t)
{
   std::stringstream ss;
   ss << *t;
   return ss.str();
}

} // namespace cling

#endif
