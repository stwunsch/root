#ifndef ROOT_PyROOT_RDataFrameAsMatrix
#define ROOT_PyROOT_RDataFrameAsMatrix

#include "ROOT/RDF/RInterface.hxx"

namespace PyROOT {

template <typename OutputType, typename... ColTypes>
class AsMatrixHelper : public ROOT::Detail::RDF::RActionImpl<AsMatrixHelper<OutputType, ColTypes...>> {
public:
   using Result_t = std::vector<OutputType>;

private:
   std::vector<std::shared_ptr<Result_t>> fData;
   std::shared_ptr<Result_t> fResult = std::make_shared<Result_t>();

public:
   AsMatrixHelper()
   {
      const auto nSlots = ROOT::IsImplicitMTEnabled() ? ROOT::GetImplicitMTPoolSize() : 1;
      for (auto i : ROOT::TSeqU(nSlots)) {
         fData.emplace_back(std::make_shared<Result_t>());
      }
   }
   AsMatrixHelper(AsMatrixHelper &&) = default;
   AsMatrixHelper(const AsMatrixHelper &) = delete;
   std::shared_ptr<Result_t> GetResultPtr() const { return fResult; }
   void Initialize() {}
   void InitTask(TTreeReader *, unsigned int) {}
   void Exec(unsigned int slot, ColTypes... values)
   {
      int expander[] = {(fData[slot]->emplace_back(OutputType(values)), 0)..., 0};
      (void)expander;
   }

   void Finalize()
   {
      // Get number of entries
      unsigned int numEntries = 0;
      for (auto i = 0; i < fData.size(); i++) {
         numEntries += fData[i]->size();
      }

      // Fill vector
      fResult->reserve(numEntries);
      for (auto i = 0; i < fData.size(); i++) {
         fResult->insert(fResult->end(), fData[i]->begin(), fData[i]->end());
         /*
         for (auto j = 0; j < fData[i]->size(); j++) {
            fResult->emplace_back(fData[i]->at(j));
         }
         */
      }
   }
   std::string GetActionName() { return "AsMatrixHelper"; }
};

template <typename OutputType, typename... ColTypes>
class CallHelper {
public:
   std::vector<OutputType>
   Call(ROOT::RDF::RNode df, AsMatrixHelper<OutputType, ColTypes...> &helper, std::vector<std::string> &columns)
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
