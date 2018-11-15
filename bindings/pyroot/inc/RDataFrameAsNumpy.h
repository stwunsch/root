#ifndef ROOT_PyROOT_RDataFrameAsNumpy
#define ROOT_PyROOT_RDataFrameAsNumpy

#include "ROOT/RDF/RInterface.hxx"
#include <tuple>
#include <iostream>
#include "Python.h"

namespace PyROOT {

template <typename... ColTypes>
class AsNumpyHelper : public ROOT::Detail::RDF::RActionImpl<AsNumpyHelper<ColTypes...>> {
public:
   using Result_t = PyObject *;
   using Data_t = std::vector<std::tuple<ColTypes...>>;

private:
   std::vector<std::shared_ptr<Data_t>> fData;
   std::shared_ptr<Result_t> fResult = std::make_shared<Result_t>();

public:
   AsNumpyHelper()
   {
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
      unsigned int numEvents = 0;
      for (auto i = 0; i < fData.size(); i++) {
         numEvents += fData[i]->size();
      }
      *fResult = PyTuple_New(numEvents);
   }
   std::string GetActionName() { return "AsNumpyHelper"; }
};

template <typename... ColTypes>
class BookingHelper {
public:
   ROOT::RDF::RResultPtr<typename AsNumpyHelper<ColTypes...>::Result_t>
   Book(ROOT::RDF::RNode df, AsNumpyHelper<ColTypes...> &helper, std::vector<std::string> &columns)
   {
      return df.Book<ColTypes...>(std::move(helper), columns);
   }
};

template <typename T>
ROOT::RDF::RNode AsRNode(T df)
{
   return df;
}
}

#endif
