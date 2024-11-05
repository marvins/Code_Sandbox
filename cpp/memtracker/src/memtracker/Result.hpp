/**
 * @file    Result.hpp
 * @author  Marvin Smith
 * @date    11/3/2024
 * 
 * Shamelessly lifted from Yegor.  I'm a fan of his work.
 * https://yegor.pomortsev.com/post/result-type/
 */
#pragma once

// C++ Standard Libraries
#include <variant>

namespace mem {

enum class StatusType {
  SUCCESS = 0,
  UNINITIALIZED = 1,
};

/**
 * @class Ok
 * Return type if there was no error
 */
template <typename T>
class Ok {
  public:
    explicit constexpr Ok(T value) : value(std::move(value)) {}

    constexpr T&& take_value() { return std::move(value); }

    T value;
};

/**
 * @class Err
 * Unique error type
 */
template <typename T>
class Err {
  public:
    explicit constexpr Err(T value) : value(std::move(value)) {}

    constexpr T&& take_value() { return std::move(value); }

    T value;
};

/**
 * General-purpose result type.
 */
template <typename OkT, typename ErrT>
class Result {
  public:
    using VariantT = std::variant<Ok<OkT>, Err<ErrT>>;

    constexpr Result(Ok<OkT> value) : variant(std::move(value)) {}
    constexpr Result(Err<ErrT> value) : variant(std::move(value)) {}

    constexpr bool is_ok() const { return std::holds_alternative<Ok<OkT>>(variant); }
    constexpr bool is_err() const { return std::holds_alternative<Err<ErrT>>(variant); }

    constexpr OkT ok_value() const { return std::get<Ok<OkT>>(variant).value; }
    constexpr ErrT err_value() const { return std::get<Err<ErrT>>(variant).value; }

    constexpr OkT&& take_ok_value() { return std::get<Ok<OkT>>(variant).take_value(); }
    constexpr ErrT&& take_err_value() { return std::get<Err<ErrT>>(variant).take_value(); }

    VariantT variant;
}; // End of Result Type

} // End of mem namespace