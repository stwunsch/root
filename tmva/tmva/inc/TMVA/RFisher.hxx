#ifndef TMVA_RFISHER
#define TMVA_RFISHER

#include "TMVA/RTensor.hxx"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Reader.h"
#include "TFile.h"

namespace TMVA {
namespace Experimental {

template <typename T = float>
class RFisher {
private:
   Factory *fFactory;
   DataLoader *fDataLoader;
   Reader *fReader;
   vector<T> fEvent;

public:
   RFisher();
   ~RFisher();
   void Train(RTensor<T> &x, RTensor<T> &y, string methodOptions = "H:!V:Fisher",
              string factoryOptions = "Color:DrawProgressBar", string factoryName = "TMVAClassificationFisher",
              string factoryOutputFile = "TMVAFisher.root", string dataloaderName = "TMVADataset",
              string dataloaderOptions = "NormMode=NumEvents");
   RTensor<T> Predict(RTensor<T> &x);
};

template <typename T>
RFisher<T>::RFisher()
{
}

template <typename T>
RFisher<T>::~RFisher()
{
}

template <typename T>
void RFisher<T>::Train(RTensor<T> &x, RTensor<T> &y, string methodOptions, string factoryOptions, string factoryName,
                       string factoryOutputFile, string dataloaderName, string dataloaderOptions)
{
   // Create factory
   auto outputFile = TFile::Open(factoryOutputFile.c_str(), "RECREATE");
   fFactory = new Factory(factoryName.c_str(), outputFile, factoryOptions.c_str());

   // Create datalaoder
   fDataLoader = new DataLoader(dataloaderName.c_str());

   // Add variables to dataloader
   std::vector<std::string> variables;
   for (auto i = 0; i < x.GetShape()[1]; i++) {
      variables.push_back(TString::Format("var%u", i + 1).Data());
      fDataLoader->AddVariable(variables[i].c_str());
   }

   // Read data to trees
   std::vector<TTree *> trees;
   std::vector<std::string> classes = {"Background", "Signal"};
   fEvent.resize(variables.size());
   for (auto &c : classes) {
      auto tree = new TTree(c.c_str(), c.c_str());
      for (auto i = 0; i < variables.size(); i++) {
         tree->Branch(variables[i].c_str(), &fEvent[i]);
      }
      trees.push_back(tree);
   }
   for (auto i = 0; i < x.GetShape()[0]; i++) {
      for (auto j = 0; j < x.GetShape()[1]; j++) {
         fEvent[j] = x(i, j);
      }
      trees[y(i)]->Fill();
   }

   // Add trees to dataloader
   for (auto i = 0; i < trees.size(); i++) {
      fDataLoader->AddTree(trees[i], classes[i].c_str(), 1.0, TCut(""), Types::kTraining);
   }
   fDataLoader->PrepareTrainingAndTestTree(TCut(""), TCut(""), dataloaderOptions.c_str());

   // Book method
   fFactory->BookMethod(fDataLoader, Types::kFisher, "Fisher", methodOptions);

   // Train
   fFactory->TrainAllMethods();

   // Create reader
   fReader = new Reader("Silent");
   for (auto i = 0; i < variables.size(); i++) {
      fReader->AddVariable(variables[i], &fEvent[i]);
   }

   // Book method in reader
   fReader->BookMVA("Fisher",
                    TString::Format("%s/weights/%s_Fisher.weights.xml", dataloaderName.c_str(), factoryName.c_str()));
}

template <typename T>
RTensor<T> RFisher<T>::Predict(RTensor<T> &x)
{
   RTensor<T> y({x.GetShape()[0]});
   for (auto i = 0; i < x.GetShape()[0]; i++) {
      for (auto j = 0; j < x.GetShape()[1]; j++) {
         fEvent[j] = x(i, j);
      }
      y(i) = fReader->EvaluateMVA("Fisher");
   }
   return y;
}

} // namespace Experimental
} // namespace TMVA

#endif // TMVA_RFISHER
