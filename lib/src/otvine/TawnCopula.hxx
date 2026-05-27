//                                               -*- C++ -*-
/**
 *  @brief The TawnCopula distribution
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
#ifndef OTVINE_TAWNCOPULA_HXX
#define OTVINE_TAWNCOPULA_HXX

#include <openturns/ExtremeValueCopula.hxx>

#include "otvine/otvineprivate.hxx"

namespace OTVINE
{

/**
 * @class TawnCopula
 *
 * The TawnCopula distribution (asymmetric logistic model).
 */
class OTVINE_API TawnCopula
  : public OT::ExtremeValueCopula
{
  CLASSNAME
public:

  /** Default constructor */
  TawnCopula();

  /** Parameters constructor */
  explicit TawnCopula(const OT::Scalar theta,
                      const OT::Scalar psi1 = 0.5,
                      const OT::Scalar psi2 = 0.5);

  /** Comparison operator */
  using OT::ExtremeValueCopula::operator ==;
  OT::Bool operator ==(const TawnCopula & other) const;

  /** String converter */
  OT::String __repr__() const override;
  OT::String __str__(const OT::String & offset = "") const override;

  /* Interface inherited from Distribution */
  /** Virtual constructor */
  TawnCopula * clone() const override;

  /** Parameters value accessors */
  void setParameter(const OT::Point & parameter) override;
  OT::Point getParameter() const override;

  /** Parameters description accessor */
  OT::Description getParameterDescription() const override;

  /** Mutator of theta */
  void setTheta(const OT::Scalar theta);

  /** Accessor to theta */
  OT::Scalar getTheta() const;

  /** Mutator of psi1 */
  void setPsi1(const OT::Scalar psi1);

  /** Accessor to psi1 */
  OT::Scalar getPsi1() const;

  /** Mutator of psi2 */
  void setPsi2(const OT::Scalar psi2);

  /** Accessor to psi2 */
  OT::Scalar getPsi2() const;

  /** Method save() stores the object through the StorageManager */
  void save(OT::Advocate & adv) const override;

  /** Method load() reloads the object from the StorageManager */
  void load(OT::Advocate & adv) override;

protected:


private:
  // update function when parameters are modified
  void updatePickandFunction();

  /** Tawn Copula parameters */
  OT::Scalar theta_ = 0.0;
  OT::Scalar psi1_ = 0.0;
  OT::Scalar psi2_ = 0.0;

}; /* class TawnCopula */


} /* namespace OTVINE */

#endif /* OTVINE_TAWNCOPULA_HXX */
