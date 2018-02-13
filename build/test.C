#include "TMVA/MinMaxScaler.h"

void test(){
    ROOT::Experimental::TDataFrame tdf("TreeS", "tmva_class_example.root");
    tdf.Define("var5", "var1*var2");
    auto x = tdf.Take<float>("var1");
    cout << x->size() << endl;
    return;

    TMVA::Tools::Instance();
    float inputs[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    auto scaler = TMVA::Preprocessing::MinMaxScaler();
    scaler.PresentInputs(inputs, 3, 2);
    scaler.Finalize();

    std::cout << "Original data:" << std::endl;
    for(UInt_t i=0; i<6; i++) std::cout << inputs[i] << " ";
    std::cout << std::endl;

    std::cout << "Transformation:" << std::endl;
    scaler.Transform(inputs, 3);
    for(UInt_t i=0; i<6; i++) std::cout << inputs[i] << " ";
    std::cout << std::endl;

    std::cout << "Inverse transformation:" << std::endl;
    scaler.InverseTransform(inputs, 3);
    for(UInt_t i=0; i<6; i++) std::cout << inputs[i] << " ";
    std::cout << std::endl;
}
