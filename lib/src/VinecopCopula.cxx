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
#include "otvine/VinecopCopula.hxx"
#include <openturns/PersistentObjectFactory.hxx>

#include <vinecopulib.hpp>

using namespace OT;

namespace OTVINE
{

CLASSNAMEINIT(VinecopCopula);

static Factory<VinecopCopula> Factory_VinecopCopula;


/* Default constructor */
VinecopCopula::VinecopCopula()
  : DistributionImplementation()
{
  isCopula_ = true;
  setName("VinecopCopula");
}

VinecopCopula::VinecopCopula(const Pointer<vinecopulib::Vinecop> & p_vinecop)
  : DistributionImplementation()
  , p_vinecop_(p_vinecop)
{
  isCopula_ = true;
  setName("VinecopCopula");
  setDimension(p_vinecop->get_dim());
}

/* Virtual constructor method */
VinecopCopula * VinecopCopula::clone() const
{
  return new VinecopCopula(*this);
}

/* Get the PDF of the distribution */
Scalar VinecopCopula::computePDF(const Point & point) const
{
  const UnsignedInteger dimension = getDimension();
  Eigen::MatrixXd u(1, dimension);
    for (UnsignedInteger j = 0; j < dimension; ++ j)
      u(0, j) = point[j];
  return p_vinecop_->pdf(u)[0];
}

/* Get the CDF of the distribution */
Scalar VinecopCopula::computeCDF(const Point & point) const
{
  const UnsignedInteger dimension = getDimension();
  Eigen::MatrixXd u(1, dimension);
  for (UnsignedInteger j = 0; j < dimension; ++ j)
    u(0, j) = point[j];
  return p_vinecop_->cdf(u)[0];
}

/* Get one realization of the distribution */
Point VinecopCopula::getRealization() const
{
  const UnsignedInteger dimension = getDimension();
  const Eigen::MatrixXd u(p_vinecop_->simulate(1));
  Point result(dimension);
  for (UnsignedInteger j = 0; j < dimension; ++ j)
    result[j] = u(0, j);
  return result;
}

Sample VinecopCopula::getSample(const UnsignedInteger size) const
{
  const UnsignedInteger dimension = getDimension();
  const Eigen::MatrixXd u(p_vinecop_->simulate(size));
  Sample result(size, dimension);
  for (UnsignedInteger i = 0; i < size; ++ i)
    for (UnsignedInteger j = 0; j < dimension; ++ j)
      result(i, j) = u(i, j);
  return result;
}

/* Parameters value accessor */
Point VinecopCopula::getParameter() const
{
  return {};
}

void VinecopCopula::setParameter(const Point & parameter)
{
  if (parameter.getSize() != 0)
    throw InvalidArgumentException(HERE) << "Error: expected 0 value, got " << parameter.getSize();
}

/* Parameters description accessor */
Description VinecopCopula::getParameterDescription() const
{
  return {};
}

/* Comparison operator */
Bool VinecopCopula::operator ==(const VinecopCopula & other) const
{
  if (this == &other) return true;
  return false;
}

Bool VinecopCopula::equals(const DistributionImplementation & other) const
{
  const VinecopCopula* p_other = dynamic_cast<const VinecopCopula*>(&other);
  return p_other && (*this == *p_other);
}

/* String converter */
String VinecopCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << VinecopCopula::GetClassName() << " " << p_vinecop_->str();
  return oss;
}

/* Method save() stores the object through the StorageManager */
void VinecopCopula::save(Advocate & adv) const
{
  DistributionImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void VinecopCopula::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
}


} /* namespace OTVINE */
