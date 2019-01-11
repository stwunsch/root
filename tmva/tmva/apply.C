using namespace TMVA::Experimental;

TH1F predict(const char* name) {
    // Construct dataframe
    ROOT::RDataFrame df(name, "tmva_class_example.root");

    // Read data as RTensor
    auto x = AsTensor<float>(df, {"var1", "var2", "var3", "var4"});

    // Load method and compute prediction
    RReader reader("dataset/weights/TMVAClassification_Fisher.weights.xml");
    auto y = reader.Predict(x);

    // Fill histogram
    TH1F h(name, name, 50, -3, 3);
    for(auto& v: y) h.Fill(v);

    return h;
}

void apply() {
    auto sig = predict("TreeS");
    sig.SetLineColor(kRed);

    auto bkg = predict("TreeB");
    bkg.SetLineColor(kBlue);

    TCanvas c("c", "c", 800, 800);
    bkg.Draw("HIST");
    sig.Draw("HIST SAME");
    c.SaveAs("apply.pdf");
}
