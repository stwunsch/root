void skim_nanoaod() {
    string filename = "/home/stefan/cms_opendata_higgs/Run2012B_DoubleMuParked.root";
    const auto numEvents = 10000000;
    auto df = ROOT::RDataFrame("Events", filename);
    auto df2 = df.Range(numEvents);
    df2.Snapshot("Events", "/home/stefan/Run2012B_DoubleMuParked_10000000.root", {"nMuon", "nElectron"});
}
