import ROOT
import numpy as np
from time import time
import uproot


def run_rdf(filename):
    df = ROOT.RDataFrame("Events", filename)
    df_rnode = ROOT.PyROOT.AsRNode(df)
    columns = ROOT.std.vector("string")()
    columns.push_back("nMuon")
    columns.push_back("nElectron")

    helper = ROOT.PyROOT.AsMatrixHelper("unsigned int", "unsigned int", "unsigned int")()
    call_helper = ROOT.PyROOT.CallHelper("unsigned int", "unsigned int", "unsigned int")()
    vec = call_helper.Call(df_rnode, helper, columns)
    flat_npy = np.asarray(vec)
    num_columns = 2
    npy = flat_npy.reshape(flat_npy.size/num_columns, num_columns)
    return npy


def test_rdf(filename, num_threads=None):
    if num_threads != None:
        ROOT.ROOT.EnableImplicitMT(num_threads)
    start = time()
    npy = run_rdf(filename)
    end = time()
    print("Runtime RDF: {}".format(end-start))
    print("Test: {}, {}, {}".format(np.mean(npy, axis=0), npy.shape, npy.dtype))
    print(npy.flags)


def test_pyroot(filename):
    start = time()
    npy = run_pyroot(filename)
    end = time()
    print("Runtime pyroot: {}".format(end-start))
    print("Test: {}, {}, {}".format(np.mean(npy, axis=0), npy.shape, npy.dtype))
    print(npy.flags)


def run_pyroot(filename):
    col1 = []
    col2 = []
    f = ROOT.TFile(filename)
    tree = f.Get("Events")
    for event in tree:
        col1.append(event.nMuon)
        col2.append(event.nElectron)
    npy = np.vstack([np.array(col1), np.array(col2)]).T
    f.Close()
    return npy


def run_uproot(filename):
    f = uproot.open(filename)["Events"]
    cols = f.arrays(["nMuon", "nElectron"])
    npy = np.vstack([cols["nMuon"], cols["nElectron"]]).T
    return npy


def test_uproot(filename):
    start = time()
    npy = run_uproot(filename)
    end = time()
    print("Runtime uproot: {}".format(end-start))
    print("Test: {}, {}, {}".format(np.mean(npy, axis=0), npy.shape, npy.dtype))
    print(npy.flags)


if __name__ == "__main__":
    filename = "/home/stefan/Run2012B_DoubleMuParked_1000000.root"
    test_uproot(filename)
    #test_pyroot(filename)
    test_rdf(filename, num_threads=4)
