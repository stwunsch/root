import ROOT

# Make simple dataframe
df = ROOT.RDataFrame(5)
df2 = df.Define("x", "float(tdfentry_)")
df3 = df2.Define("y", "2*int(tdfentry_)")

# Prepare node
df_rnode = ROOT.PyROOT.AsRNode(df3)
columns = ROOT.std.vector("string")()
columns.push_back("x")
columns.push_back("y")

# Register helper and call
helper = ROOT.PyROOT.AsNumpyHelper("float", "int")()
call_helper = ROOT.PyROOT.CallHelper("float", "int")()
pyobj = call_helper.Call(df_rnode, helper, columns)
print(pyobj)
print(type(pyobj))
