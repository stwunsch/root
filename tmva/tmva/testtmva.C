#include "ROOT/RDataFrame.hxx"
#include "TMVA/Preprocessing.h"
#include "TMVA/Application.h"
#include "TMVA/Utility.h"

#include <iostream>
#include <string>
#include <vector>

/*
 * To be solved:
 * - Batch processing -> RTensor?
 * - Processing of high-dimensional data? -> RTensor?
 * - Thread-safety of TMVA/other libraries? Copy constructors?
 */

template <typename T>
using RTensor = typename std::vector<std::vector<T>>;

void test()
{
   // Run multi-threaded
   // ROOT::EnableImplicitMT();

   // Load data
   auto f = new TFile("/home/stefan/root-dev/tmva/tmva/dataset.root", "READ");
   auto t = (TTree *)f->Get("TreeS");
   auto df = ROOT::RDataFrame(*t);
   std::cout << "Dataset size: " << t->GetEntries() << std::endl;

   // Dataframe based preprocessing
   auto df1 = df.Define("new_var", "var1*var2");

   // Make inputs
   std::vector<std::string> variables = {"var1", "var2", "var3", "var4"};
   auto df2 = df1.Define("inputs", TMVA::Utility::MakeVector<4, float>::type(), variables);

   // ML based preprocessing
   auto preprocessing = TMVA::Preprocessing::SomeMethod<float>("path_to_serialized_parameters");

   // Set up model
   auto model = TMVA::Application::SomeMethod<float>(
      "/home/stefan/root-dev/tmva/tmva/dataset/weights/TMVAClassification_Fisher.weights.xml");

   // Run single prediction
   std::vector<float> inputs = {1.f, 2.f, 3.f, 4.f};
   auto p = model.Predict(preprocessing.Transform(inputs));
   std::cout << "Single prediction: " << p[0] << std::endl;

   // Batch processing of inputs
   RTensor<float> multipleInputs = {{1.f, 2.f, 3.f, 4.f}, {-1.f, -2.f, -3.f, -4.f}};
   auto p2 = model.Predict(preprocessing.Transform(multipleInputs));
   std::cout << "Batch prediction: " << p2[0][0] << ", " << p2[1][0] << std::endl;

   // Inference on dataframe (model only)
   auto df3 = df2.Define("predictions_nopreprocessing", model, {"inputs"});

   // Inference on dataframe (chain preprocessing and model)
   auto df4 = df3.Define("predictions", TMVA::Utility::Chain<float>(preprocessing, model), {"inputs"});

   // Store results
   df4.Snapshot("tree", "df.root", {"predictions", "inputs"});
}

int main()
{
   test();
}
