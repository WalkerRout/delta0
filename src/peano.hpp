#ifndef PEANO_HPP
#define PEANO_HPP

#include <array>
#include <cstdint>

namespace peano {

namespace details {

namespace binding {

struct unbounded_t {};

template <std::size_t Bound> struct bounded_t {
  static constexpr auto bound = Bound;
};

} // namespace binding

namespace syntax {

// non-logical symbols

struct const_zero_t {};

template <typename L> struct fn_unary_t {};

template <typename L, typename R> struct fn_binary_1_t {};

template <typename L, typename R> struct fn_binary_2_t {};

template <typename L, typename R> struct rel_binary_t {};

// logical symbols

// label is user facing id...
template <std::size_t Label> struct variable_t {};

template <typename L, typename R> struct equality_t {};

template <typename L> struct negation_t {};

template <typename L, typename R> struct conjunction_t {};

template <typename L, typename R> struct disjunction_t {};

template <typename L, typename R> struct conditional_t {};

template <typename L, typename R> struct biconditional_t {};

template <typename Var, typename Body, typename Binding = binding::unbounded_t> struct forall_t {};

template <typename Var, typename Body, typename Binding = binding::unbounded_t> struct exists_t {};

} // namespace syntax

namespace semantics {

// maps variable labels to de bruijn indices
template <std::size_t... Labels> struct binding_context_t {};

// find de bruijn index for label in context
template <std::size_t Label, typename Context> struct find_index;

// base case, label not found, should never happen in a wff
template <std::size_t Label> struct find_index<Label, binding_context_t<>> {
  static constexpr std::size_t value = SIZE_MAX; // dummy value to hopefully crash
};

// recursive case, search through context...
template <std::size_t Label, std::size_t First, std::size_t... Rest>
struct find_index<Label, binding_context_t<First, Rest...>> {
  static constexpr std::size_t value =
      (Label == First) ? 0 : (1 + find_index<Label, binding_context_t<Rest...>>::value);
};

// prepend new binding to context
template <std::size_t Label, typename Context> struct push_binding;

template <std::size_t Label, std::size_t... Labels>
struct push_binding<Label, binding_context_t<Labels...>> {
  using type = binding_context_t<Label, Labels...>;
};

// de bruijn binding stack
template <std::size_t Capacity> class environment_t {
public:
  using value_type = std::size_t;

  explicit constexpr environment_t() noexcept = default;

  [[nodiscard]] constexpr auto lookup(const std::size_t k) const noexcept -> value_type {
    return m_data[m_depth - 1 - k];
  }

  constexpr auto bind(const value_type val) noexcept -> environment_t& {
    m_data[m_depth] = val;
    m_depth += 1;
    return *this;
  }

  constexpr auto unbind() noexcept -> environment_t& {
    m_depth -= 1;
    return *this;
  }

private:
  std::array<value_type, Capacity> m_data;
  std::size_t m_depth;
};

template <typename Term, typename Context, std::size_t Depth> struct interpret_term;
template <typename Formula, typename Context, std::size_t Depth> struct interpret_formula;

// standard model non-logical symbols

template <typename Context, std::size_t Depth>
struct interpret_term<syntax::const_zero_t, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>&) noexcept -> std::size_t {
    return 0;
  }
};

template <typename L, typename Context, std::size_t Depth>
struct interpret_term<syntax::fn_unary_t<L>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> std::size_t {
    return interpret_term<L, Context, Depth>::eval(env) + 1;
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_term<syntax::fn_binary_1_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> std::size_t {
    return interpret_term<L, Context, Depth>::eval(env) +
           interpret_term<R, Context, Depth>::eval(env);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_term<syntax::fn_binary_2_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> std::size_t {
    return interpret_term<L, Context, Depth>::eval(env) *
           interpret_term<R, Context, Depth>::eval(env);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_formula<syntax::rel_binary_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return interpret_term<L, Context, Depth>::eval(env) <
           interpret_term<R, Context, Depth>::eval(env);
  }
};

// standard model logical symbols

template <std::size_t Label, typename Context, std::size_t Depth>
struct interpret_term<syntax::variable_t<Label>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> std::size_t {
    return env.lookup(find_index<Label, Context>::value);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_formula<syntax::equality_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return interpret_term<L, Context, Depth>::eval(env) ==
           interpret_term<R, Context, Depth>::eval(env);
  }
};

template <typename L, typename Context, std::size_t Depth>
struct interpret_formula<syntax::negation_t<L>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return !interpret_formula<L, Context, Depth>::eval(env);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_formula<syntax::conjunction_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return interpret_formula<L, Context, Depth>::eval(env) &&
           interpret_formula<R, Context, Depth>::eval(env);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_formula<syntax::disjunction_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return interpret_formula<L, Context, Depth>::eval(env) ||
           interpret_formula<R, Context, Depth>::eval(env);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_formula<syntax::conditional_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return !interpret_formula<L, Context, Depth>::eval(env) ||
           interpret_formula<R, Context, Depth>::eval(env);
  }
};

