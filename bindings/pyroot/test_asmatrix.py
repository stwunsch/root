import ROOT

# Make simple dataframe
df = ROOT.RDataFrame(10)
df2 = df.Define("x", "float(tdfentry_)")
df3 = df2.Define("y", "2*int(tdfentry_)")
df4 = df3.Filter("x>4")

# Prepare node
df_rnode = ROOT.PyROOT.AsRNode(df4)
columns = ROOT.std.vector("string")()
columns.push_back("x")
columns.push_back("y")

# Register helper and call
helper = ROOT.PyROOT.AsMatrixHelper("float", "float", "int")()
call_helper = ROOT.PyROOT.CallHelper("float", "float", "int")()
vec = call_helper.Call(df_rnode, helper, columns)

# Adopt memory
import numpy as np
flat_npy = np.asarray(vec)
num_columns = 2
npy = flat_npy.reshape(flat_npy.size/num_columns, num_columns)
print(np.sum(npy))

# Push to pandas for nicer plotting
import pandas
pdf = pandas.DataFrame(data=npy, columns=[c for c in columns])
print(pdf)
