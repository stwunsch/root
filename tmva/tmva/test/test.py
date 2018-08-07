import ROOT
import numpy as np
RTensor = ROOT.TMVA.Experimental.RTensor
Training = ROOT.TMVA.Experimental.Training
Inference = ROOT.TMVA.Experimental.Inference

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
scaler_train = Training.MinMaxScaler("float")()
scaler_train.Fit(x)

# Apply preprocessing method on inputs
scaler_apply = Inference.MinMaxScaler("float")(scaler_train)
x_scaled = scaler_apply.Transform(x)

# Train MVA method

# Apply MVA method

# Plot ROC
