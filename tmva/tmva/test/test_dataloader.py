import ROOT

Utility = ROOT.TMVA.Experimental.Utility
Training = ROOT.TMVA.Experimental.Training

# Create dataframes for each class
df_sig = ROOT.RDataFrame("TreeS", "tmva_class_example.root")
df_bkg = ROOT.RDataFrame("TreeB", "tmva_class_example.root")

# Set up a dataloader
variables = ROOT.std.vector("string")()
for var in ["var1", "var2", "var3", "var4"]:
    variables.push_back(var)

dataloader = Utility.DataLoader("float")(variables)
dataloader.AddDataFrame(df_sig, "Signal")
dataloader.AddDataFrame(df_bkg, "Background")

# Get inputs
x_sig = dataloader.GetInputs("Signal")
x_bkg = dataloader.GetInputs("Background")

# Get targets
y_sig = dataloader.GetTargets("Signal", Utility.TargetType.Labeled)
y_bkg = dataloader.GetTargets("Background", Utility.TargetType.Labeled)

# Convert to numpy via memory adoption
import numpy as np

x_numpy = np.vstack((np.asarray(x_sig), np.asarray(x_bkg)))
y_numpy = np.hstack((np.asarray(y_sig), np.asarray(y_bkg)))

print("x: {}".format(x_numpy.shape))
print("y: {}".format(y_numpy.shape))

# Train a classifier with the data
x = ROOT.AsTensor(x_numpy)
y = ROOT.AsTensor(y_numpy)

fisher = Training.Fisher("float")()
fisher.Fit(x, y)

# Serialize trained classifier
fisher.SetName("Fisher")
f = ROOT.TFile("fisher.root", "RECREATE")
fisher.Write()
f.Close()
