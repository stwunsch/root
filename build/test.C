#include "TMVA/MinMaxScaler.h"

void test(){
    // Read out matrix
    ROOT::Experimental::TDataFrame tdf("TreeS", "tmva_class_example.root");
    tdf.Define("var5", "var1*var2");
    auto var1 = tdf.Take<float>("var1").GetValue();
    auto var2 = tdf.Take<float>("var2").GetValue();
    auto var3 = tdf.Take<float>("var3").GetValue();
    auto var4 = tdf.Take<float>("var4").GetValue();

    vector<float> x(var1.size()*4);
    for(UInt_t i=0; i<var1.size(); i++){
        x[4*i+0] = var1[i];
        x[4*i+1] = var2[i];
        x[4*i+2] = var3[i];
        x[4*i+3] = var4[i];
    }

    for(UInt_t i=0; i<4; i++) cout << x[i] << " ";
    cout << endl;

    // Prepare transformation
    TMVA::Tools::Instance();
    auto scaler = TMVA::Preprocessing::MinMaxScaler(-1, 1);
    scaler.Fit(x.data(), 6000, 4);

    // Run transformation
    scaler.Transform(x.data(), 6000);
    for(UInt_t i=0; i<4; i++) cout << x[i] << " ";
    cout << endl;

    // Store object to ROOT file
    scaler.SetName("my_transformation");
    scaler.SaveAs("from_cpp.root");
}
