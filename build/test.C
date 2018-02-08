#include "TMVA/MinMaxScaler.h"

void test(){
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
