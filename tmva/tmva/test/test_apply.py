import ROOT
import numpy as np
import matplotlib.pyplot as plt

RTensor = ROOT.TMVA.Experimental.RTensor
Inference = ROOT.TMVA.Experimental.Inference


def make_plot(scores, x_sig_numpy):
    scores_sig_numpy = scores_numpy[:x_sig_numpy.shape[0]]
    scores_bkg_numpy = scores_numpy[x_sig_numpy.shape[0]:]
    plt.figure(figsize=(5, 5))
    plt.hist(
        scores_bkg_numpy,
        bins=30,
        range=(-10, 10),
        histtype="step",
        lw=3,
        label="Background")
    plt.hist(
        scores_sig_numpy,
        bins=30,
        range=(-10, 10),
        histtype="step",
        lw=3,
        label="Signal")
    plt.xlabel("Fisher score")
    plt.ylabel("Count")
    plt.legend()
    plt.xlim((-10, 10))
    plt.savefig("fisher.png", bbox_inches="tight")


if __name__ == "__main__":
    # Load training data
    file_ = ROOT.TFile("tmva_class_example.root")
    tree_sig = file_.Get("TreeS")
    tree_bkg = file_.Get("TreeB")

    variables = ["var1", "var2", "var3", "var4"]
    x_sig_numpy = tree_sig.AsMatrix(columns=variables, dtype="float")
    x_bkg_numpy = tree_bkg.AsMatrix(columns=variables, dtype="float")
    x_numpy = np.vstack([x_sig_numpy, x_bkg_numpy])
    x = ROOT.AsTensor(x_numpy)
    file_.Close()

    # Apply MVA method
    fisher = Inference.Fisher("float")("fisher.root", "Fisher")
    scores = fisher.Predict(x)

    # Plot scores
    scores_numpy = np.asarray(scores)
    make_plot(scores_numpy, x_sig_numpy)
