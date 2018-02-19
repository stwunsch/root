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
        x[i_event,0] = t.var1
        x[i_event,1] = t.var2
        x[i_event,2] = t.var3
        x[i_event,3] = t.var4
    f.Close()

print(x)
