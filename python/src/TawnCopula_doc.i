%feature("docstring") OTVINE::TawnCopula
R"RAW(Tawn copula (asymmetric logistic model).

This is an extreme value copula with Pickands dependence function:

.. math::

    A(t) = (1-\psi_1)(1-t) + (1-\psi_2)t + [(\psi_2 t)^{\theta} + (\psi_1 (1-t))^{\theta}]^{1/\theta}

for :math:`\theta \ge 1`, :math:`\psi_1, \psi_2 \in [0, 1]`.

Parameters
----------
theta : float
    :math:`\theta \ge 1`
psi1 : float
    :math:`\psi_1 \in [0, 1]`
psi2 : float
    :math:`\psi_2 \in [0, 1]`

Examples
--------
>>> import otvine
>>> copula = otvine.TawnCopula(2.0, 0.6, 0.3)
>>> print(copula)
TawnCopula(theta = 2, psi1 = 0.6, psi2 = 0.3)
)RAW"
