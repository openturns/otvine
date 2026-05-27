//                                               -*- C++ -*-
/**
 *  @brief VineCopulaFactory
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
#ifndef OTVINE_VINECOPULAFACTORY_HXX
#define OTVINE_VINECOPULAFACTORY_HXX

#include <openturns/DistributionFactoryImplementation.hxx>

#include "otvine/otvineprivate.hxx"

namespace OTVINE
{

/**
 * @class VineCopulaFactory
 *
 * VineCopulaFactory is some vinecopulafactory type to illustrate how to add some classes in OpenTURNS
 */
class OTVINE_API VineCopulaFactory
  : public OT::DistributionFactoryImplementation
{
  CLASSNAME

public:
  /** Default constructor */
  VineCopulaFactory();

  /** Virtual constructor method */
  VineCopulaFactory * clone() const override;

  /** example of a func that return a point squared. **/
  OT::Distribution build(const OT::Sample & sample) const override;

  /** String converter */
  OT::String __repr__() const override;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const override;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv) override;

  void setNative(const OT::Bool native);
  void setAllowRotations(const OT::Bool allowRotations);
private:

  OT::Bool native_ = false;
  OT::Bool allowRotations_ = true;
}; /* class VineCopulaFactory */

} /* namespace OTVINE */

#endif /* OTVINE_VINECOPULAFACTORY_HXX */
