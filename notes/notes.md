# Alphabet Reduction Step

1. Convert PCP into boolean circuit $\psi$
2. Build an assignment tester on $\psi$
3. Turn assignment tester back to a PCP


## Conversion to a boolean circuit



## Assignment Tester

### Modified Test

With probability $1/2$, run the above Long Code Test.

With probability $1/2$, run consistency Check:
1. Choose random $x_i$ in $X = \{x_1, ..., x_s\}$ where X is the original boolean circuit
2. Choose a random circuit $f \in L$
3. Test $\sigma(x_i) = A(f) \oplus A(f + e_i)$ where $\sigma(x_i)$ is the assignment value for $x_i$
4. Create new variable $z_r$ per outcome $r$ from random coin tosses

#### The proof for consistency test:

Note $\text{true} = -1$, $\text{false} = 1$, so xor becomes multiplication

define $e_i$

$$e_i(a) = a(i)$$

$$A(f+e_i) = (f+e_i)(a) = f(a) \oplus a(i) = f(a) \cdot a(i)$$
$$A(f)\oplus A(f+e_i) = A(f) \cdot A(f+e_i) = f(a)\cdot (f(a)\cdot a(i)) = a(i) = \sigma(x_i)$$

#### Long Code Test ($T$)

The long Code Test $T$ is a randomised algorithm used to check if a string A is a valid codeword with respect to a function $\psi$. Defined as $T^A_\psi$.

let $L = \{\{\{0, 1\} ^ s\rightarrow \{0, 1\}\}\rightarrow \{0, 1\}\}$ 

$\forall f \in L.\ A_a(f) = f(a)$

Input: $\psi: \{0, 1\}^s \rightarrow \{0, 1\}$

Oracle Access to $A$: The test can query $A$ at specific locations.

Note that $A$ is folded with $\psi$, meaning that $\forall f. A(f) = A(f\wedge \psi)$. This is only the case for some $f(a) = (f\wedge \psi)(a) = f(a) \wedge \psi(a)$. Note that the folding property holds either when $\psi(a) = \text{true}$, or $\forall f. A(f) = \text{false}$

Computes predicate $w : \{0, 1\} ^ 3 \rightarrow \{\text{true}, \text{false}\}$ and three locations $f_1, f_2, f_3 \in L$. The output is $w(A(f_1), A(f_2), A(f_3))$.

This test has perfect completeness and strong soundness

### Test construction

Identify $[n] = \{0, 1\}^s$

Boolean functions $\psi : [n] \rightarrow \{1, -1\}$ and $-1$ is true

Long code $L$ is $\{f:[n] \rightarrow\{1, -1\}\}$

Let $A : L \rightarrow\{1, -1\}$ 

By definition of folded string, $\forall f\in \{-1, 1\}^n A(-f) = -A(f)$ and $A(f) = A(f\wedge \psi)$

If $\psi(a) = -1$, then automatically $A$ is folded. 

#### Test procedure

1. Select $f, g\in L$ uniformly at random. 
2. define $\mu$ where $\forall y$ if $f(y) = 1$, set $\mu(y) = -1$, if $f(y) = -1$, $99\%$ chance $\mu(y) = 1$, $1\%$ chance $\mu(y) = -1$
3. Set $h$ to be the pointwise product of $g$ and $\mu$, which is equivalent to do a pointwise xor
4. Accept unless $A(g) = A(f) = A(h) = 1$ meaning they are all false. 

Note:
* If $\psi(a)$ is false, then $\forall f. A(f)$ is false, then test will never accept.
* If $\psi(a)$ is true, then 
  * if $A(f)$ is true, test accept
  * if $A(f)$ is false, we have $\mu(a) = -1$. Then either $g(a) = -1$ or $h(a) = -1$ based on construction of $g, h$. (test validity of code)

## Constructing new PCP
1. Introduce variables $X$ and $L$, where $X$ is the set of boolean variables in boolean circuit, and $L$ is the set of locations in long code
2. Introducing variables $z_r$, which has domain $\{0, 1\} ^ 3$. For each random coin toss, $z_r$'s value contains either the encoding of the values at $f_1, f_2, f_3$, or $\sigma(x_i), f, f+e_i$
3. Variables from $X, L$ also have domain $\{0, 1\}$ where false is encoded as $(0, 0, 0)$ and true is $(1, 1, 1)$
4. The constraints are defined by a consistency constraint between $z_r$ and some $x, l \in X, L$. We can encode test as unary constraint on $z_r$.