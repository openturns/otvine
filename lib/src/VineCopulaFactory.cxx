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
#include "otvine/VineCopulaFactory.hxx"
#include "otvine/TawnCopula.hxx"
#include "otvine/RotatedCopula.hxx"
#include "otvine/VinecopCopula.hxx"

#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/ClaytonCopula.hxx>
#include <openturns/GumbelCopula.hxx>
#include <openturns/FrankCopula.hxx>
#include <openturns/JoeCopula.hxx>
#include <openturns/NormalCopula.hxx>
#include <openturns/StudentCopula.hxx>

#include <vinecopulib.hpp>

using namespace OT;

namespace OTVINE
{

CLASSNAMEINIT(VineCopulaFactory)

static Factory<VineCopulaFactory> Factory_VineCopulaFactory;


/* Default constructor */
VineCopulaFactory::VineCopulaFactory()
  : DistributionFactoryImplementation()
{
  // Nothing to do
}

/* Virtual constructor method */
VineCopulaFactory * VineCopulaFactory::clone() const
{
  return new VineCopulaFactory(*this);
}

/* example of a func that return a point squared. */
Distribution VineCopulaFactory::build(const Sample & sample) const
{
  const UnsignedInteger dimension = sample.getDimension();
  const UnsignedInteger size = sample.getSize();
  Eigen::MatrixXd data(size, dimension);
  for (UnsignedInteger i = 0; i < size; ++ i)
    for (UnsignedInteger j = 0; j < dimension; ++ j)
      data(i, j) = sample(i, j);
  Pointer <vinecopulib::Vinecop> p_vinecop = new vinecopulib::Vinecop(dimension);
  std::vector<vinecopulib::BicopFamily> family_set;
  if (native_)
  {
    family_set = {
      vinecopulib::BicopFamily::gaussian,
      vinecopulib::BicopFamily::clayton,
      vinecopulib::BicopFamily::gumbel,
      vinecopulib::BicopFamily::frank,
      vinecopulib::BicopFamily::student,
      vinecopulib::BicopFamily::tawn,
    };
  }
  vinecopulib::FitControlsVinecop controls(family_set);
  controls.set_allow_rotations(allowRotations_);
  p_vinecop->select(data, controls);

  if (native_)
  {
    std::vector<std::vector<vinecopulib::Bicop> > pairs = p_vinecop->get_all_pair_copulas();
    if (p_vinecop->get_dim() == 2 && pairs.size() == 1 && pairs[0].size() == 1)
    {
      vinecopulib::Bicop bicop = pairs[0][0];
      Distribution copula;
      const Scalar theta = bicop.get_parameters()(0, 0);
      LOGDEBUG(OSS() << "family="<< bicop.get_family_name() << " theta=" << theta << " rotation=" << bicop.get_rotation());

      if (bicop.get_family_name() == "Clayton")
        copula = ClaytonCopula(theta);
      else if (bicop.get_family_name() == "Gaussian")
      {
        CorrelationMatrix R(2);
        R(1, 0) = theta;
        copula = NormalCopula(R);
      }
      else if (bicop.get_family_name() == "Student")
      {
        CorrelationMatrix R(2);
        R(1, 0) = theta;
        const Scalar nu = bicop.get_parameters()(1, 0);
        copula = StudentCopula(nu, R);
        copula.setName("StudentCopula"); // Fixed in OT>=1.27
      }
      else if (bicop.get_family_name() == "Frank")
        copula = FrankCopula(theta);
      else if (bicop.get_family_name() == "Gumbel")
        copula = GumbelCopula(theta);
      // else if (bicop.get_family_name() == "Joe")
      //   copula = JoeCopula(theta);
      else if (bicop.get_family_name() == "Tawn")
      {
        const Scalar psi1 = bicop.get_parameters()(0, 0);
        const Scalar psi2 = bicop.get_parameters()(1, 0);
        const Scalar thetaTawn = bicop.get_parameters()(2, 0);
        copula = TawnCopula(thetaTawn, psi1, psi2);
      }
      if (copula.getDimension() == 2)
      {
        int rotation = bicop.get_rotation();
        if (rotation == 0)
          return copula;
        else
          return RotatedCopula(copula, rotation);
      }
    }
  }
  return VinecopCopula(p_vinecop);
}


/* String converter */
String VineCopulaFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << VineCopulaFactory::GetClassName();
  return oss;
}

/* Method save() stores the object through the StorageManager */
void VineCopulaFactory::save(Advocate & adv) const
{
  DistributionFactoryImplementation::save(adv);
  adv.saveAttribute("native_", native_);
}

/* Method load() reloads the object from the StorageManager */
void VineCopulaFactory::load(Advocate & adv)
{
  DistributionFactoryImplementation::load(adv);
  adv.loadAttribute("native_", native_);
}

void VineCopulaFactory::setNative(const Bool native)
{
  native_ = native;
}

void VineCopulaFactory::setAllowRotations(const OT::Bool allowRotations)
{
  allowRotations_ = allowRotations;
}

} /* namespace OTVINE */
