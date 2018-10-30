################################################################################
# Make dataframe
################################################################################

import ROOT
df = ROOT.RDataFrame(10)
df2 = df.Define("x", "(float)tdfentry_")
df3 = df2.Define("y", "sqrt(x)")
df4 = df3.Define("z", "x*y")
df5 = df4.Filter("x>5")

################################################################################
# RDataFrame.AsNumpy
################################################################################

# Arguments
self = df5
columns = ["x", "y", "z"]

# Book take actions
take_helpers = {}
for column in columns:
    dtype = self.GetColumnType(column)
    take_helpers[column] = ROOT.ROOT.Detail.RDF.TakeHelper(dtype)(
        ROOT.ROOT.Detail.RDF.ToRNode(self), column)

# Get data in a single event loop
data_cpp = {}
for column in columns:
    data_cpp[column] = take_helpers[column].GetValue()

# Convert to numpy array via memory adoption
import numpy
data_npy = {}
for column in columns:
    data_npy[column] = numpy.asarray(data_cpp[column])

# Return values
data = data_npy

################################################################################
# Do stuff with it using the scipy stack
################################################################################

# numpy.mean
for values, label in zip(data.values(), data.keys()):
    print("mean of column {}: {}".format(label, numpy.mean(values)))

# pandas.DataFrame
import pandas
pdf = pandas.DataFrame(data)
print(pdf)

# matplotlib.pyplot
import matplotlib.pyplot as plt
plt.figure()
for values, label in zip(data.values(), data.keys()):
    plt.plot(values, label=label)
plt.legend()
plt.show()
