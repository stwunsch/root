import ROOT
RTensor = ROOT.TMVA.Experimental.RTensor
import numpy as np

# Create tensor with shape (3, 2) and adopted memory
data = ROOT.std.vector("float")(6)
for i in range(data.size()):
    data[i] = i
shape = ROOT.std.vector("size_t")(2)
shape[0], shape[1] = 3, 2
x = RTensor("float")(data.data(), shape)
print("Initial RTensor:\n{}\n".format(x))

# Show getitem and setitem
print("Get entry at [0,0]: {}\n".format(x[0, 0]))

x[0, 0] = 42.0
print("Set entry [0,0] to 42: {}\n".format(x[0, 0]))

# Adopt memory via numpy's array interface
x_np = np.asarray(x)
print("Numpy array with adopted memory:\n{}\n".format(x_np))
x_np[0, 0] = 999
print("Numpy array with element [0,0] set to 999:\n{}\n".format(x_np))

# Show change in tensor
print("Again RTensor:\n{}\n".format(x))

# Adopt memory of numpy array
y_np = np.array([[0, 1, 2], [3, 4, 5]], dtype="float32")
print("Initial Numpy array:\n{}\n".format(y_np))

y = ROOT.AsTensor(y_np)
print("RTensor with adopted memory from Numpy array:\n{}\n".format(y))

# Show that shape matches
print("Shapes of RTensor and Numpy array: {}, {}\n".format(
    y.GetShape(), y_np.shape))

# Show that memory got adopted
y_np[0, 0] = 42
print("RTensor with first element set to 42:\n{}\n".format(y))

# Show change in numpy array
print("Again Numpy array:\n{}\n".format(y_np))
