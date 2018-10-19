// Author: Danilo Piparo, Enrico Guiraud, Stefan Wunsch CERN  04/2018

/*************************************************************************
 * Copyright (C) 1995-2016, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_RDataFramePyFilter
#define ROOT_RDataFramePyFilter

#include <Python.h>

#include "ROOT/RDataFrame.hxx"
#include "ROOT/RDF/RInterface.hxx"
#include <iostream>
#include <vector>
#include <string>

namespace PyROOT {

template <typename T>
ROOT::RDF::RNode PassAsRNode(T df)
{
   return df;
}

ROOT::RDF::RNode RDataFramePyFilter(ROOT::RDF::RNode df, PyObject *f, std::vector<std::string> &columns,
                                    std::vector<std::string> &types, std::string pytype, std::string name);
}

#endif
