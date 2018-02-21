#!/usr/bin/env python

import ROOT
from array import array
import numpy as np
np.random.seed(1234)

f = ROOT.TFile("large_foo.root", "RECREATE")
t = ROOT.TTree("TreeS", "TreeS")
arrays = {v: array('f', [-999]) for v in ["var1", "var2", "var3", "var4"]}
branches = {v: t.Branch(v, arrays[v], v + "/F") for v in arrays}
size = int(5 * 1e7)  # 1e7 equals 142M
for i in range(size):
    if i % (size / 10) == 0:
        print(i / float(size))
    for key in arrays:
        arrays[key][0] = np.random.randn()
    t.Fill()
f.Write()
f.Close()
