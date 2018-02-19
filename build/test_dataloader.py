import numpy as np
import ROOT
import sys


x = np.zeros((6000, 4), dtype=np.float32)

if sys.argv[1] == "tdf":
    tdf = ROOT.Experimental.TDataFrame("TreeS", "tmva_class_example.root")
    for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
        for i_event, event in enumerate(tdf.TakeFloat(var)):
            x[i_event,i_var] = event

elif sys.argv[1] == "classic":
    f = ROOT.TFile("tmva_class_example.root")
    t = f.Get("TreeS")
    for i_event, event in enumerate(t):
        for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
            x[i_event,i_var] = getattr(event, var)
    f.Close()

elif sys.argv[1] == "future":
    tdf = ROOT.Experimental.TDataFrame("TreeS", "tmva_class_example.root")
    x = tdf.Take(["var1", "var2", "var3", "var4"])


print(x)
