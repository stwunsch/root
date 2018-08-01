import unittest
import ROOT
import numpy as np


class RTensor(unittest.TestCase):
    def test_getitem(self):
        dtype = "float"
        data = ROOT.std.vector(dtype)(6)
        for i in range(data.size()):
            data[i] = i
        shape = ROOT.std.vector("size_t")(2)
        shape[0] = 2
        shape[1] = 3
        x = ROOT.TMVA.Experimental.RTensor(dtype)(data.data(), shape)
        count = 0
        for i in range(shape[0]):
            for j in range(shape[1]):
                self.assertEqual(count, x[i, j])
                count += 1

    def test_setitem(self):
        dtype = "float"
        shape = ROOT.std.vector("size_t")(2)
        shape[0] = 2
        shape[1] = 3
        x = ROOT.TMVA.Experimental.RTensor(dtype)(shape)
        count = 0
        for i in range(shape[0]):
            for j in range(shape[1]):
                x[i, j] = count
                count += 1
        count = 0
        for i in range(shape[0]):
            for j in range(shape[1]):
                self.assertEqual(count, x[i, j])
                count += 1


if __name__ == '__main__':
    unittest.main()
