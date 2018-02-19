import numpy as np
import ROOT
import sys
import argparse
import root_numpy

parser = argparse.ArgumentParser(description="Test TDFs as dataloader.")
parser.add_argument("file", type=str, help="Test file")
parser.add_argument("mode", type=str, help="Testing mode")
parser.add_argument(
    "num_threads",
    type=int,
    help="Number of threads to be used by ROOT's implicit multi-threading")
args = parser.parse_args()

ROOT.ROOT.EnableImplicitMT(args.num_threads)
print("Pool size: {}".format(ROOT.ROOT.GetImplicitMTPoolSize()))

if args.mode == "classic":
    print("classic")
    f = ROOT.TFile(args.file)
    t = f.Get("TreeS")
    x = np.zeros((t.GetEntries(), 4), dtype=np.float32)
    for i_event, event in enumerate(t):
        for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
            x[i_event, i_var] = getattr(event, var)
    f.Close()

elif args.mode == "tdf_ugly":
    print("tdf_ugly")
    tdf = ROOT.Experimental.TDataFrame("TreeS", args.file)
    x = np.zeros((600000, 4), dtype=np.float32)
    for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
        for i_event, event in enumerate(tdf.TakeFloat(var)):
            x[i_event, i_var] = event

elif args.mode == "tdf":
    print("tdf")
    tdf = ROOT.Experimental.TDataFrame("TreeS", args.file)
    x = np.array([
        np.fromiter(tdf.TakeFloat(v), np.float32)
        for v in ["var1", "var2", "var3", "var4"]
    ])
    x = x.T

elif args.mode == "root_numpy":
    print("root_numpy")
    f = ROOT.TFile(args.file)
    t = f.Get("TreeS")
    x = np.array([
        list(x) for x in root_numpy.tree2array(
            t, branches=["var1", "var2", "var3", "var4"])
    ])

elif args.mode == "future":
    tdf = ROOT.Experimental.TDataFrame("TreeS", args.file)
    x = tdf.Take(["var1", "var2", "var3", "var4"])

print(x.flags)
print(x)
