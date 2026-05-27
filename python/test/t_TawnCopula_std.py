#!/usr/bin/env python

import openturns as ot
import openturns.testing as ott
import otvine

# Test default constructor
copula = otvine.TawnCopula()
print(copula)
print(repr(copula))
assert "TawnCopula" in str(copula)

# Test parameter constructor
copula = otvine.TawnCopula(2.5, 0.6, 0.3)
print(copula)
assert "TawnCopula" in str(copula)

# Test accessors
assert copula.getTheta() == 2.5
assert copula.getPsi1() == 0.6
assert copula.getPsi2() == 0.3

# Test get/set parameter
param = copula.getParameter()
print(param)
assert param[0] == 2.5
assert param[1] == 0.6
assert param[2] == 0.3
desc = copula.getParameterDescription()
print(desc)

# Test sample
sample = copula.getSample(100)
assert sample.getSize() == 100
assert sample.getDimension() == 2

# Test PDF
pdf = copula.computePDF([0.5, 0.5])
print(f"pdf={pdf}")
assert pdf > 0.0

# Test CDF
cdf = copula.computeCDF([0.5, 0.5])
print(f"cdf={cdf}")

# Test validation
ot.Log.Show(ot.Log.TRACE)
validation = ott.DistributionValidation(copula)
validation.run()
