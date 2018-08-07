import ROOT
import numpy as np
import matplotlib.pyplot as plt

RTensor = ROOT.TMVA.Experimental.RTensor
Training = ROOT.TMVA.Experimental.Training
Inference = ROOT.TMVA.Experimental.Inference


def print_tranformation(x, x_scaled):
    min_before = np.min(np.asarray(x), axis=0)
    max_before = np.max(np.asarray(x), axis=0)
    min_after = np.min(np.asarray(x_scaled), axis=0)
    max_after = np.max(np.asarray(x_scaled), axis=0)
    for i in range(4):
        print("Variable: {}".format(variables[i]))
        print("    Min (before/after transf.): {:.2f} / {:.2f}".format(
            min_before[i], min_after[i]))
        print("    Max (before/after transf.): {:.2f} / {:.2f}".format(
            max_before[i], max_after[i]))


def make_plot(scores, x_sig_numpy):
    scores_sig_numpy = scores_numpy[:x_sig_numpy.shape[0]]
    scores_bkg_numpy = scores_numpy[x_sig_numpy.shape[0]:]
    plt.figure(figsize=(5, 5))
    plt.hist(scores_bkg_numpy, histtype="step", lw=3, label="Background")
    plt.hist(scores_sig_numpy, histtype="step", lw=3, label="Signal")
    plt.xlabel("Fisher score")
    plt.ylabel("Count")
    plt.legend()
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
    y_numpy = np.hstack([
        np.ones(x_sig_numpy.shape[0], dtype="float32"),
        np.zeros(x_bkg_numpy.shape[0], dtype="float32")
    ])
    x = ROOT.AsTensor(x_numpy)
    y = ROOT.AsTensor(y_numpy)

    print("x: {}".format(x.GetShape()))
    print("y: {}".format(y.GetShape()))

    # Train preprocessing method
    scaler_train = Training.MinMaxScaler("float")(-3, 3)
    scaler_train.Fit(x)

    # Apply preprocessing method on inputs
    scaler_apply = Inference.MinMaxScaler("float")(scaler_train)
    x_scaled = scaler_apply.Transform(x)

    print_tranformation(x, x_scaled)

    # Train MVA method
    fisher_train = Training.Fisher("float")()
    fisher_train.Fit(x_scaled, y)

    # Apply MVA method
    fisher_apply = Inference.Fisher("float")(fisher_train)
    scores = fisher_apply.Predict(x_scaled)

    # Plot scores
    scores_numpy = np.asarray(scores)
    make_plot(scores_numpy, x_sig_numpy)
