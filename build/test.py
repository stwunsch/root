import ROOT
import numpy as np

ROOT.TMVA.Tools.Instance()

x = np.array([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], np.float32)
scaler = ROOT.TMVA.Preprocessing.MinMaxScaler()
scaler.PresentInputs(x, 3, 2)

print("Original:", x)

scaler.Transform(x, 3)
print("Transformation:", x)

scaler.InverseTransform(x, 3)
print("Inverse transformation:", x)
