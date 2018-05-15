import ROOT
import numpy
from time import time

num_calls = int(1e8)
t = ROOT.Test(num_calls)

x = 9.0
print("Initial: {}".format(x))
print("Apply (default): {}".format(t.Apply(x)))


def f(x):
    return 2.0 * x


t.SetFunction(f)

# Benchmark internal call
start = time()
print("Apply (Python function): {}".format(t.Apply(x)))
end = time()
runtime_internal = end - start
print("Runtime (internal): {}".format(runtime_internal))

# Benchmark native call
start = time()
for i in range(num_calls):
    f(x)
end = time()
runtime_native = end - start
print("Runtime (native): {}".format(runtime_native))

print("Native / internal: {}".format(runtime_native / runtime_internal))
