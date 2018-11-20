import ROOT
import numpy as np
from time import time
import uproot


def run_rdf(filename):
    df = ROOT.RDataFrame("TreeS", filename)
    df_rnode = ROOT.PyROOT.AsRNode(df)
    columns = ROOT.std.vector("string")()
    columns.push_back("var1")
    columns.push_back("var2")
    columns.push_back("var3")
    columns.push_back("var4")

    helper = ROOT.PyROOT.AsMatrixHelper("float", "float", "float", "float", "float")()
    call_helper = ROOT.PyROOT.CallHelper("float", "float", "float", "float", "float")()
    vec = call_helper.Call(df_rnode, helper, columns)
    flat_npy = np.asarray(vec)
    num_columns = 4
    npy = flat_npy.reshape(flat_npy.size/num_columns, num_columns)
    return npy


def test_rdf(filename, num_threads=None):
    if num_threads != None:
        ROOT.ROOT.EnableImplicitMT(num_threads)
    start = time()
    npy = run_rdf(filename)
    end = time()
    print("Runtime RDF: {}".format(end-start))
    print("Test: {}, {}".format(np.mean(npy, axis=0), npy.shape))
    print(npy.flags)


def test_pyroot(filename):
    start = time()
    npy = run_pyroot(filename)
    end = time()
    print("Runtime pyroot: {}".format(end-start))
    print("Test: {}, {}".format(np.mean(npy, axis=0), npy.shape))
    print(npy.flags)


def run_pyroot(filename):
    col1 = []
    col2 = []
    col3 = []
    col4 = []
    f = ROOT.TFile(filename)
    tree = f.Get("TreeS")
    for event in tree:
        col1.append(event.var1)
        col2.append(event.var2)
        col3.append(event.var3)
        col4.append(event.var4)
    npy = np.vstack([np.array(col1), np.array(col2), np.array(col3), np.array(col4)]).T
    f.Close()
    return npy


def run_uproot(filename):
    f = uproot.open(filename)["TreeS"]
    cols = f.arrays(["var1", "var2", "var3", "var4"])
    npy = np.vstack([cols["var1"], cols["var2"], cols["var3"], cols["var4"]]).T
    return npy


def test_uproot(filename):
    start = time()
    npy = run_uproot(filename)
    end = time()
    print("Runtime uproot: {}".format(end-start))
    print("Test: {}, {}".format(np.mean(npy, axis=0), npy.shape))
    print(npy.flags)


if __name__ == "__main__":
    filename = "/home/stefan/tmva_class_example.root"
    test_uproot(filename)
    test_pyroot(filename)
    test_rdf(filename, num_threads=4)
