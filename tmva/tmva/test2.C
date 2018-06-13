#include "ROOT/RDataFrame.hxx"
#include "TMVA/SomeModel.h"

#include <iostream>
#include <string>
#include <vector>

void test()
{
   // Load data
   auto f = new TFile("test.root", "READ");
   auto t = (TTree *)f->Get("tree");
   auto df = ROOT::RDataFrame(*t);

   // Preprocessing
   auto df1 = df.Define("x_float", "float(x)");

   // Make inputs
   std::vector<std::string> variables = {"x_float", "y"};
   auto df2 = df1.Define("inputs", TMVA::MakeVector<2, float>::type(), variables);
   auto df3 = df2.Define("input0", "inputs[0]");

   // Set up model
   auto model = TMVA::SomeModel<float>("pathToModelFile");

   // Run single prediction
   auto p = model.Predict({-1.f, 2.f});
   std::cout << "Single prediction: " << p[0] << std::endl;

   // Inference on dataframe
   auto df4 = df3.Define("predictions", model, {"inputs"});

   // Store results
   df4.Snapshot("tree", "df.root", {"predictions", "inputs", "input0", "x"});
}

int main()
{
   test();
}
