#ifndef ROOT_PyROOT_RDataFrameAsDict
#define ROOT_PyROOT_RDataFrameAsDict

#include "ROOT/RDF/RInterface.hxx"

namespace PyROOT {

class TakeHelper {
public:
   ROOT::RDF::RResultPtr<std::vector<unsigned int>> Take(ROOT::RDF::RNode df, std::string column);
};

/*
template <typename T>
class TakeHelper {
public:
   ROOT::RDF::RResultPtr<std::vector<T>> Take(ROOT::RDF::RNode df, std::string column)
   {
      return df.Take<T>(column);
   }
};
*/
}

#endif
