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
#include "otvine/VineCopula.hxx"
#include <openturns/PersistentObjectFactory.hxx>
#include <openturns/SpecFunc.hxx>
#include <openturns/RandomGenerator.hxx>

#include <map>
#include <set>
#include <vector>

using namespace OT;

namespace OTVINE
{

CLASSNAMEINIT(VineCopula);

static Factory<VineCopula> Factory_VineCopula;

static const Scalar EPS = std::sqrt(SpecFunc::ScalarEpsilon);


VineCopula::VineCopula()
  : DistributionImplementation()
{
  isCopula_ = true;
  setName("VineCopula");
}

VineCopula::VineCopula(const UnsignedInteger dimension)
  : DistributionImplementation()
{
  isCopula_ = true;
  setName("VineCopula");
  setDimension(dimension);
}

VineCopula::VineCopula(const Matrix & matrix,
                       const Collection<Distribution> & copulaCollection)
  : DistributionImplementation()
{
  isCopula_ = true;
  setName("VineCopula");
  setDimension(matrix.getNbRows());
  matrix_ = matrix;
  copulaCollection_ = copulaCollection;
}

VineCopula * VineCopula::clone() const
{
  return new VineCopula(*this);
}

void VineCopula::addArc(const Indices & indices, const Distribution & distribution)
{
  if (indices.getSize() != 2)
    throw InvalidArgumentException(HERE) << "VineCopula::addArc expected 2 indices, got " << indices.getSize();
  if (distribution.getDimension() != 2)
    throw InvalidArgumentException(HERE) << "VineCopula::addArc expected bivariate distribution, got dimension " << distribution.getDimension();
  if (!distribution.isCopula())
    throw InvalidArgumentException(HERE) << "VineCopula::addArc expected a copula";

  arcBuffer_.push_back(indices);
  copulaCollection_.add(distribution);
  buildMatrix();
}

Matrix VineCopula::getMatrix() const
{
  return matrix_;
}

PersistentCollection<Distribution> VineCopula::getCopulaCollection() const
{
  return copulaCollection_;
}

namespace
{

// Compute arc index in the collection for a D-vine
// tree: 0-indexed tree level
// edge: 0-indexed edge within the tree
UnsignedInteger arcIndex(UnsignedInteger tree, UnsignedInteger edge, UnsignedInteger d)
{
  return tree * d - tree * (tree + 1) / 2 + edge;
}

// Compute F(u_j | u_{j-1}, ..., u_{i+1}) for the "right side" pseudo-observation
// Uses hfunc2 (P(Var_2 ≤ v | Var_1 = u)) computed via finite differences of CDF
Scalar hfunc2(const Distribution & copula, Scalar v, Scalar u)
{
  const Scalar uLo = std::max(0.0, u - EPS);
  const Scalar uHi = std::min(1.0, u + EPS);
  const Scalar cLo = copula.computeCDF(Point({uLo, v}));
  const Scalar cHi = copula.computeCDF(Point({uHi, v}));
  return (cHi - cLo) / (uHi - uLo);
}

// Inverse of hfunc2: find v such that hfunc2(copula, v, u) = q
// Uses bisection
Scalar hfunc2_inv(const Distribution & copula, Scalar q, Scalar u)
{
  if (q <= 0.0) return 0.0;
  if (q >= 1.0) return 1.0;
  Scalar lo = 0.0;
  Scalar hi = 1.0;
  for (UnsignedInteger iter = 0; iter < 50; ++iter)
  {
    const Scalar mid = 0.5 * (lo + hi);
    const Scalar fmid = hfunc2(copula, mid, u) - q;
    if (fmid > 0.0)
      hi = mid;
    else
      lo = mid;
    if (hi - lo < 1e-14)
      break;
  }
  return 0.5 * (lo + hi);
}

// Build the R-vine matrix for a D-vine with given order (0-based).
Matrix buildDVineMatrix(const std::vector<UnsignedInteger> & order)
{
  const UnsignedInteger d = order.size();
  Matrix mat(d, d);
  for (UnsignedInteger i = 0; i < d; ++i)
    for (UnsignedInteger j = 0; j < d; ++j)
      mat(i, j) = 0.0;

  // Fill the R-vine structure
  // Lower triangular matrix where:
  // - mat(d-1-i, i) = order[i] + 1 (counter-diagonal)
  // - mat(i, j) = order[i + j + 1] + 1 for i < d-1, j < d-1-i (structure above diagonal)
  for (UnsignedInteger i = 0; i < d; ++i)
    mat(d - 1 - i, i) = order[i] + 1.0;

  for (UnsignedInteger j = 0; j < d - 1; ++j)
    for (UnsignedInteger i = 0; i < d - 1 - j; ++i)
      mat(i, j) = order[i + j + 1] + 1.0;

  return mat;
}

} // anonymous namespace

void VineCopula::buildMatrix()
{
  const UnsignedInteger d = getDimension();
  if (d == 0) return;
  const UnsignedInteger expectedArcs = d * (d - 1) / 2;
  if (arcBuffer_.size() < expectedArcs) return;
  if (arcBuffer_.size() > expectedArcs)
  {
    throw InvalidArgumentException(HERE)
        << "VineCopula::buildMatrix expected at most " << expectedArcs
        << " arcs, got " << arcBuffer_.size();
  }
  // Extract D-vine order from tree 0 edges (first d-1 arcs)
  std::map<UnsignedInteger, std::set<UnsignedInteger>> adj;
  for (UnsignedInteger i = 0; i < d - 1; ++i)
  {
    const UnsignedInteger a = arcBuffer_[i][0];
    const UnsignedInteger b = arcBuffer_[i][1];
    adj[a].insert(b);
    adj[b].insert(a);
  }

  UnsignedInteger start = arcBuffer_[0][0];
  for (const auto & pair : adj)
  {
    if (pair.second.size() == 1)
    {
      start = pair.first;
      break;
    }
  }

  std::vector<UnsignedInteger> order0(d);
  order0[0] = start;
  for (UnsignedInteger i = 1; i < d; ++i)
  {
    for (UnsignedInteger neighbor : adj[order0[i - 1]])
    {
      if ((i == 1) || (neighbor != order0[i - 2]))
      {
        order0[i] = neighbor;
        break;
      }
    }
  }

  matrix_ = buildDVineMatrix(order0);
}

Scalar VineCopula::computePDF(const Point & point) const
{
  if (matrix_.getNbRows() == 0)
    throw InvalidArgumentException(HERE) << "VineCopula::computePDF: no arcs added";
  const UnsignedInteger d = getDimension();
  const UnsignedInteger nbTrees = d - 1;

  // v[tree][edge] matrix of pseudo-observations
  // v[0][i] = u_i
  // v[t+1][i] = hfunc1(v[t][i], v[t][i+1]) from C_{i, i+t+1}
  std::vector<std::vector<Scalar>> v(d);
  for (UnsignedInteger t = 0; t < d; ++t)
    v[t].resize(d - t);

  for (UnsignedInteger i = 0; i < d; ++i)
    v[0][i] = point[i];

  Scalar pdf = 1.0;

  for (UnsignedInteger t = 0; t < nbTrees; ++t)
  {
    const UnsignedInteger nbEdges = d - 1 - t;
    for (UnsignedInteger i = 0; i < nbEdges; ++i)
    {
      const UnsignedInteger idx = arcIndex(t, i, d);
      const Distribution & copula = copulaCollection_[idx];

      // Pair-copula density contribution
      pdf *= copula.computePDF(Point({v[t][i], v[t][i + 1]}));

      // Update pseudo-observation for next tree
      v[t + 1][i] = copula.computeConditionalCDF(v[t][i], Point(1, v[t][i + 1]));
    }
  }

  return pdf;
}

Scalar VineCopula::computeCDF(const Point & point) const
{
  if (matrix_.getNbRows() == 0)
    throw InvalidArgumentException(HERE) << "VineCopula::computeCDF: no arcs added";
  return DistributionImplementation::computeCDF(point);
}

Point VineCopula::getRealization() const
{
  if (matrix_.getNbRows() == 0)
    throw InvalidArgumentException(HERE) << "VineCopula::getRealization: no arcs added";
  const UnsignedInteger d = getDimension();

  // Generate independent uniforms
  Point w(d);
  for (UnsignedInteger i = 0; i < d; ++i)
    w[i] = RandomGenerator::Generate();

  // v[t][i] matrix (same structure as in computePDF)
  std::vector<std::vector<Scalar>> v(d);
  for (UnsignedInteger t = 0; t < d; ++t)
    v[t].resize(d - t);

  // x_0 = w[0]
  v[0][0] = w[0];

  for (UnsignedInteger i = 1; i < d; ++i)
  {
    // Inverse Rosenblatt cascade
    Scalar val = w[i];
    for (UnsignedInteger k = 0; k < i; ++k)
    {
      const UnsignedInteger idx = arcIndex(k, i - k - 1, d);
      val = hfunc2_inv(copulaCollection_[idx], val, v[k][i - k - 1]);
    }
    v[0][i] = val;

    // Update pseudo-observations for this new variable
    for (UnsignedInteger k = 1; k <= i; ++k)
    {
      const UnsignedInteger idx = arcIndex(k - 1, i - k, d);
      v[k][i - k] = copulaCollection_[idx].computeConditionalCDF(v[k - 1][i - k], Point(1, v[k - 1][i - k + 1]));
    }
  }

  Point result(d);
  for (UnsignedInteger i = 0; i < d; ++i)
    result[i] = v[0][i];
  return result;
}

Sample VineCopula::getSample(const UnsignedInteger size) const
{
  Sample result(size, getDimension());
  for (UnsignedInteger i = 0; i < size; ++i)
    result[i] = getRealization();
  return result;
}

Point VineCopula::getParameter() const
{
  return {};
}

void VineCopula::setParameter(const Point & parameter)
{
  if (parameter.getSize() != 0)
    throw InvalidArgumentException(HERE) << "Error: expected 0 values, got " << parameter.getSize();
}

Description VineCopula::getParameterDescription() const
{
  return {};
}

Bool VineCopula::operator ==(const VineCopula & other) const
{
  if (this == &other) return true;
  return false;
}

Bool VineCopula::equals(const DistributionImplementation & other) const
{
  const VineCopula* p_other = dynamic_cast<const VineCopula*>(&other);
  return p_other && (*this == *p_other);
}

String VineCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << VineCopula::GetClassName();
  return oss;
}

String VineCopula::__str__(const String & ) const
{
  OSS oss;
  oss << "VineCopula";
  return oss;
}

void VineCopula::save(Advocate & adv) const
{
  DistributionImplementation::save(adv);
  adv.saveAttribute("matrix_", matrix_);
  adv.saveAttribute("copulaCollection_", copulaCollection_);
}

void VineCopula::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
  adv.loadAttribute("matrix_", matrix_);
  adv.loadAttribute("copulaCollection_", copulaCollection_);
}


} /* namespace OTVINE */
