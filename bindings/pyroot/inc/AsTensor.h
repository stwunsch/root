#ifndef ROOT_PyROOT_AsTensor
#define ROOT_PyROOT_AsTensor

namespace PyROOT {
template <typename T>
T *GetAddressFromLong(long ptr)
{
   return reinterpret_cast<T *>(ptr);
}
}

#endif
