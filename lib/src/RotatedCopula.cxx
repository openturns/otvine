//                                               -*- C++ -*-
/**
 *  @brief RotatedCopula
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
#include "otvine/RotatedCopula.hxx"
#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/SpecFunc.hxx>

#include <vinecopulib.hpp>

using namespace OT;

namespace OTVINE
{

CLASSNAMEINIT(RotatedCopula)

static Factory<RotatedCopula> Factory_RotatedCopula;


/* Default constructor */
RotatedCopula::RotatedCopula()
  : DistributionImplementation()
{
  isCopula_ = true;
  setName("RotatedCopula");
}

RotatedCopula::RotatedCopula(const OT::Distribution & copula, const SignedInteger rotation)
  : DistributionImplementation()
  , copula_(copula)
  , rotation_(rotation % 360)
{
  isCopula_ = true;
  setName("RotatedCopula");
  setDimension(copula.getDimension());
  setDescription(copula.getDescription());
  computeRange();
  if (!copula.isCopula())
      throw InvalidArgumentException(HERE) << "RotatedCopula distribution is not a copula";
  if (copula.getDimension() != 2)
    throw NotYetImplementedException(HERE) << "RotatedCopula only supports dimension 2";
  if (rotation % 90 != 0)
    throw NotYetImplementedException(HERE) << "RotatedCopula invalid rotation";
}

/* Virtual constructor method */
RotatedCopula * RotatedCopula::clone() const
{
  return new RotatedCopula(*this);
}

Point RotatedCopula::rotate(const Point & x, const Bool inverse) const
{
  if (x.getDimension() != 2)
    throw NotYetImplementedException(HERE) << "RotatedCopula only supports dimension 2";
  const Scalar u1 = x[0];
  const Scalar u2 = x[1];
  UnsignedInteger rotation = rotation_;
  if (inverse)
  {
    switch (rotation)
    {
      case (90):
        rotation = 270;
        break;
      case (270):
        rotation = 90;
        break;
      default:
        break;
    }
  }
  switch (rotation)
  {
    case (0):
      return {u1, u2};
    case (90):
      return {u2, 1.0 - u1};
    case (180):
      return {1.0 - u1, 1.0 - u2};
    case (270):
      return {1.0 - u2, u1};
    default:
      throw NotYetImplementedException(HERE) << "RotatedCopula invalid rotation";
  }
}

/* Get the PDF of the distribution */
Scalar RotatedCopula::computePDF(const Point & point) const
{
  return copula_.computePDF(rotate(point));
}

/* Get the DDF of the distribution */
Point RotatedCopula::computeDDF(const Point & point) const
{
  Point ddf0(copula_.computeDDF(rotate(point)));
  switch (rotation_)
  {
    case (0):
      return ddf0;
    case (90):
      return {-ddf0[1], ddf0[0]};
    case (180):
      return {-ddf0[0], -ddf0[1]};
    case (270):
      return {ddf0[1], -ddf0[0]};
    default:
      throw NotYetImplementedException(HERE) << "RotatedCopula invalid rotation";
  }
}

Point RotatedCopula::computePDFGradient(const Point & point) const
{
  return copula_.computePDFGradient(rotate(point));
}

Point RotatedCopula::computeCDFGradient(const Point & point) const
{
  Point gradient0(copula_.computeCDFGradient(rotate(point)));
  switch (rotation_)
  {
    case (0):
      return gradient0;
    case (90):
      return -gradient0;
    case (180):
      return gradient0;
    case (270):
      return -gradient0;
    default:
      throw NotYetImplementedException(HERE) << "RotatedCopula invalid rotation";
  }
}

/* Get the CDF of the distribution */
Scalar RotatedCopula::computeCDF(const Point & point) const
{
  // https://quantitative-thinking.com/copula_theory/cop_modifications/clockwise_rot.html
  // https://vinecopulas.readthedocs.io/en/latest/bivariatecopulas.html#Rotations
  const Scalar cdf0 = copula_.computeCDF(rotate(point));
  const Scalar u0 = SpecFunc::Clip01(point[0]);
  const Scalar u1 = SpecFunc::Clip01(point[1]);
  switch (rotation_)
  {
    case (0):
      return cdf0;
    case (90):
      return u1 - cdf0;
    case (180):
      return u0 + u1 - 1.0 + cdf0;
    case (270):
      return u0 - cdf0;
    default:
      throw NotYetImplementedException(HERE) << "RotatedCopula invalid rotation";
  }
}

/* Get one realization of the distribution */
Point RotatedCopula::getRealization() const
{
  return rotate(copula_.getRealization(), true);
}

/* Parameters value accessor */
Point RotatedCopula::getParameter() const
{
  return copula_.getParameter();
}

void RotatedCopula::setParameter(const Point & parameter)
{
  copula_.setParameter(parameter);
}

/* Parameters description accessor */
Description RotatedCopula::getParameterDescription() const
{
  return copula_.getParameterDescription();
}

/* Comparison operator */
Bool RotatedCopula::operator ==(const RotatedCopula & other) const
{
  if (this == &other) return true;
  return (copula_ == other.copula_) && (rotation_ == other.rotation_);
}

Bool RotatedCopula::equals(const DistributionImplementation & other) const
{
  const RotatedCopula* p_other = dynamic_cast<const RotatedCopula*>(&other);
  return p_other && (*this == *p_other);
}

/* String converter */
String RotatedCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << RotatedCopula::GetClassName() << " copula=" << copula_ << " rotation=" << rotation_;
  return oss;
}

String RotatedCopula::__str__(const String &) const
{
  OSS oss;
  oss << "RotatedCopula(copula=" << copula_.__str__() << ", rotation=" << rotation_ << ")";
  return oss;
}

Bool RotatedCopula::isContinuous() const
{
  return copula_.isContinuous();
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
Scalar RotatedCopula::computeConditionalCDF(const Scalar x, const Point & y) const
{
  if (rotation_ == 0)
    return copula_.computeConditionalCDF(x, y);
  else
    return DistributionImplementation::computeConditionalCDF(x, y);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
Scalar RotatedCopula::computeConditionalQuantile(const Scalar q, const Point & y) const
{
  if (rotation_ == 0)
    return copula_.computeConditionalQuantile(q, y);
  else
    return DistributionImplementation::computeConditionalQuantile(q, y);
}

OT::UnsignedInteger RotatedCopula::getRotation() const
{
  return rotation_;
}

OT::Distribution RotatedCopula::getCopula() const
{
  return copula_;
}

/* Method save() stores the object through the StorageManager */
void RotatedCopula::save(Advocate & adv) const
{
  DistributionImplementation::save(adv);
  adv.saveAttribute("copula_", copula_);
  adv.saveAttribute("rotation_", rotation_);
}

/* Method load() reloads the object from the StorageManager */
void RotatedCopula::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
  adv.loadAttribute("copula_", copula_);
  adv.loadAttribute("rotation_", rotation_);
  computeRange();
}


} /* namespace OTVINE */
