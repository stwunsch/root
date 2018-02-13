import ROOT
import numpy as np
import matplotlib.pyplot as plt

def plot(x_, name):
    plt.figure()
    plt.clf()
    for i in range(4):
        plt.hist(x_[:,i], 20, histtype="step", lw=3, label="var{}".format(i+1))
    plt.title(name)
    plt.legend()
    plt.savefig("{}.png".format(name))

# Read out matrix
tdf = ROOT.Experimental.TDataFrame("TreeS", "tmva_class_example.root")
x = np.zeros((6000, 4), dtype=np.float32)
for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
    for i_event, event in enumerate(tdf.TakeFloat(var)):
        x[i_event, i_var] = event

# Flatten matrix
x_flat = x.reshape(6000*4)
plot(x_flat.reshape(6000, 4), "original")

# Prepare transformation
ROOT.TMVA.Tools.Instance()
scaler = ROOT.TMVA.Preprocessing.MinMaxScaler()
scaler.Fit(x_flat, 6000, 4)

# Run transformation
scaler.Transform(x_flat, len(x_flat))
plot(x_flat.reshape(6000, 4), "transform")

scaler.InverseTransform(x_flat, len(x_flat))
plot(x_flat.reshape(6000, 4), "inverse_transform")