template <typename L, typename R, typename Context, std::size_t Depth>
struct interpret_formula<syntax::biconditional_t<L, R>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    return interpret_formula<L, Context, Depth>::eval(env) ==
           interpret_formula<R, Context, Depth>::eval(env);
  }
};

template <std::size_t Label, typename Body, typename Context, std::size_t Depth>
struct interpret_formula<syntax::forall_t<syntax::variable_t<Label>, Body, binding::unbounded_t>,
                         Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    using new_context = typename push_binding<Label, Context>::type;
    for (std::size_t x = 0;; ++x) {
      env.bind(x);
      if (!interpret_formula<Body, new_context, Depth>::eval(env)) {
        env.unbind();
        return false;
      }
      env.unbind();
    }
    return true;
  }
};

template <std::size_t Label, typename Body, std::size_t Bound, typename Context, std::size_t Depth>
struct interpret_formula<
    syntax::forall_t<syntax::variable_t<Label>, Body, binding::bounded_t<Bound>>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    using new_context = typename push_binding<Label, Context>::type;
    for (std::size_t x = 0; x < Bound; ++x) {
      env.bind(x);
      if (!interpret_formula<Body, new_context, Depth>::eval(env)) {
        env.unbind();
        return false;
      }
      env.unbind();
    }
    return true;
  }
};

template <std::size_t Label, typename Body, typename Context, std::size_t Depth>
struct interpret_formula<syntax::exists_t<syntax::variable_t<Label>, Body, binding::unbounded_t>,
                         Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    using new_context = typename push_binding<Label, Context>::type;
    for (std::size_t x = 0;; ++x) {
      env.bind(x);
      if (interpret_formula<Body, new_context, Depth>::eval(env)) {
        env.unbind();
        return true;
      }
      env.unbind();
    }
    return false;
  }
};

template <std::size_t Label, typename Body, std::size_t Bound, typename Context, std::size_t Depth>
struct interpret_formula<
    syntax::exists_t<syntax::variable_t<Label>, Body, binding::bounded_t<Bound>>, Context, Depth> {
  static constexpr auto eval(environment_t<Depth>& env) noexcept -> bool {
    using new_context = typename push_binding<Label, Context>::type;
    for (std::size_t x = 0; x < Bound; ++x) {
      env.bind(x);
      if (interpret_formula<Body, new_context, Depth>::eval(env)) {
        env.unbind();
        return true;
      }
      env.unbind();
    }
    return false;
  }
};

} // namespace semantics

template <std::size_t N> struct build_numeral_t {
  using type = details::syntax::fn_unary_t<typename build_numeral_t<N - 1>::type>;
};

template <> struct build_numeral_t<0> {
  using type = details::syntax::const_zero_t;
};

} // namespace details

// evaluate a closed formula under the standard model
template <typename Formula, std::size_t Depth = 8>
[[nodiscard]] consteval auto evaluate() noexcept -> bool {
  details::semantics::environment_t<Depth> env{};
  using empty_context = details::semantics::binding_context_t<>;
  return details::semantics::interpret_formula<Formula, empty_context, Depth>::eval(env);
}

// evaluate with initial bindings
template <typename Formula, std::size_t Depth = 8, typename... Args>
[[nodiscard]] consteval auto evaluate(Args... args) noexcept -> bool {
  details::semantics::environment_t<Depth> env{};
  (env.bind(args), ...);
  using empty_context = details::semantics::binding_context_t<>;
  return details::semantics::interpret_formula<Formula, empty_context, Depth>::eval(env);
}

namespace dsl {

template <std::size_t N> using Bounded = details::binding::bounded_t<N>;

using Unbounded = details::binding::unbounded_t;

// non-logical symbols
using Zero = details::syntax::const_zero_t;

template <typename L> using Succ = details::syntax::fn_unary_t<L>;

template <typename L, typename R> using Add = details::syntax::fn_binary_1_t<L, R>;

template <typename L, typename R> using Mul = details::syntax::fn_binary_2_t<L, R>;

template <typename L, typename R> using Lt = details::syntax::rel_binary_t<L, R>;

template <std::size_t N> using Numeral = typename details::build_numeral_t<N>::type;

// logical symbols
template <std::size_t Label> using Var = details::syntax::variable_t<Label>;

template <typename L, typename R> using Eq = details::syntax::equality_t<L, R>;

template <typename L> using Not = details::syntax::negation_t<L>;

template <typename L, typename R> using And = details::syntax::conjunction_t<L, R>;

template <typename L, typename R> using Or = details::syntax::disjunction_t<L, R>;

template <typename L, typename R> using If = details::syntax::conditional_t<L, R>;

template <typename L, typename R> using Iff = details::syntax::biconditional_t<L, R>;

template <typename Var, typename Body, typename Binding = details::binding::unbounded_t>
using Forall = details::syntax::forall_t<Var, Body, Binding>;

template <typename Var, typename Body, typename Binding = details::binding::unbounded_t>
using Exists = details::syntax::exists_t<Var, Body, Binding>;

} // namespace dsl

} // namespace peano

#endif // PEANO_HPP