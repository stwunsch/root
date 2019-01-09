using namespace TMVA::Experimental;

void test()
{
   // Load data
   ROOT::RDataFrame df_sig("TreeS", "tmva_class_example.root");
   ROOT::RDataFrame df_bkg("TreeB", "tmva_class_example.root");
   vector<string> vars = {"var1", "var2", "var3", "var4"};
   auto sig = AsTensor<float>(df_sig, vars, MemoryOrder::RowMajor);
   auto bkg = AsTensor<float>(df_bkg, vars, MemoryOrder::RowMajor);
   auto x_data = sig.GetDataAsVec();
   for (auto &v : bkg.GetDataAsVec())
      x_data.push_back(v);
   auto x = RTensor<float>(x_data.data(), {12000, 4});
   vector<float> y_data(12000);
   for (auto i = 0; i < 6000; i++) {
      y_data[i] = 1.0;
      y_data[6000 + i] = 0.0;
   }
   auto y = RTensor<float>(y_data.data(), {12000});

   // Train model
   RFisher<float> model;
   model.Train(x, y);

   // Make prediction
   auto p_sig = model.Predict(sig);
   auto p_bkg = model.Predict(bkg);

   // Plot results
   TH1F h_sig("h_sig", "h_sig", 50, -3, 3);
   for (auto &v : p_sig)
      h_sig.Fill(v);
   h_sig.SetLineWidth(2);
   h_sig.SetLineColor(kRed);

   TH1F h_bkg("h_bkg", "h_bkg", 50, -3, 3);
   for (auto &v : p_bkg)
      h_bkg.Fill(v);
   h_bkg.SetLineWidth(2);
   h_bkg.SetLineColor(kBlue);
   h_bkg.SetTitle("");
   h_bkg.GetXaxis()->SetTitle("MVA score");
   h_bkg.GetYaxis()->SetTitle("Count");

   gStyle->SetOptStat(0);
   TCanvas c("c", "c", 600, 600);
   c.cd();
   h_bkg.Draw("HIST");
   h_sig.Draw("HIST SAME");
   c.SaveAs("test.png");
}
