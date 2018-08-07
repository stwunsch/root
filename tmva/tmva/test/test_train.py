import ROOT
import numpy as np
import matplotlib.pyplot as plt

RTensor = ROOT.TMVA.Experimental.RTensor
Training = ROOT.TMVA.Experimental.Training


if __name__ == "__main__":
    # Load training data
    file_ = ROOT.TFile("tmva_class_example.root")
    tree_sig = file_.Get("TreeS")
    tree_bkg = file_.Get("TreeB")

    variables = ["var1", "var2", "var3", "var4"]
    x_sig_numpy = tree_sig.AsMatrix(columns=variables, dtype="float")
    x_bkg_numpy = tree_bkg.AsMatrix(columns=variables, dtype="float")
    x_numpy = np.vstack([x_sig_numpy, x_bkg_numpy])
    y_numpy = np.hstack([
        np.ones(x_sig_numpy.shape[0], dtype="float32"),
        np.zeros(x_bkg_numpy.shape[0], dtype="float32")
    ])
    x = ROOT.AsTensor(x_numpy)
    y = ROOT.AsTensor(y_numpy)
    file_.Close()

    # Train MVA method
    fisher = Training.Fisher("float")()
    fisher.Fit(x, y)

    # Serialize method to file
    fisher.SetName("Fisher")
    f = ROOT.TFile("test.root", "RECREATE")
    fisher.Write()
    f.Close()
