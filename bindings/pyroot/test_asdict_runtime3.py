import ROOT
import numpy as np
from time import time
import uproot


keep_alive = []


def run_rdf(filename):
    df = ROOT.RDataFrame("Events", filename)
    df_rnode = ROOT.PyROOT.AsRNode(df)
    helper = ROOT.PyROOT.TakeHelper("unsigned int")()
    x = helper.Take(df_rnode, "nMuon")
    y = helper.Take(df_rnode, "nElectron")
    keep_alive.append(x)
    keep_alive.append(y)
    x_ = x.GetValue()
    y_ = y.GetValue()
    dic = {"nMuon": np.asarray(x_), "nElectron": np.asarray(y_)}
    return dic


def test_rdf(filename, num_threads=None):
    if num_threads != None:
        ROOT.ROOT.EnableImplicitMT(num_threads)
    start = time()
    dic = run_rdf(filename)
    end = time()
    print("Runtime RDF: {}".format(end-start))
    print("Test: {}, {}, {}".format(np.mean(dic["nMuon"]), dic["nMuon"].size, dic["nMuon"].dtype))
    print("Test: {}, {}, {}".format(np.mean(dic["nElectron"]), dic["nElectron"].size, dic["nMuon"].dtype))
    print(dic["nMuon"].flags)


def run_uproot(filename):
    f = uproot.open(filename)["Events"]
    cols = f.arrays(["nMuon", "nElectron"])
    dic = {"nMuon": cols["nMuon"], "nElectron": cols["nElectron"]}
    return dic


def test_uproot(filename):
    start = time()
    dic = run_uproot(filename)
    end = time()
    print("Runtime uproot: {}".format(end-start))
    print("Test: {}, {}, {}".format(np.mean(dic["nMuon"]), dic["nMuon"].size, dic["nMuon"].dtype))
    print("Test: {}, {}, {}".format(np.mean(dic["nElectron"]), dic["nElectron"].size, dic["nMuon"].dtype))
    print(dic["nMuon"].flags)


if __name__ == "__main__":
    filename = "/home/stefan/Run2012B_DoubleMuParked_1000000.root"
    test_uproot(filename)
    test_rdf(filename, num_threads=4)
