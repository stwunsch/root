// Author: Danilo Piparo, Enrico Guiraud, Stefan Wunsch CERN  04/2018

/*************************************************************************
 * Copyright (C) 1995-2016, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDataFramePyFilter
#define ROOT_TDataFramePyFilter

#include "ROOT/TDataFrame.hxx"
#include "Python.h"
#include <iostream>
#include <vector>
#include <string>

namespace PyROOT {

using namespace ROOT::Experimental;

inline ULong64_t GetAddress(TDataFrame &p)
{
   return reinterpret_cast<ULong64_t>(&p);
}

ROOT::Experimental::TDF::TInterface<ROOT::Detail::TDF::TJittedFilter> *ConvertTDataFramePyFilter(ULong64_t p)
{
   return reinterpret_cast<ROOT::Experimental::TDF::TInterface<ROOT::Detail::TDF::TJittedFilter> *>(p);
}

TDF::TInterface<ROOT::Detail::TDF::TJittedFilter> TDataFramePyFilter(TDataFrame &df, PyObject *f, bool lockGil)
{
   auto l = [f, lockGil](float x) {
      PyGILState_STATE gstate;
      if(lockGil) gstate = PyGILState_Ensure();
      PyObject *pyret = PyObject_CallFunction(f, (char *)"f", x);
      bool ret = PyObject_IsTrue(pyret);
      Py_XDECREF(pyret);
      if(lockGil) PyGILState_Release(gstate);
      return ret;
   };

   return df.Filter(l, {"b1"}).Filter("true");
}
}

#endif
