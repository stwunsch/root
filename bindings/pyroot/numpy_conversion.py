import ROOT
import numpy as np


# TODO: Move this magic to ROOT.py
def asarray(obj):
    c = ROOT.TNumpyConverter("float")()
    c.GetAttributes(obj) # Gets pointer to data and shape
    return c.AsTVec()
ROOT.asarray = asarray


if __name__ == "__main__":
    np_array = np.ones(2, dtype=np.float32)
    tvec = ROOT.asarray(np_array)
    print(np_array.__array_interface__)
    print("numpy:", np_array)
    print("tvec:", [tvec[i] for i in range(tvec.size())])

    np_array[0] = 0.0
    print("numpy:", np_array)
    print("tvec:", [tvec[i] for i in range(tvec.size())])
