// Author: Stefan Wunsch, Enric Tejedor CERN  06/2018
// Original PyROOT code by Wim Lavrijsen, LBL

/*************************************************************************
 * Copyright (C) 1995-2018, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "Python.h"

#include "CPyCppyy.h"
#include "PyROOTPythonize.h"
#include "CPPInstance.h"
#include "Utility.h"
#include "TInterpreter.h"
#include "TInterpreterValue.h"

#include <sstream>

using namespace CPyCppyy;

std::string GetCppName(CPPInstance *self)
{
   return Cppyy::GetScopedFinalName(self->ObjectIsA());
}

PyObject *ClingPrintValue(CPPInstance *self)
{
   const std::string className = GetCppName(self);
   auto printResult = gInterpreter->ToString(className.c_str(), self->GetObject());
   if (printResult.find("@0x") == 0) {
      // Fall back to __repr__ if we just get an address from cling
      auto method = PyObject_GetAttrString((PyObject*)self, "__repr__");
      auto res = PyObject_CallObject(method, nullptr);
      Py_DECREF(method);
      return res;
   } else {
      return CPyCppyy_PyUnicode_FromString(printResult.c_str());
   }
}

////////////////////////////////////////////////////////////////////////////
/// \brief Add pretty printing pythonization
/// \param[in] self Always null, since this is a module function.
/// \param[in] args Pointer to a Python tuple object containing the arguments
/// received from Python.
///
/// This function adds the following pythonizations to print the object more
/// user-friendly than cppyy by using the output of cling::printValue as the
/// return value of the special method __str__.
PyObject *PyROOT::AddPrettyPrintingPyz(PyObject * /* self */, PyObject *args)
{
   PyObject *pyclass = PyTuple_GetItem(args, 0);
   Utility::AddToClass(pyclass, "__str__", (PyCFunction)ClingPrintValue);
   Py_RETURN_NONE;
}


// Init of class used as decorator to create C++ wrapper using numba
// The init parses the arguments passed to the decorator.
PyObject* NumbaCallableImpl_init(PyObject * /*self*/, PyObject *args)
{
   // Parse arguments and extract input and output types and an optional verbose flag
   if (!PyTuple_Check(args)) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to parse arguments: Invalid tuple.");
      return NULL;
   }

   if (!(PyTuple_Size(args) == 3)) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to parse arguments: Expect exactly two arguments (list of input types, return type).");
      return NULL;
   }

   auto instance = PyTuple_GetItem(args, 0);
   auto inputTypes = PyTuple_GetItem(args, 1);
   auto returnType = PyTuple_GetItem(args, 2);

   // Attach arguments to instance
   PyObject_SetAttrString(instance, "input_types", inputTypes);
   PyObject_SetAttrString(instance, "return_type", returnType);

   Py_RETURN_NONE;
}


