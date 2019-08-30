## \file
## \ingroup tutorial_pyroot
## \notebook -nodraw
## This tutorial illustrates how PyROOT supports declaring C++ callables from
## Python callables making the for example usable with RDataFrame.
##
## \macro_code
## \macro_output
##
## \date August 2019
## \author Stefan Wunsch

import ROOT

# To mark a Python callable to be used from C++, you have to use the decorator
# provided by PyROOT passing the C++ types of the input arguments and the return
# value.
@ROOT.DeclareCppCallable(["float", "int"], "float")
def pypow(x, y):
    return x**y

# The Python callable is now available from C++ in the CppCallable namespace.
# For example, we can use it from the interpreter.
ROOT.gInterpreter.ProcessLine('cout << "2^3 = " << CppCallable::pypow(2, 3) << endl;')

# Or we can use the callable as well within a RDataFrame workflow.
data = ROOT.RDataFrame(4).Define("x", "(float)rdfentry_")\
                         .Define("xpow2", "CppCallable::pypow(x, 2)")\
                         .AsNumpy()

for col in sorted(data):
    print("{}: {}".format(col, data[col]))

# The performance of using a Python callable from C++ is similar to a plain C++
# callable, if we can use numba in the back to compile it. See the callable below
# to enforce this behaviour.
@ROOT.DeclareCppCallable(["float"], "float", numba_only=True)
def numba_callable(x):
    return 2.0 * x

# However, if we process complex (C++) objects in the callable, we fall back to
# generic implementation. Be aware that this comes with a significant performance
# penalty. You should also be warned at runtime that the callable could not be
# compiled using numba.
@ROOT.DeclareCppCallable(["vector<float>"], "int")
def generic_callable(x):
    y = 0.0
    for v in x:
        y += v
    return y

ROOT.gInterpreter.ProcessLine("""
vector<float> x = {1, 2, 3};
cout << "sum({1, 2, 3}) =  " << CppCallable::generic_callable(x) << endl;
""")

# From each decorated callable, you can get the C++ wrapper code printing the
# __cpp_wrapper__ attribute of the callable.
print(generic_callable.__cpp_wrapper__)
