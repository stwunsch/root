import ROOT

# Initialize from data via memory adoption
data = ROOT.std.vector("float")(6)
for i in range(len(data)):
    data[i] = i + 1.0

shape = ROOT.std.vector("size_t")(2)
shape[0] = 2
shape[1] = 3

x = ROOT.TMVA.Experimental.RTensor("float")(data.data(), shape)

print("Initialize tensor:\n\n{}\n".format(x))

# Access element

print("Access elements:\n\n{}, {}, {}\n".format(x[0, 0], x[0, 1], x[0, 2]))

# Set elements

v = -1.0
for i in range(x.Shape()[0]):
    for j in range(x.Shape()[1]):
        x[i, j] = v
        v -= 1.0

print("Set elements:\n\n{}\n".format(x))

# Reshape tensor

shape[0] = 6
shape[1] = 1
x.Reshape(shape)

print("Reshape tensor:\n\n{}\n".format(x))

# Squeeze tensor

x.Squeeze()

print("Squeeze tensor:\n\n{}\n".format(x))

# Expand dimensions

x.ExpandDims(0)

print("Expand dimensions:\n\n{}\n".format(x))

# Reshape back

shape[0] = 2
shape[1] = 3
x.Reshape(shape)

print("Reshape tensor:\n\n{}\n".format(x))

# Convert to numpy array
import numpy as np

x_np = np.asarray(x)

print("Convert to numpy array:\n\n{}\n".format(x_np))
