import ROOT
import numpy

t = ROOT.Test()

x = 9.0
print("Initial: {}".format(x))
print("Apply (default): {}".format(t.Apply(x)))

def f(x):
    return numpy.sqrt(x)
t.SetFunction(f)

print("Apply (Python function): {}".format(t.Apply(x)))
