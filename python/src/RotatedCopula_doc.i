%feature("docstring") OTVINE::RotatedCopula
R"RAW(Rotated copula.

It is defined by:

.. math::

    C_{90}(u_1, u_2) = u_2 - C(1-u_1, u_2) \\
    C_{180}(u_1, u_2) = u_1 + u_2 - 1 + C(1-u_1, 1-u_2) \\
    C_{270}(u_1, u_2) = u_1 - C(u_1, 1-u_2) \\

for :math:`(u_1, u_2) \in [0, 1]^2`

Parameters
----------
copula : :py:class:`openturns.Distribution`
    A copula
rotation : int
    The rotation angle as a multiple of 90 degrees

Examples
--------
>>> import otvine
>>> import openturns as ot
>>> copula = ot.GumbelCopula()
>>> rotated = otvine.RotatedCopula(copula, 180)
)RAW"

// ---------------------------------------------------------------------

%feature("docstring") OTVINE::RotatedCopula::getRotation
"Rotation accessor.

Returns
-------
rotation : int
    The rotation angle as a multiple of 90
"

// ---------------------------------------------------------------------

%feature("docstring") OTVINE::RotatedCopula::getCopula
"Copula accessor.

Returns
-------
copula : :py:class:`openturns.Distribution`
    A copula
"