## Axiomatized Theory
##### Alphabet
* Standard logical symbols (plus equality): $\lnot \, \land \, \lor \, \implies \, \iff \, \forall \, \, \, \exists \, \, \, ( \, \, \, ) \, \, \, , \, \, \, v_0, v_1, \dots, v_n \, \, \, =$
* Non-logical symbols in $\mathscr{L_A}$: $0 \, \, \, ' \, + \, \times \, <$
##### Grammar
- Terms are built from variables and $0$ by applying function symbols:
  - If $t$ is a term, $t'$ is a term (officially the function $'(t)$)
  - If $s, t$ are terms, then $(s + t)$ and $(s \times t)$ are terms
* Atomic formulas have the form $s = t$ or $s < t$ where $s, t$ are terms
* Formulas are built from atomic formulas using $\lnot \, \land \, \lor \, \implies \, \iff$ and quantifiers $\forall x \, \, \, \exists y$ in the usual first-order way
  * Similarly, a sentence is a formula with no free variables
##### Rules of Inference
> Could be any FOL proof system, for fun we show sequent calculus below...
######  Identity Rules
- Identity
$$
\dfrac{}{A \vdash A} (Id)
$$
- Reflexivity
$$
\dfrac{}{t = t} (Ref)
$$
- Substitution
$$
\dfrac{A(t), \Gamma \vdash \Delta}{s = t, A(s), \Gamma \vdash \Delta} (= L)
\qquad
\dfrac{\Gamma \vdash \Delta, A(t)}{\Gamma \vdash \Delta, s = t, A(s)} (= R)
$$
###### Structural Rules
- Weakening
$$
\dfrac{\Gamma \vdash \Delta}{A, \Gamma \vdash \Delta} (WL)
\qquad
\dfrac{\Gamma \vdash \Delta}{\Gamma \vdash \Delta, A} (WR)
$$
- Contraction
$$
\dfrac{A, A, \Gamma \vdash \Delta}{A, \Gamma \vdash \Delta} (CL)
\qquad
\dfrac{\Gamma \vdash \Delta, A, A}{\Gamma \vdash \Delta, A} (CR)
$$
- Exchange
$$
\dfrac{\Gamma, A, B, \Pi \vdash \Delta}{\Gamma, B, A, \Pi \vdash \Delta} (XL)
\qquad
\dfrac{\Gamma \vdash \Delta, A, B, \Lambda}{\Gamma \vdash \Delta, B, A, \Lambda} (XR)
$$
- (Cut)
$$
\dfrac{\Gamma \vdash \Delta, A \qquad A, \Pi \vdash \Lambda}{\Gamma, \Pi \vdash \Delta, \Lambda} (Cut)
$$
###### Propositional Rules
- $\lnot$
$$
\dfrac{\Gamma \vdash \Delta, A}{\lnot A, \Gamma \vdash \Delta} (\lnot L)
\qquad
\dfrac{A, \Gamma \vdash \Delta}{\Gamma \vdash \Delta, \lnot A} (\lnot R)
$$
- $\land$
$$
\dfrac{A,\Gamma \vdash \Delta}{A \land B, \Gamma \vdash \Delta} (\land L)
\qquad
\dfrac{B, \Gamma \vdash \Delta}{A \land B, \Gamma \vdash \Delta} (\land L)
\qquad
\dfrac{\Gamma \vdash \Delta, A \qquad \Gamma \vdash \Delta, B}{\Gamma \vdash \Delta, A \land B} (\land R)
$$
- $\lor$
$$
\dfrac{A, \Gamma \vdash \Delta \qquad B, \Gamma \vdash \Delta}{A \lor B, \Gamma \vdash \Delta} (\lor L)
\qquad
\dfrac{\Gamma \vdash \Delta, A}{\Gamma \vdash \Delta, A \lor B} (\lor R)
\qquad
\dfrac{\Gamma \vdash \Delta, B}{\Gamma \vdash \Delta, A \lor B} (\lor R)
$$
- $\implies$
$$
\dfrac{\Gamma \vdash \Delta, A \qquad B, \Pi \vdash \Lambda}{A \implies B, \Gamma, \Pi \vdash \Delta, \Lambda} (\implies L)
\qquad
\dfrac{A, \Gamma \vdash \Delta, B}{\Gamma \vdash \Delta, A \implies B} (\implies R)
$$
###### Quantifier Rules
- $\forall$
$$
\dfrac{A(t), \Gamma \vdash \Delta}{\forall x \, A(x), \Gamma \vdash \Delta} (\forall L)
\qquad
\dfrac{\Gamma \vdash \Delta, A(a)}{\Gamma \vdash \Delta, \forall x \, A(x)} (\forall R)
$$
- $\exists$
$$
\dfrac{A(a), \Gamma \vdash \Delta}{\exists x \, A(x), \Gamma \vdash \Delta} (\exists L)
\qquad
\dfrac{\Gamma \vdash \Delta, A(t)}{\Gamma \vdash \Delta, \exists x \, A(x)} (\exists R)
$$
##### Axioms
###### Robinson Axioms
Let $x'$ denote successor. Take the following eight sentences as $Q_0$:
- ($Q1$) $\forall x \, \forall y \, (x' = y' \implies x = y)$
- ($Q2$) $\forall x \, 0 \neq x'$
- ($Q3$) $\forall x \, (x = 0 \lor \exists y \, x = y')$
- ($Q4$) $\forall x \, (x + 0) = x$
- ($Q5$) $\forall x \, \forall y \, (x + y') = (x + y)'$
- ($Q6$) $\forall x \, (x \times 0) = 0$
- ($Q7$) $\forall x \, \forall y \, (x \times y') = ((x \times y) + x)$
- ($Q8$) $\forall x \, \forall y \, (x < y \iff \exists z \, (z' + x) = y)$
Then $Q$ can be defined as $Q = \{ A : Q_0 \vDash A\}$
###### Induction Schema
$$
\forall y_1, \dots, \forall y_n ((A(0) \land \forall x (A(x) \implies A(x'))) \implies \forall x \, A(x))
$$
$PA = Q + \text{ all such instances for every formula } A(x, y_1, \dots, y_n)$