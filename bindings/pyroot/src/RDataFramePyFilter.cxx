#include "RDataFramePyFilter.h"

#include "RootWrapper.h"
#include "Cppyy.h"

ROOT::RDF::RNode PyROOT::RDataFramePyFilter(ROOT::RDF::RNode df, PyObject *f, std::vector<std::string> &columns, std::vector<std::string>& types,
                                    std::string pytype, std::string name)
{
   // TODO: Sanitize number of arguments for callable (segfault otherwise)

   // TODO: How to sanitize the call? How to forward the Python interpreter error?

   // JIT lambda with correct signature
   auto l = [f](std::vector<float>& x) {
      PyObject *pyret = PyObject_CallFunction((PyObject*)f, "O",
              (PyObject*)PyROOT::BindCppObject(&x, Cppyy::GetScope("std::vector<float>")));
      bool ret = PyObject_IsTrue(pyret);
      Py_XDECREF(pyret);
      return ret;
   };

   foo

   return df.Filter(l, columns, name);
}
