import ROOT
import pandas
import argparse
import time
import numpy as np

def print_tree(tree):
    data, columns = tree.AsMatrix(return_labels=True)
    x = pandas.DataFrame(data=data, columns=columns)
    print(x)

def fill_tree(treeName, fileName, numEntries):
    tdf = ROOT.ROOT.Experimental.TDataFrame(int(numEntries))
    tdf.Define("b1", "(float) tdfentry_")\
       .Define("b2", "(float) tdfentry_ * tdfentry_").Snapshot(treeName, fileName)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument("mode", type=str, help="Mode")
    parser.add_argument("--debug", default=False, action="store_true", help="Debug")
    parser.add_argument("--mt", default=False, action="store_true", help="Multi-threading")
    parser.add_argument("--create", default=False, action="store_true", help="Create file")
    args = parser.parse_args()

    # Toggle multi-threading
    if args.mt:
        ROOT.ROOT.EnableImplicitMT()

    # Create tree
    fileName = "tdf001_introduction_py.root"
    treeName = "myTree"
    numEntries = 5e6
    if args.create:
        fill_tree(treeName, fileName, numEntries)

    # Create dataframe
    file_ = ROOT.TFile(fileName)
    tree = file_.Get(treeName)
    if args.debug:
        print_tree(tree)
    df = ROOT.Experimental.TDataFrame(tree)

    # Apply filter
    def f(x):
        return True if np.sqrt(x) > 100 else False

    if "py" in args.mode:
        print("Mode: py")
        columns = ROOT.std.vector("string")()
        columns.push_back("b1")
        df2 = df.PyFilter(f, columns)
    elif "cpp" in args.mode:
        print("Mode: cpp")
        columns = ROOT.std.vector("string")()
        df2 = df.Filter("sqrt(b1)>100")
    else:
        raise Exception("Mode not known.")

    # Make snapshot
    columns = ROOT.vector("string")()
    columns.push_back("b1")
    columns.push_back("b2")

    start = time.time()
    df2.Snapshot("tree", "test.root", columns)
    end = time.time()
    print("Runtime: {}s".format((end-start)))

    if args.debug:
        # Reload and print
        file_ = ROOT.TFile("test.root")
        tree = file_.Get("tree")
        print_tree(tree)
