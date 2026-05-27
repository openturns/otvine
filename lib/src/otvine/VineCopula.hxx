//                                               -*- C++ -*-
/**
 *  @brief VineCopula
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
#ifndef OTVINE_VINECOPULA_HXX
#define OTVINE_VINECOPULA_HXX

#include <openturns/DistributionImplementation.hxx>
#include <openturns/Distribution.hxx>

#include "otvine/otvineprivate.hxx"

namespace OTVINE
{

/**
 * @class VineCopula
 *
 * VineCopula is a vine copula built from a R-Vine matrix using a simplified syntax.
 * Use addArc() to add pair-copulas tree by tree.
 */
class OTVINE_API VineCopula
  : public OT::DistributionImplementation
{
  CLASSNAME

public:
  /** Default constructor */
  VineCopula();
  explicit VineCopula(const OT::UnsignedInteger dimension);
  VineCopula(const OT::Matrix & matrix,
             const OT::Collection<OT::Distribution> & copulaCollection);

  /** Virtual constructor method */
  VineCopula * clone() const override;

  OT::Bool operator ==(const VineCopula & other) const;
protected:
  OT::Bool equals(const DistributionImplementation & other) const override;
public:

  /** String converter */
  OT::String __repr__() const override;
  OT::String __str__(const OT::String & offset = "") const override;

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

  /** Add a pair-copula to the vine */
  void addArc(const OT::Indices & indices, const OT::Distribution & distribution);

  /** Accessor to the R-vine matrix */
  OT::Matrix getMatrix() const;

  /** Accessor to the pair-copula collection */
  OT::PersistentCollection<OT::Distribution> getCopulaCollection() const;

private:
  void buildMatrix();

  OT::Matrix matrix_;
  OT::PersistentCollection<OT::Distribution> copulaCollection_;
  std::vector<OT::Indices> arcBuffer_;

}; /* class VineCopula */

} /* namespace OTVINE */

#endif /* OTVINE_VINECOPULA_HXX */
