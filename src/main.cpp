#include <cstdio>

#include "peano.hpp"

using namespace peano::dsl;

using Q1 =
    Forall<Var<0>,
           Forall<Var<1>, If<Eq<Succ<Var<0>>, Succ<Var<1>>>, Eq<Var<0>, Var<1>>>, Bounded<100>>,
           Bounded<100>>;
static_assert(peano::evaluate<Q1>());

using Q2 = Forall<Var<0>, Not<Eq<Zero, Succ<Var<0>>>>, Bounded<100>>;
static_assert(peano::evaluate<Q2>());

using Q3 =
    Forall<Var<0>, Or<Eq<Var<0>, Zero>, Exists<Var<1>, Eq<Var<1>, Succ<Var<0>>>, Bounded<100>>>,
           Bounded<100>>;
static_assert(peano::evaluate<Q3>());

using Q4 = Forall<Var<0>, Eq<Add<Var<0>, Zero>, Var<0>>, Bounded<100>>;
static_assert(peano::evaluate<Q4>());

using Q5 =
    Forall<Var<0>,
           Forall<Var<1>, Eq<Add<Var<1>, Succ<Var<0>>>, Succ<Add<Var<1>, Var<0>>>>, Bounded<100>>,
           Bounded<100>>;
static_assert(peano::evaluate<Q5>());

using Q6 = Forall<Var<0>, Eq<Mul<Var<0>, Zero>, Zero>, Bounded<100>>;
static_assert(peano::evaluate<Q6>());

using Q7 = Forall<
    Var<0>,
    Forall<Var<1>, Eq<Mul<Var<1>, Succ<Var<0>>>, Add<Mul<Var<1>, Var<0>>, Var<1>>>, Bounded<100>>,
    Bounded<100>>;
static_assert(peano::evaluate<Q7>());

using Q8 = Forall<Var<0>,
                  Forall<Var<1>,
                         Iff<Lt<Var<1>, Var<0>>,
                             Exists<Var<2>, Eq<Add<Succ<Var<0>>, Var<2>>, Var<1>>, Bounded<100>>>,
                         Bounded<100>>,
                  Bounded<100>>;
static_assert(peano::evaluate<Q8>());

auto main(int argc, char* argv[]) -> int {
  (void)argc;
  (void)argv;
  return 0;
}