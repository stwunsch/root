import ROOT
import numpy as np

from test import plot

if __name__ == "__main__":
    # Read out matrix
    tdf = ROOT.Experimental.TDataFrame("TreeS", "tmva_class_example.root")

    x_flat = np.zeros(24000, dtype=np.float32)
    for i_var, var in enumerate(["var1", "var2", "var3", "var4"]):
        for i_event, event in enumerate(tdf.TakeFloat(var)):
            x_flat[4*i_event+i_var] = event

    # Load transformation from file
    scaler = ROOT.TFile("from_cpp.root").Get("my_transformation")

    # Run transformation
    scaler.Transform(x_flat, 6000)
    plot(x_flat.reshape(6000, 4), "transform_loaded")

    scaler.InverseTransform(x_flat, 6000)
    plot(x_flat.reshape(6000, 4), "inverse_transform_loaded")
