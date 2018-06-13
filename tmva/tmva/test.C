#include "ROOT/RDataFrame.hxx"
#include "TMVA/SomeModel.h"

#include <iostream>
#include <string>
#include <vector>

void test() {
  // Load data
  auto f = new TFile("test.root", "READ");
  auto t = (TTree *)f->Get("tree");
  auto df = ROOT::RDataFrame(*t);

  // Preprocessing
  auto df1 = df.Define("x_float", "float(x)");

  // Set up model
  auto model = TMVA::SomeModel<float>("pathToModelFile");

  // Run single prediction
  auto p = model.Predict({-1.f, 2.f});
  std::cout << "Single prediction: " << p[0] << std::endl;

  // Inference
  std::vector<std::string> variables = {"x_float", "y"};
  auto df2 =
      df1.Define("prediction", TMVA::Predict<2, float>::type(model), variables);
  auto df3 = df2.Define("test", "prediction[0]");

  // Store results
  df3.Snapshot("tree", "df.root", {"prediction", "test"});
}

int main() { test(); }
