//                                               -*- C++ -*-
/**
 *  @brief VinecopCopula
 *
 *  Copyright 2005-2026 Airbus-EDF-IMACS-ONERA-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTVINE_VINECOPCOPULA_HXX
#define OTVINE_VINECOPCOPULA_HXX

#include <openturns/DistributionImplementation.hxx>

#include "otvine/otvineprivate.hxx"

namespace vinecopulib {
class Vinecop;
}

namespace OTVINE
{

/**
 * @class VinecopCopula
 *
 * VinecopCopula is some vinecopulafactory type to illustrate how to add some classes in OpenTURNS
 */
class OTVINE_API VinecopCopula
  : public OT::DistributionImplementation
{
  CLASSNAME

public:
  /** Default constructor */
  VinecopCopula();

  explicit VinecopCopula(const OT::Pointer<vinecopulib::Vinecop> & vinecop);

  /** Virtual constructor method */
  VinecopCopula * clone() const override;

  OT::Bool operator ==(const VinecopCopula & other) const;
protected:
  OT::Bool equals(const DistributionImplementation & other) const override;
public:

  /** String converter */
  OT::String __repr__() const override;

  /** Get one realization of the distribution */
  OT::Point getRealization() const override;
  OT::Sample getSample(const OT::UnsignedInteger size) const override;

  /** Get the PDF of the distribution */
  using OT::DistributionImplementation::computePDF;
  OT::Scalar computePDF(const OT::Point & point) const override;

  /** Get the CDF of the distribution */
  using OT::DistributionImplementation::computeCDF;
  OT::Scalar computeCDF(const OT::Point & point) const override;
  
  /** Parameters value accessors */
  void setParameter(const OT::Point & parameter) override;
  OT::Point getParameter() const override;

  /** Parameters description accessor */
  OT::Description getParameterDescription() const override;
  
  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const override;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv) override;

private:
  OT::Pointer<vinecopulib::Vinecop> p_vinecop_;

}; /* class VinecopCopula */

} /* namespace OTVINE */

#endif /* OTVINE_VINECOPCOPULA_HXX */
