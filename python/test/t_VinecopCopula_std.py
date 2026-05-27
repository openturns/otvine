#!/usr/bin/env python

import openturns as ot
import openturns.testing as ott
import otvine

ot.RandomGenerator.SetSeed(0)

print("=== 3D D-vine: matrix/collection constructor ===")
copula = otvine.VineCopula(3)
copula.addArc([0, 1], ot.FrankCopula(3.0))
copula.addArc([1, 2], ot.GumbelCopula(2.0))
copula.addArc([0, 2], ot.ClaytonCopula(1.5))
mat = copula.getMatrix()
copulas = copula.getCopulaCollection()
print(f"Matrix dim={mat.getNbRows()}x{mat.getNbColumns()}")
print(f"Nb copulas={len(copulas)}")

# Construct VinecopCopula from matrix + copulas
vc = otvine.VinecopCopula(mat, copulas)
print(vc)
print(repr(vc))

u = [0.3, 0.5, 0.7]
pdf0 = copula.computePDF(u)
pdf = vc.computePDF(u)
print(f"Native PDF={pdf0}, Vinecop PDF={pdf}")
assert pdf > 0.0
ott.assert_almost_equal(pdf0, pdf, 1e-7, 0.0)

sample = vc.getSample(100)
assert sample.getSize() == 100
assert sample.getDimension() == 3
print(f"Sample mean={sample.computeMean()}")

r = vc.getRealization()
assert r.getDimension() == 3

print()
print("=== 4D D-vine ===")
copula4 = otvine.VineCopula(4)
copula4.addArc([0, 1], ot.FrankCopula(3.0))
copula4.addArc([1, 2], ot.GumbelCopula(2.0))
copula4.addArc([2, 3], ot.FrankCopula(2.0))
copula4.addArc([0, 2], ot.ClaytonCopula(1.5))
copula4.addArc([1, 3], ot.NormalCopula(ot.CorrelationMatrix(2)))
copula4.addArc([0, 3], ot.GumbelCopula(1.5))
mat4 = copula4.getMatrix()
copulas4 = copula4.getCopulaCollection()

vc4 = otvine.VinecopCopula(mat4, copulas4)
pdf4_0 = copula4.computePDF([0.3, 0.4, 0.5, 0.6])
pdf4 = vc4.computePDF([0.3, 0.4, 0.5, 0.6])
print(f"4D Native PDF={pdf4_0}, Vinecop PDF={pdf4}")
assert pdf4 > 0.0
ott.assert_almost_equal(pdf4_0, pdf4, 1e-7, 0.0)



sample4 = vc4.getSample(50)
assert sample4.getSize() == 50
assert sample4.getDimension() == 4

print()
print("=== Distribution consistency (two-sample KS) ===")
ot.RandomGenerator.SetSeed(0)
sample_native = copula.getSample(500)
sample_vc = vc.getSample(500)
alpha = 0.01
for j in range(3):
    marg_native = sample_native.getMarginal(j)
    marg_vc = sample_vc.getMarginal(j)
    result = ot.HypothesisTest.TwoSamplesKolmogorov(marg_native, marg_vc, alpha)
    pval = result.getPValue()
    print(f"  Marginal {j}: p-value={pval:.4f}")
    assert pval > alpha, f"Marginal {j} KS test failed (p={pval})"

print()
print("All tests passed!")
