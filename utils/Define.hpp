//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_DEFINE_HPP
#define INC_2SHOOT_DEFINE_HPP

#ifdef TO_LITERAL_STRING
#error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

namespace detail {
template<typename F, bool EnableArguments = false, typename... Args>
struct InvokeOnDestruct_argsSelector;

template<typename F, typename... Args>
struct InvokeOnDestruct_argsSelector<F, false, Args...>
{
  std::tuple<Args...> args{};
};

template<typename F, typename... Args>
struct InvokeOnDestruct_argsSelector<F, true, Args...>
{};

}

template<typename F, typename... Args>
requires std::is_invocable_r_v<void, F, Args...>
struct InvokeOnDestruct : detail::InvokeOnDestruct_argsSelector<F, (sizeof...(Args) == 0), Args...>
{
  F called{};

  constexpr InvokeOnDestruct() noexcept = default;

  InvokeOnDestruct(InvokeOnDestruct const&) = delete;

  constexpr InvokeOnDestruct(InvokeOnDestruct&&) noexcept = default;

  constexpr explicit InvokeOnDestruct(F&& inCalled) requires(sizeof...(Args) == 0)
      : called(std::forward<F>(inCalled))
  {}

  constexpr explicit InvokeOnDestruct(F&& inCalled, Args&&... inArgs) noexcept requires(sizeof...(Args) > 0)
      : InvokeOnDestruct::InvokeOnDestruct_argsSelector{ std::make_tuple(std::forward<Args>(inArgs)...) }
      , called(std::forward<F>(inCalled))
  {}

  constexpr void reset(F&& inCalled = {}, Args&&... inArgs) noexcept
  {
    if constexpr (sizeof...(Args) > 0)
      this->args = std::make_tuple(std::forward<Args>(inArgs)...);
    called = std::forward<F>(inCalled);
  }

  ~InvokeOnDestruct()
  {
    if constexpr (sizeof...(Args) > 0)
      std::apply(called, this->args);
    else
      std::invoke(called);
  }
};
#endif // INC_2SHOOT_DEFINE_HPP
