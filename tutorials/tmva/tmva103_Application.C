/// \file
/// \ingroup tutorial_tmva
/// \notebook -nodraw
///
/// \macro_code
/// \macro_output
///
/// \date December 2018
/// \author Stefan Wunsch

using namespace TMVA::Experimental;
using namespace ROOT::VecOps;

void tmva102_Application()
{
    // Load BDT model
    BDT bdt("BDT", "model.root");

    // Apply model on a single input
    auto y1 = bdt.Compute({1.0, 2.0, 3.0, 4.0});

    std::cout << "Apply on a single input vector: " << y1 << std::endl;

    // Apply model on a batch of inputs
    const auto filepath = "root://eospublic.cern.ch//eos/root-eos/cms_opendata_2012_nanoaod/SMHiggsToZZTo4L.root";
    ROOT::RDataFrame df("Events", filepath);
    auto df2 = df.Define("Muon_pt_1", "Muon_pt[0]")
                 .Define("Muon_pt_2", "Muon_pt[1]")
                 .Define("Electron_pt_1", "Electron_pt[0]")
                 .Define("Electron_pt_2", "Electron_pt[1]");
    std::vector<std::string> variables = {"Muon_pt_1", "Muon_pt_2", "Electron_pt_1", "Electron_pt_2"};
    auto x = AsRTensor<float>(df2, variables);

    auto y2 = bdt.Compute(x);

    std::cout << "Apply on an input tensor: " << y2 << std::endl;

    // Apply model as part of a RDataFrame workflow
    auto df3 = df2.Define("y", Compute<4, float>(bdt), variables);

    std::cout << "Mean response on signal sample: " << df3.Mean("y") << std::endl;
}
