# Generating Functions

## Ordinary Generating Function
### Some problems on finding OGF
#### Finding OGF of Finbonacci Series

The fibonacci series is defined as $f_{n+2} = f_{n+1} + f_{n}, f_0 = 0, f_1 = 1$. Let us solve this using generating functions. Let $F(x)$ denote the ordinary generating function of $f$. Multiply both sides by $x^n$

$$f_{n+2}x^n = f_{n+1}x^n + f_{n}x^n \\ $$
$$\frac{F(x) - f_1x - f_0}{x^2} = \frac{F(x) - f_0}{x} + F(x)$$

Putting values of $f_0\ \text{and}\ f_1$

$$F(x) - x = xF(x) + x^2F(x)$$
$$F(x) = \frac{x}{1 - x - x^2}$$

#### Finding OGF of Catalan numbers

The catalan numbers are defined by $c_0 = 1$ and $c_{n+1} = \sum_{i=0}^{n}c_ic_{n-i}$. Let $C(x)$ denote the OGF of this series. Multiply both sides by $x^n$

$$c_{n+1}x^n = \sum_{i=0}^{n}c_ic_{n-i}x^n$$
$$\frac{C(x) - c_0}{x} = \sum_{i=0}^{n} c_ix^i c_{n-i}x^{n-i}$$

The R.H.S. can be easily seen as $C(x)^2$

$$\frac{C(x) - 1}{x} = C(x)^2$$
$$C(x) = \frac{1 \pm \sqrt{1 - 4x}}{2x}$$

If we take the $+$ sign then $C(x)$ does not converge at $x=0$.

$$\therefore C(x) = \frac{1 - \sqrt{1 - 4x}}{2x}$$

## Exponential Generating Function  

Let $a_0, a_1, a_2, \dots$ be a sequence of numbers. Then EGF of $a$ is defined as $A(x) = \sum_{i=0}^{\infty}\frac{a_i}{i!}x^i$.

#### Finding EGF of Bell numbers

## Algebric Manipulation of Generating Functions

#### Addition

For both OGF and EGF, $C(x) = A(x) + B(x)$ generates the sequence $c_n = a_n + b_n$ .

#### Shifting

For OGF, $C(x) = x^kA(x)$ generates the sequence $c_n = a_{n-k}$ where $a_i = 0\ \forall\ i<0$. <br> <br>

For EGF you need to integrate the series $A(x),\ k$ times to get the same effect.<br> <br>

For OGF, $C(x) = \frac{A(x) - (a_0 + a_1x^1 + a_2x^2 + \dots + a_{k-1}x^{k-1})}{x^k}$ generates the sequence $c_n = a_{n+k}$. <br> <br>

For EGF, $C(x) = A^{(k)}x$ generates the sequence $c_n = a_{n+k}$, where $A^{(x)}(x)$ denotes $A(x)$ differenciated $k$ times.

#### Multiplication by n

#### Convulation

The most important operation on generating functions.

For OGF, $C(x) = A(x)B(x)$ generates the sequence $c_n = \sum_{k=0}^{n}a_kb_{n-k}$. <br><br>

For EGF, $C(x) = A(x)B(x)$ generates the sequence $c_n = \sum_{k=0}^{n}\binom{n}{k}a_kb_{n-k}$. This is the reason EGF is useful in dealing with recurrences involving binomial coefficients or factorials.

#### Power of a generating function

This is a direct consequence of convulation. <br>

For OGF, $C(x) = A(x)^k$ generates the sequence $c_n = \sum_{i_1 + i_2 + \dots +i_k = n}a_{i_1}.a_{i_2}\dots a_{i_k}$ <br><br>

For EGF, $C(x) = A(x)^k$ generates the sequence $c_n = \sum_{i_1 + i_2 + \dots +i_k=n}\tfrac{n!}{i_1!i_2!\dots{i_k}!}a_{i_1}.a_{i_2}\dots a_{i_k}$

#### Prefix Sum trick

Useful trick for OGF, Suppose we have a sequence $a_0, a_1, a_2, \dots$. Lets $A(x)$ be OGF of this sequence. Then $C(x) = \frac{1}{1-x}A(x)$, where $C(x)$ denotes the OGF of sequence $c_0, c_1, c_2, \dots$ where $c_n = a_0 + a_1 + a_2 + \dots + a_n$. This trick is useful in calculating prefix sum of a sequence. Proof can easily be seen by convulation.

## Some Problems Involving Permutations

#### Cyclic from of permutations

Any permutation can be written as a union of disjoint cycles. A cycle is a set such that the element at position $a_i$ in the original permutation is $a_{i+1}$. For example the cycles $(1, 3, 5)(2, 4)$ uniquely represents the permutation $(3, 4, 5, 2, 1)$. <br> <br>

How many different permutations of length $n$ are a cycle? The total number of permutation of length $n$ is $n!$. But each permutation is counted $n$ extra times as the cycle (1 2 3) and (2 3 1) represent the same permutation (same can be said for $n$ such permutations). Therefore, the answer is $(n - 1)!$

#### Calculating a formula for Sterling numbers of first kind

Problem $\rightarrow$ Count the number of permutations of length $n$ with $k$ cycles. <br> <br>

