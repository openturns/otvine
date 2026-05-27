// SWIG file VineCopula.i

%{
#include "otvine/VineCopula.hxx"
#include "openturns/PythonWrappingFunctions.hxx"

namespace OT {
  template <>
  struct traitsPythonType< OT::Distribution >
  {
    typedef _PyObject_ Type;
  };

  template <>
  inline
  bool
  canConvert< _PyObject_, OT::Distribution >(PyObject * pyObj)
  {
    void * ptr = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr(pyObj, &ptr, SWIG_TypeQuery("OT::Distribution *"), 0))) {
      OT::Distribution * p_mf = reinterpret_cast< OT::Distribution * >(ptr);
      return p_mf != NULL;
    } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIGTYPE_p_OT__DistributionImplementation, 0))) {
      OT::DistributionImplementation * p_impl = reinterpret_cast< OT::DistributionImplementation * >(ptr);
      return p_impl != NULL;
    } else if (SWIG_IsOK(SWIG_ConvertPtr( pyObj, &ptr, SWIG_TypeQuery("OT::Pointer<OT::DistributionImplementation> *"), 0))) {
      OT::Pointer<OT::DistributionImplementation> * p_impl = reinterpret_cast< OT::Pointer<OT::DistributionImplementation> * >(ptr);
      return p_impl != NULL && !p_impl->isNull();
    }
    return false;
  }

  template <>
  inline
  OT::Distribution
  convert<_PyObject_, OT::Distribution>(PyObject * pyObj)
  {
    void * ptr = 0;
    if (SWIG_IsOK(SWIG_ConvertPtr(pyObj, &ptr, SWIG_TypeQuery("OT::Distribution *"), 0)))
    {
      OT::Distribution * p_ki = reinterpret_cast< OT::Distribution * >(ptr);
      return Distribution(*p_ki);
    }
    else if (SWIG_IsOK(SWIG_ConvertPtr(pyObj, &ptr, SWIG_TypeQuery("OT::DistributionImplementation *"), 0)))
    {
      OT::DistributionImplementation * p_ki = reinterpret_cast< OT::DistributionImplementation * >(ptr);
      return *p_ki;
    }
    throw OT::InvalidArgumentException(HERE) << "Object passed as argument is not an object convertible to an DistributionImplementation";
  }

} // namespace OT

%}

%include VineCopula_doc.i

%copyctor OTVINE::VineCopula;

%include otvine/VineCopula.hxx
