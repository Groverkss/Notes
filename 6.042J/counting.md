# Counting

### Approximating a summation

It is not always possible to find a closed-form expression for a sum. For example, no closed form is known for 

$$
S = \sum_{i=1}^{n} \sqrt{i}
$$

In such cases, we have to resort to approximating sums.

Let $f : R^+ \rightarrow R^+$ be a *non decreasing function*. Define

$$
S ::= \sum_{i = 1}^{n}f(i) \\
$$

and

$$
I ::= \int_1^n f(x)dx 
$$

Then

$$
I + f(1) \le S \le I + f(n)
$$

*Similarily*, if $f$ is *non increasing*, then

$$
I + f(n) \le S \le I + f(1)
$$

**Proof:**

Suppose $f : R^+ \rightarrow R^+$ is non decreasing. The value of $S$ is the sum of the areas of $n$ unit-width rectangles of height $f(1), f(2), \dots, f(n)$. The areas are shown in the following figure:

![13.1](https://i.imgur.com/Qcl92ho.png)

The value of $I$ is shown in the figure below:

![13.2](https://i.imgur.com/VnqY9KF.png)

Comparing the above two figurse, the lower bound of the area can be seen as $f(1) + I$. Therefore, $S \ge f(1) + I$. <br><br>

To derive the the area for the upperbound, we shift the curve to one unit by left as shown below. 

![13.3](https://i.imgur.com/R9kMR8u.png)

Now, the upperbound can be easily seen as $I + f(n)$. Therefore $S \le I + f(n)$. <br><br>

A similar analysis can be done for *non increasing functions*, or you can just see that the mirror image of the graph of a *non decreasing function* is a *non increasing function* and replace $x$ with $-x$ to get the mirror image and invert the equality signs.

### Asymptotic Notation

Note: All of these relations are on functions.

1. **Asymptotic Equivalence** (~)

Def: $f(n)$ ~ $g(n)$

$$
\lim_{n \rightarrow \infty} \frac{f(n)}{g(n)} = 1
$$

For example, $n^2$ ~ $n^2 + n$ because 

$$
\lim_{n \rightarrow \infty} \frac{n^2 + n}{n^2} = 1
$$

~ is an equivalence relation.

2. **Asymptotically smaller** (Little Oh: o(.))

Def: $f(n) = o(g(n))$ iff 

$$
\lim_{n \rightarrow \infty} \frac{f(n)}{g(n)} = 0
$$

o(.) is a strict partial order relation

3. **Asymptotic Order of Growth** (Big Oh: O(.))

Def: $f = O(g)$

$$
\limsup_{n \rightarrow \infty} \frac{f(n)}{g(n)} < \infty
$$

So this is just shit now.

4. **Same order of growth** (Theta)

Def: $f = \theta(g)$

$$
f = O(g)
$$

and

$$
g = O(f)
$$

$\theta$ is an equivalance realtion

### Common Mistakes

> ". = O(.)" defines a relation

Don't write $O(g) = f$ <br> <br>

Otherwise: $x = O(x)$, so $O(x) = x$. But $2x = O(x)$, so $2x = O(x) = x$, $2x = x$ which is nonsense.

> "f is atleast O($n^2$)"

This should say $n^2 = O(f)$.

> Treating O(.) as a quantity is wrong

You can't say $O(1) + O(1) + \dots + O(1) = O(n)$. Simple counter-example, $\sum_{i = 1}^{n}i = \theta(n^2)$ not O(n). 