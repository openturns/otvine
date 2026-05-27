//                                               -*- C++
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
#include <cmath>
#include "otvine/TawnCopula.hxx"
#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/Exception.hxx>
#include <openturns/SymbolicFunction.hxx>

using namespace OT;

namespace OTVINE
{

CLASSNAMEINIT(TawnCopula)

static Factory<TawnCopula> Factory_TawnCopula;

/* Default constructor */
TawnCopula::TawnCopula()
  : ExtremeValueCopula()
  , theta_(2.0)
  , psi1_(0.5)
  , psi2_(0.5)
{
  setName("TawnCopula");
  updatePickandFunction();
}

/* Parameters constructor */
TawnCopula::TawnCopula(const Scalar theta,
                       const Scalar psi1,
                       const Scalar psi2)
  : ExtremeValueCopula()
{
  setTheta(theta);
  setPsi1(psi1);
  setPsi2(psi2);

  setName("TawnCopula");
  updatePickandFunction();
}

/* update function wrt parameters */
void TawnCopula::updatePickandFunction()
{
  // A(t) = (1-psi1)*(1-t) + (1-psi2)*t + ((psi2*t)^theta + (psi1*(1-t))^theta)^(1/theta)
  // use the vinecopulib convention which switches the psi1/psi2 roles wrt the litterature
  const String formula(OSS() << "(1-" << psi1_ << ")*(1-t)+(1-" << psi2_ << ")*t+((" << psi2_
                 << "*t)^" << theta_ << "+(" << psi1_ << "*(1-t))^" << theta_ << ")^(1.0/" << theta_ << ")");
  ExtremeValueCopula::setPickandFunction(SymbolicFunction(Description(1, "t"), Description(1, formula)), false);
  computeRange();
}


/* Comparison operator */
Bool TawnCopula::operator ==(const TawnCopula & other) const
{
  if (this == &other) return true;
  return (theta_ == other.theta_) && (psi1_ == other.psi1_) && (psi2_ == other.psi2_);
}

/* String converter */
String TawnCopula::__repr__() const
{
  OSS oss(true);
  oss << "class=" << TawnCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " theta=" << theta_
      << " psi1=" << psi1_
      << " psi2=" << psi2_;
  return oss;
}

String TawnCopula::__str__(const String & ) const
{
  OSS oss(false);
  oss << getClassName() << "(theta = " << theta_ << ", psi1 = " << psi1_ << ", psi2 = " << psi2_ << ")";
  return oss;
}

/* Virtual constructor */
TawnCopula * TawnCopula::clone() const
{
  return new TawnCopula(*this);
}

/* Parameters value accessor */
Point TawnCopula::getParameter() const
{
  Point result;
  result.add(theta_);
  result.add(psi1_);
  result.add(psi2_);
  return result;
}

void TawnCopula::setParameter(const Point & parameter)
{
  if (parameter.getSize() != 3) throw InvalidArgumentException(HERE) << "Error: expected 3 values, got " << parameter.getSize();
  const Scalar w = getWeight();
  *this = TawnCopula(parameter[0], parameter[1], parameter[2]);
  setWeight(w);
}

/* Parameters description accessor */
Description TawnCopula::getParameterDescription() const
{
  Description result;
  result.add("theta");
  result.add("psi1");
  result.add("psi2");
  return result;
}

/* theta parameter accessor */
Scalar TawnCopula::getTheta() const
{
  return theta_;
}

/* psi1 parameter accessor */
Scalar TawnCopula::getPsi1() const
{
  return psi1_;
}

/* psi2 parameter accessor */
Scalar TawnCopula::getPsi2() const
{
  return psi2_;
}

/* theta parameter accessor */
void TawnCopula::setTheta(const Scalar theta)
{
  if (!(theta >= 1.0))
    throw InvalidArgumentException(HERE) << "Theta must be >= 1";
  if (theta != theta_)
  {
    theta_ = theta;
    updatePickandFunction();
  }
}

/* psi1 parameter accessor */
void TawnCopula::setPsi1(const Scalar psi1)
{
  if (!(psi1 >= 0.0) || !(psi1 <= 1.0))
    throw InvalidArgumentException(HERE) << "Psi1 must be in [0, 1]";
  if (psi1 != psi1_)
  {
    psi1_ = psi1;
    updatePickandFunction();
  }
}

/* psi2 parameter accessor */
void TawnCopula::setPsi2(const Scalar psi2)
{
  if (!(psi2 >= 0.0) || !(psi2 <= 1.0))
    throw InvalidArgumentException(HERE) << "Psi2 must be in [0, 1]";
  psi2_ = psi2;
  updatePickandFunction();
}

/* Method save() stores the object through the StorageManager */
void TawnCopula::save(Advocate & adv) const
{
  ExtremeValueCopula::save(adv);
  adv.saveAttribute( "theta_", theta_ );
  adv.saveAttribute( "psi1_", psi1_ );
  adv.saveAttribute( "psi2_", psi2_ );
}

/* Method load() reloads the object from the StorageManager */
void TawnCopula::load(Advocate & adv)
{
  ExtremeValueCopula::load(adv);
  adv.loadAttribute( "theta_", theta_ );
  adv.loadAttribute( "psi1_", psi1_ );
  adv.loadAttribute( "psi2_", psi2_ );
  updatePickandFunction();
}


} /* namespace OTVINE */
