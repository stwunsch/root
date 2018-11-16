#ifndef ROOT_PyROOT_RDataFrameAsNumpy
#define ROOT_PyROOT_RDataFrameAsNumpy

#include "ROOT/RDF/RInterface.hxx"
#include <tuple>
#include <iostream>

namespace PyROOT {

PyObject *PyROOT_PyTuple_New(unsigned int n);
int PyROOT_PyTuple_SetItem(PyObject *po, unsigned int pos, PyObject *item);
PyObject* PyROOT_PyTuple_GetElement(float x);
PyObject* PyROOT_PyTuple_GetElement(int x);
void PyROOT_import_array();
PyObject* PyROOT_PyArray_EMPTY(int rows, int cols);

template <typename... ColTypes>
class AsNumpyHelper : public ROOT::Detail::RDF::RActionImpl<AsNumpyHelper<ColTypes...>> {
public:
   using Data_t = std::vector<std::tuple<ColTypes...>>;
   using Result_t = PyObject *;

private:
   std::vector<std::shared_ptr<Data_t>> fData;
   std::shared_ptr<Result_t> fResult = std::make_shared<Result_t>();

public:
   AsNumpyHelper()
   {
      PyROOT_import_array();
      const auto nSlots = ROOT::IsImplicitMTEnabled() ? ROOT::GetImplicitMTPoolSize() : 1;
      for (auto i : ROOT::TSeqU(nSlots)) {
         fData.emplace_back(std::make_shared<Data_t>());
      }
   }
   AsNumpyHelper(AsNumpyHelper &&) = default;
   AsNumpyHelper(const AsNumpyHelper &) = delete;
   std::shared_ptr<Result_t> GetResultPtr() const { return fResult; }
   void Initialize() {}
   void InitTask(TTreeReader *, unsigned int) {}
   void Exec(unsigned int slot, ColTypes... values) { fData[slot]->emplace_back(std::make_tuple(values...)); }
   void Finalize()
   {
      // Get number of events and make Python tuple of that size
      unsigned int numEvents = 0;
      for (auto i = 0; i < fData.size(); i++) {
         numEvents += fData[i]->size();
      }
      *fResult = PyROOT_PyArray_EMPTY(numEvents, std::tuple_size<std::tuple<ColTypes...>>::value);

      // Fill Python tuple
      /*
      unsigned int counter = 0;
      const auto tuple_size = std::tuple_size<std::tuple<ColTypes...>>::value;
      for (auto i = 0; i < fData.size(); i++) {
         for (auto j = 0; j < fData[i]->size(); j++) {
            auto entry = PyROOT_PyTuple_New(tuple_size);
            const auto element0 = PyROOT_PyTuple_GetElement(std::get<0>(fData[i]->at(j)));
            const auto element1 = PyROOT_PyTuple_GetElement(std::get<1>(fData[i]->at(j)));
            PyROOT_PyTuple_SetItem(entry, 0, element0);
            PyROOT_PyTuple_SetItem(entry, 1, element1);
            PyROOT_PyTuple_SetItem(*fResult, counter, entry);
            counter++;
         }
      }
      */
   }
   std::string GetActionName() { return "AsNumpyHelper"; }
};

template <typename... ColTypes>
class CallHelper {
public:
   typename AsNumpyHelper<ColTypes...>::Result_t
   Call(ROOT::RDF::RNode df, AsNumpyHelper<ColTypes...> &helper, std::vector<std::string> &columns)
   {
      return df.Book<ColTypes...>(std::move(helper), columns).GetValue();
   }
};

template <typename T>
ROOT::RDF::RNode AsRNode(T df)
{
   return df;
}
}

#endif
