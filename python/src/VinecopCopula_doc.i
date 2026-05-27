%feature("docstring") OTVINE::VinecopCopula
R"RAW(Vine copula backed by vinecopulib.

Built from an R-vine matrix and a collection of pair-copulas, or from fitted data.

Parameters
----------
matrix : :class:`~openturns.Matrix`
    R-vine matrix (1-based indices, lower-triangular).
copulas : sequence of :class:`~openturns.Distribution`
    Pair-copulas in tree-by-tree order.

Examples
--------
>>> import openturns as ot
>>> import otvine
>>> # Build from matrix and copulas
>>> vine = otvine.VineCopula(3)
>>> vine.addArc([0, 1], ot.FrankCopula(3.0))
>>> vine.addArc([1, 2], ot.GumbelCopula(2.0))
>>> vine.addArc([0, 2], ot.ClaytonCopula(1.5))
>>> vc = otvine.VinecopCopula(vine.getMatrix(), vine.getCopulaCollection())
)RAW"
