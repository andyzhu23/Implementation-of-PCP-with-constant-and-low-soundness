## Setup

We start with a 3-CNF instance $\phi$.

## Arithmetization

We create a function $C_\phi:\{0, 1\}^{O(\log n)}\rightarrow\{0, 1\}$.

Where $C_\phi(\alpha, \beta_1, \beta_2, \beta_3) = 1$ iff the $i^\text{th}$ literal in $\alpha^{\text{th}}$ clause is $\beta_i = (\gamma_i, \sigma_i)$. $\sigma_i$ is an encoding of the sign ($\neg$), and $\gamma_i$ is means we are dealing with the $\gamma_i^{\text{th}}$ clause.

We want $$\sum_{\beta_1\beta_2\beta_3\in\{0, 1\}^{3\log 2n}}\phi(\alpha, \beta_1, \beta_2, \beta_3)\cdot\prod_{i=1}^3(1-A'(\beta_i))=0$$ for every clause $\alpha$

## Small-Bias Reduction

We can use a pseudo-randomly generated list of weights $(s_1, ..., s_\phi) \in S$ and check $$\sum_{\alpha\beta_1\beta_2\beta_3\in\{0, 1\}^{l}}s_{\alpha}\cdot\phi(\alpha, \beta_1, \beta_2, \beta_3)\cdot\prod_{i=1}^3(1-A'(\beta_i))=0$$


## The Sum-Check Protocol

The process generalises the summation of a polynomial $P$ in the form $$\sum_{x_1,x_2,...,x_l} P(x_1, ..., x_l) = 0$$.

We check the sum by first let the variable $x_1$ be a free variable $z$, and consider the univariate polynomial $$g_1(z)=\sum_{x_2,...,x_l}P(z,x_2,...,x_l)$$

The verifier receives $g_1(z)$, and checks that $g_1(0) + g_1(1) = 0$. Then a random point $r_1$ is chosen, and we want to keep check if the value $g_1(r_1)$ is $$\sum_{r_1,x_2,...,x_l} P(r_1, ..., x_l) = 0$$

When translating to a CSP, $\forall z.g_1(z)$ is a variable in the CSP. 

Checking $g_1(0) + g_1(1) = 0$ becomes checking the corresponding variables with a corresponding sum.

For corresponding iterations, we check $g_i(r_1,...,r_i) = g_{i+1}(r_1,...,r_i,0) + g_{i+1}(r_1,...,r_i,1)$ for randomly picked points $r_i$.


## Low Degree Test

We need low degree test to show that the functions we claim $g$ are indeed low degree polynomials. 

We pick a random line $l(t) = x+ty$

we query $g_i$ on points $l(t)$ to get $v_t = g_i(l(t))$

to test $g$ is a degree-$d$ polynomial, we can run linear constraints on list of $d$ points $(l(t), v_t)$.


Try to find external libraries for pcpp

implement a fake pcpp