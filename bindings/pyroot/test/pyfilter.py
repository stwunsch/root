import ROOT
import numpy as np

# Create file
dfx = ROOT.ROOT.RDataFrame(10)
dfx1 = dfx.Define("b1", "(float) tdfentry_").Snapshot("tree", "test.root")

# Test
df1 = ROOT.ROOT.RDataFrame("tree", "test.root")

def f(x):
    return True if np.sqrt(x) > np.pi/2 and x < 2*np.pi else False

df2 = ROOT.PyFilter(df1, f, ["b1"], "Filter with Python callable")
df2.Snapshot("tree", "test2.root")

# Check
f = ROOT.TFile("test.root")
t = f.Get("tree")
print("Before: {}".format(np.squeeze(t.AsMatrix())))

f = ROOT.TFile("test2.root")
t = f.Get("tree")
print("After: {}".format(np.squeeze(t.AsMatrix())))
