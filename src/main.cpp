#include <cstdio>

#include "peano.hpp"

using namespace peano::dsl;

using x = Var<0>;
using y = Var<1>;
using z = Var<2>;

using Q1 = Forall<x, Forall<y, If<Eq<Succ<x>, Succ<y>>, Eq<x, y>>, Bounded<100>>, Bounded<100>>;
static_assert(peano::evaluate<Q1>());

using Q2 = Forall<x, Not<Eq<Zero, Succ<x>>>, Bounded<100>>;
static_assert(peano::evaluate<Q2>());

using Q3 = Forall<x, Or<Eq<x, Zero>, Exists<y, Eq<x, Succ<y>>, Bounded<100>>>, Bounded<100>>;
static_assert(peano::evaluate<Q3>());

using Q4 = Forall<x, Eq<Add<x, Zero>, x>, Bounded<100>>;
static_assert(peano::evaluate<Q4>());

using Q5 = Forall<x, Forall<y, Eq<Add<y, Succ<x>>, Succ<Add<y, x>>>, Bounded<100>>, Bounded<100>>;
static_assert(peano::evaluate<Q5>());

using Q6 = Forall<x, Eq<Mul<x, Zero>, Zero>, Bounded<100>>;
static_assert(peano::evaluate<Q6>());

using Q7 = Forall<x, Forall<y, Eq<Mul<y, Succ<x>>, Add<Mul<y, x>, y>>, Bounded<100>>, Bounded<100>>;
static_assert(peano::evaluate<Q7>());

using Q8 =
    Forall<x,
           Forall<y, Iff<Lt<x, y>, Exists<z, Eq<Add<Succ<x>, z>, y>, Bounded<100>>>, Bounded<100>>,
           Bounded<100>>;
static_assert(peano::evaluate<Q8>());

auto main(int argc, char* argv[]) -> int {
  (void)argc;
  (void)argv;
  return 0;
}