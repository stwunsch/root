import ROOT

df = ROOT.RDataFrame(10)
df2 = df.Define("x", "(unsigned int)tdfentry_").Define("y", "2*(unsigned int)tdfentry_")
df_rnode = ROOT.PyROOT.AsRNode(df2)
helper = ROOT.PyROOT.TakeHelper("unsigned int")()
x = helper.Take(df_rnode, "x")
y = helper.Take(df_rnode, "y")
print(x.GetValue())
print(y.GetValue())
