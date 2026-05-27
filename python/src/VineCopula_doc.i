%feature("docstring") OTVINE::VineCopula
R"RAW(Vine copula (D-vine) built with a simplified syntax.

This class implements a D-vine copula using native OpenTURNS distribution objects.
It provides :meth:`addArc` to incrementally build the vine by adding pair-copulas
tree by tree.

The first ``d - 1`` arcs form the first tree and determine the D-vine order.
The next ``d - 2`` arcs form the second tree, and so on.

Available methods: :meth:`computePDF`, :meth:`computeCDF`, :meth:`getRealization`,
:meth:`getSample`.

Parameters
----------
arg : int or 2-tuple
    If an integer, the dimension of the vine copula. Additional arcs are
    added via :meth:`addArc`. If a tuple ``(matrix, copulas)``, the vine
    is built directly from an R-vine matrix and a collection of bivariate
    pair-copulas.

Notes
-----
The R-vine matrix is a lower-triangular matrix using the vinecopulib
convention (1-based variable indices):

- The counter-diagonal entries give the D-vine order.
- The remaining lower-triangular entries encode the structure array.

After construction, :meth:`computePDF` evaluates the D-vine density using
the v-matrix algorithm (product of pair-copula densities). The
:meth:`getRealization` method uses the inverse Rosenblatt transform with
numerical h-function inversion.

Only D-vine structures are supported via :meth:`addArc`.

Examples
--------
>>> import openturns as ot
>>> import otvine
>>> copula = otvine.VineCopula(3)
>>> copula.addArc([0, 1], ot.FrankCopula(3.0))
>>> copula.addArc([1, 2], ot.GumbelCopula(2.0))
>>> copula.addArc([0, 2], ot.ClaytonCopula(1.5))
>>> print(copula)
VineCopula
>>> copula.computePDF([0.3, 0.5, 0.7]) 
2.225...
)RAW"

%feature("docstring") OTVINE::VineCopula::getMatrix
R"RAW(Accessor to the R-vine matrix.

Returns
-------
matrix : :class:`~openturns.Matrix`
    The R-vine matrix (lower-triangular, 1-based variable indices).
)RAW"

%feature("docstring") OTVINE::VineCopula::getCopulaCollection
R"RAW(Accessor to the pair-copula collection.

Returns
-------
copulas : :class:`~openturns.Collection` of :class:`~openturns.Distribution`
    The pair-copulas stored in tree-by-tree order.
)RAW"

%feature("docstring") OTVINE::VineCopula::addArc
R"RAW(Add a pair-copula to the vine.

Parameters
----------
indices : sequence of int
    The two variable indices connected by this arc (0-based).
distribution : :class:`~openturns.Distribution`
    A bivariate copula describing the dependence.

Notes
-----
Arcs must be added tree-by-tree in D-vine order:

- The first ``d - 1`` arcs define the first tree (consecutive pairs).
- The next ``d - 2`` arcs define the second tree (gap-2 pairs).
- Continue until the single last arc connects the first and last variable.

Example
-------
>>> import openturns as ot
>>> import otvine
>>> copula = otvine.VineCopula(3)
>>> copula.addArc([0, 1], ot.FrankCopula(3.0))
>>> copula.addArc([1, 2], ot.GumbelCopula(2.0))
>>> copula.addArc([0, 2], ot.ClaytonCopula(1.5))
)RAW"