// Call method of class used as decorator to create C++ wrapper using numba
// The call method creates the C++ wrapper class for the Python callable and
// passes through the actual callable.
PyObject* NumbaCallableImpl_call(PyObject * /*self*/, PyObject *args)
{
   // Parse arguments with Python callable and get attributes with types
   if (!PyTuple_Check(args)) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to parse arguments: Invalid tuple.");
      return NULL;
   }

   if (!(PyTuple_Size(args) == 2)) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to parse arguments: Expect exactly one argument (Python callable).");
      return NULL;
   }

   if (!(PyTuple_Size(args) == 2)) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to parse arguments: Expect exactly one argument (Python callable).");
      return NULL;
   }

   auto instance = PyTuple_GetItem(args, 0);
   auto pyfunc = PyTuple_GetItem(args, 1);
   Py_INCREF(pyfunc);

   if (!PyObject_HasAttrString(instance, "input_types")) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callble: No input_types attribute found.");
      return NULL;
   }

   if (!PyObject_HasAttrString(instance, "return_type")) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callble: No return_type attribute found.");
      return NULL;
   }

   auto inputTypes = PyObject_GetAttrString(instance, "input_types");
   auto returnType = PyObject_GetAttrString(instance, "return_type");

   // Extract name of Python callable
   if (!PyObject_HasAttrString(pyfunc, "__name__")) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callble: Python callable does not have attribute __name__.");
      return NULL;
   }

   auto pyname = PyObject_GetAttrString(pyfunc, "__name__");
   auto name = CPyCppyy_PyUnicode_AsString(pyname);
   Py_DECREF(pyname);

   // Get C++ return type
   if (!CPyCppyy_PyUnicode_Check(returnType)) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callble: Return type argument cannot be interpreted as string.");
      return NULL;
   }
   std::string returnTypeStr = CPyCppyy_PyUnicode_AsString(returnType);
   if (returnTypeStr.compare("") == 0) {
      returnTypeStr = "void";
   }

   // Find numba types for C++ types
   std::map<std::string, std::string> typemap = {
       {"float", "float32"},
       {"double", "double64"},
       {"int", "int32"},
       {"long", "int64"},
   };

   auto iter = PyObject_GetIter(inputTypes);
   if (!iter) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callable: Failed to iterate over input types.");
      return NULL;
   }

   PyObject *item;
   auto idx = 0u;
   auto inputTypesSize = PyObject_Size(inputTypes);
   std::vector<std::string> numbaTypes(inputTypesSize);
   std::vector<std::string> cppTypes(inputTypesSize);
   while ((item = PyIter_Next(iter))) {
      // Convert argument to string
      if (!CPyCppyy_PyUnicode_Check(item)) {
         PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callable: Failed to interpret input type as string.");
         return NULL;
      }

      cppTypes[idx] = CPyCppyy_PyUnicode_AsString(item);
      Py_DECREF(item);

      auto t = typemap.find(cppTypes[idx]);
      if (t != typemap.end()) {
         numbaTypes[idx] = t->second;
      } else {
         PyErr_SetString(PyExc_RuntimeError, "Failed to create C++ callable: Input type is not valid for jitting with numba.");
         return NULL;
      }

      idx++;
   }
   Py_DECREF(iter);

   // Import numba
   auto numba = PyImport_ImportModule("numba");
   if (!numba) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to import numba.");
      return NULL;
   }

   // Get cfunc method
   auto cfunc = PyObject_GetAttrString(numba, "cfunc");
   if (!cfunc) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to import cfunc from numba.");
      return NULL;
   }

   // Jit Python callable
   std::stringstream numbaSignature;
   numbaSignature << typemap[returnTypeStr] << "(";
   for(auto i = 0; i < inputTypesSize; i++) {
      numbaSignature << numbaTypes[i];
      if (i != inputTypesSize - 1) {
         numbaSignature << ", ";
      } else {
         numbaSignature << ")";
      }
   }
   auto numbaSignatureStr = numbaSignature.str();
   auto args_ = Py_BuildValue("(s)", (char*)numbaSignatureStr.c_str());
   auto kwargs_ = Py_BuildValue("{s:O}", (char*)"nopython", Py_True);
   auto decorator = PyObject_Call(cfunc, args_, kwargs_);
   if (!decorator) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to create instance of numba.cfunc.");
      return NULL;
   }
   Py_DECREF(args_);
   Py_DECREF(kwargs_);

   auto jitted = PyObject_CallFunction(decorator, (char*)"O", pyfunc);
   if (!jitted) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to jit function with numba.cfunc.");
      return NULL;
   }

   // Attach jitted function to callable
   PyObject_SetAttrString(pyfunc, "__numba_cfunc__", jitted);

   // Extract function pointer
   auto pyaddress = PyObject_GetAttrString(jitted, "address");
   if (!pyaddress) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to extract function pointer from numba.cfunc.");
      return NULL;
   }
   auto address = PyLong_AsUnsignedLongLong(pyaddress);

   // Put wrapper function in ROOT namespace
   std::stringstream code;
   code << "namespace ROOT {\n";

   // Set return type of wrapper functoin
   code << returnTypeStr << " ";

   // Set name of Python callable as function name
   code << name;

   // Build function signature, function pointer cast and variable list
   code << "(";
   std::stringstream vars;
   std::stringstream fPtr;
   fPtr << returnTypeStr << "(*)(";
   for(auto i = 0; i < inputTypesSize; i++) {
      code << cppTypes[i] << " x_" << i;
      vars << "x_" << i;
      fPtr << cppTypes[i];
      if (i != inputTypesSize - 1) {
         code << ", ";
         vars << ", ";
         fPtr << ", ";
      } else {
         code << ") {\n";
         fPtr << ")";
      }
   }

   // Protect multi-threaded scenario
   //code << "   R__WRITE_LOCKGUARD(ROOT::gCoreMutex);\n";

   // Cast int to C function pointer
   code << "   auto funcptr = reinterpret_cast<" << fPtr.str() << ">(" << address << ");\n";

   // Return result
   code << "   return funcptr(" << vars.str() << ");\n";

   // Close function and namespace
   code << "}\n}";

   // Jit C++ wrapper
   auto code_str = code.str();
   auto code_cstr = code_str.c_str();

   auto err = gInterpreter->Declare("#include \"Python.h\"");
   if (!err) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to compile C++ wrapper: Failed to include Python.h.");
      return NULL;
   }

   err = gInterpreter->Declare("#include \"TPython.h\"");
   if (!err) {
      PyErr_SetString(PyExc_RuntimeError, "Failed to compile C++ wrapper: Failed to include TPython.h.");
      return NULL;
   }

   err = gInterpreter->Declare(code_cstr);
   if (!err) {
      PyErr_SetString(PyExc_RuntimeError,
              ("Failed to compile C++ wrapper: Compilation error from following wrapper code.\n" + code.str()).c_str());
      return NULL;
   }

   // Attach code function to callable
   auto pycode = CPyCppyy_PyUnicode_FromString(code_cstr);
   PyObject_SetAttrString(pyfunc, "__cpp_wrapper__", pycode);
   Py_DECREF(pycode);

   // Clean-up and ass through Python callable
   Py_DECREF(jitted);
   Py_DECREF(decorator);
   Py_DECREF(cfunc);
   Py_DECREF(numba);
   Py_DECREF(inputTypes);
   Py_DECREF(returnType);
   return pyfunc;
}


// Method definition for class used as decorator to create C++ wrapper
static PyMethodDef NumbaCallableImplMethods[] =
{
    {"__init__", NumbaCallableImpl_init, METH_VARARGS, "Parse decorator arguments"},
    {"__call__", NumbaCallableImpl_call, METH_VARARGS, "Create C++ wrapper function"},
    {NULL},
};


// Proxy to return the C++ wrapper class which can be used as decorator
PyObject *PyROOT::NumbaCallableClass(PyObject * /*self*/, PyObject * /*args*/) {
   // Create wrapper class for decorator
   auto classDict = PyDict_New();
   auto className = PyString_FromString("NumbaCallableImpl");
   auto numbaCallableClass = PyClass_New(NULL, classDict, className);

   // Add methods
   for (auto def = NumbaCallableImplMethods; def->ml_name != NULL; def++) {
      PyObject *func = PyCFunction_New(def, NULL);
      PyObject *method = PyMethod_New(func, NULL, numbaCallableClass);
	  PyDict_SetItemString(classDict, def->ml_name, method);
	  Py_DECREF(func);
	  Py_DECREF(method);
   }

   // Clean-up and return implementation class
   Py_DECREF(classDict);
   Py_DECREF(className);
   return numbaCallableClass;
}
