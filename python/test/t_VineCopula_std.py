#!/usr/bin/env python

import openturns as ot
import openturns.testing as ott
import otvine

ot.RandomGenerator.SetSeed(0)

print("=== 3D D-vine: Frank + Gumbel + Clayton ===")
copula = otvine.VineCopula(3)
copula.addArc([0, 1], ot.FrankCopula(3.0))
copula.addArc([1, 2], ot.GumbelCopula(2.0))
copula.addArc([0, 2], ot.ClaytonCopula(1.5))
print(copula)
print(repr(copula))

u = [0.3, 0.5, 0.7]
pdf = copula.computePDF(u)
print(f"PDF={pdf}")
assert pdf > 0.0

sample = copula.getSample(100)
assert sample.getSize() == 100
assert sample.getDimension() == 3
print(f"Sample mean={sample.computeMean()}")

r = copula.getRealization()
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
print(repr(copula4))

u4 = [0.3, 0.4, 0.5, 0.6]
pdf4 = copula4.computePDF(u4)
print(f"4D PDF={pdf4}")
assert pdf4 > 0.0

sample4 = copula4.getSample(100)
assert sample4.getSize() == 100
assert sample4.getDimension() == 4

print()
print("=== 2D degenerate vine ===")
copula2 = otvine.VineCopula(2)
copula2.addArc([0, 1], ot.GumbelCopula(2.0))
print(repr(copula2))
pdf2 = copula2.computePDF([0.5, 0.5])
print(f"2D PDF={pdf2}")
assert pdf2 > 0.0

print()
print("=== Matrix/collection constructor + accessors ===")
# Build a 3D vine, extract matrix+copulas, construct a new one
copula_m = otvine.VineCopula(3)
copula_m.addArc([0, 1], ot.FrankCopula(3.0))
copula_m.addArc([1, 2], ot.GumbelCopula(2.0))
copula_m.addArc([0, 2], ot.ClaytonCopula(1.5))
mat = copula_m.getMatrix()
copulas = copula_m.getCopulaCollection()
print(f"Matrix dim={mat.getNbRows()}x{mat.getNbColumns()}")
print(f"Nb copulas={copulas.getSize()}")
assert mat.getNbRows() == 3
assert copulas.getSize() == 3

# Reconstruct from matrix and copulas
copula_r = otvine.VineCopula(mat, copulas)
pdf_r = copula_r.computePDF([0.3, 0.5, 0.7])
print(f"Reconstructed PDF={pdf_r}")
assert abs(pdf_r - pdf) < 1e-14

# Accessors
mat2 = copula_r.getMatrix()
assert mat2.getNbRows() == 3
copulas2 = copula_r.getCopulaCollection()
assert len(copulas2) == 3

print()
print("All tests passed!")
