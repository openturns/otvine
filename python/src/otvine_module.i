// SWIG file otvine_module.i

%module(docstring="otvine module") otvine

%{
#include <openturns/OT.hxx>
#include <openturns/PythonWrappingFunctions.hxx>
%}

// Prerequisites needed
%include typemaps.i
%include exception.i
%ignore *::load(OT::Advocate & adv);
%ignore *::save(OT::Advocate & adv) const;

%import base_module.i
%import uncertainty_module.i

// The new classes
%include otvine/otvineprivate.hxx
%include RotatedCopula.i
%include TawnCopula.i
%include VinecopCopula.i
%include VineCopula.i
%include VineCopulaFactory.i
