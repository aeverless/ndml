#ifndef NDML_META_FUNCTIONAL_HPP
#define NDML_META_FUNCTIONAL_HPP

namespace ndml::meta
{
/**
 * @brief Copy assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct copy_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs = rhs)) -> decltype(auto);
};

/**
 * @brief Addition assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct addition_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs += rhs)) -> decltype(auto);
};

/**
 * @brief Subtraction assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct subtraction_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs -= rhs)) -> decltype(auto);
};

/**
 * @brief Multiplication assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct multiplication_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs *= rhs)) -> decltype(auto);
};

/**
 * @brief Division assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct division_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs /= rhs)) -> decltype(auto);
};

/**
 * @brief Remainder assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct remainder_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs %= rhs)) -> decltype(auto);
};

/**
 * @brief Bitwise AND assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct bit_and_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs &= rhs)) -> decltype(auto);
};

/**
 * @brief Bitwise OR assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct bit_or_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs |= rhs)) -> decltype(auto);
};

/**
 * @brief Bitwise XOR assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct bit_xor_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs ^= rhs)) -> decltype(auto);
};

/**
 * @brief Left shift assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct left_shift_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs <<= rhs)) -> decltype(auto);
};

/**
 * @brief Right shift assignment functor.
 *
 * @tparam T type of left-side parameter
 * @tparam U type of right-side parameter
 */
template <typename T, typename U = T>
struct right_shift_assignment
{
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs >>= rhs)) -> decltype(auto);
};
}

#include "functional.inl"

#endif
