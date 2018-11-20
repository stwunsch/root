void test_asmatrix() {
    auto df = ROOT::RDataFrame(5);
    auto df2 = df.Define("x", "float(tdfentry_)");
    auto df3 = df2.Define("y", "2*int(tdfentry_)");

    auto df_rnode = (ROOT::RDF::RNode)df3;
    auto columns = std::vector<string>();
    columns.push_back("x");
    columns.push_back("y");

    auto helper = PyROOT::AsMatrixHelper<float, float, int>();
    auto call_helper = PyROOT::CallHelper<float, float, int>();
    auto vec = call_helper.Call(df_rnode, helper, columns);
}
