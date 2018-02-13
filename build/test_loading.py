import ROOT
import numpy as np

from test import plot

# Read out matrix
tdf = ROOT.Experimental.TDataFrame("TreeS", "tmva_class_example.root")
x = np.zeros((6000, 4), dtype=np.float32)
for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
    for i_event, event in enumerate(tdf.TakeFloat(var)):
        x[i_event, i_var] = event

# Flatten matrix
x_flat = np.ascontiguousarray(x.reshape(6000*4))
print("Is contiguous: {}".format(x_flat.flags['C_CONTIGUOUS']))
plot(x_flat.reshape(6000, 4), "original")

# Load transformation from file
f = ROOT.TFile("transformation.root")
scaler = f.Get("TMVA::Preprocessing::MinMaxScaler")

# Run transformation
scaler.Transform(x_flat, 6000)
plot(x_flat.reshape(6000, 4), "transform_loaded")

scaler.InverseTransform(x_flat, 6000)
plot(x_flat.reshape(6000, 4), "inverse_transform_loaded")
