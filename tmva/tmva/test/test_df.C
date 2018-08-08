void test_df()
{
   // Create Fisher method for inference
   using TMVA::Experimental::Inference::Fisher;
   Fisher<float> fisher("test.root", "Fisher");

   // Load data in dataframe
   auto df_sig = ROOT::RDataFrame("TreeS", "tmva_class_example.root");
   auto df_bkg = ROOT::RDataFrame("TreeB", "tmva_class_example.root");

   // Book nodes running the application
   std::vector<std::string> variables = {"var1", "var2", "var3", "var4"};

   auto df_sig2 = df_sig.Define("fisher_response", ROOT::RDF::PassAsVec<4, float>(fisher), variables);
   auto df_sig3 = df_sig2.Define("fisher_sig_score", "fisher_response[0]");

   auto df_bkg2 = df_bkg.Define("fisher_response", ROOT::RDF::PassAsVec<4, float>(fisher), variables);
   auto df_bkg3 = df_bkg2.Define("fisher_sig_score", "fisher_response[0]");

   // Book histograms
   auto h_sig = df_sig3.Histo1D({"h_sig", "h_sig", 30, -3, 3}, "fisher_sig_score");
   auto h_bkg = df_bkg3.Histo1D({"h_bkg", "h_bkg", 30, -3, 3}, "fisher_sig_score");

   // Plot
   h_sig->SetLineColor(kRed);
   h_sig->SetLineWidth(3);
   h_bkg->SetLineColor(kBlue);
   h_bkg->SetLineWidth(3);
   h_sig->SetTitle("");
   h_sig->GetXaxis()->SetTitle("Fisher score");
   h_sig->GetYaxis()->SetTitle("Count");
   gStyle->SetOptStat(0);

   TCanvas c("c", "c", 800, 600);
   h_sig->Draw("HIST");
   h_bkg->Draw("SAME");
   c.Print("fisher.pdf");
}
