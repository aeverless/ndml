export module ndml.meta:functional;

export namespace ndml::meta
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs = rhs)) -> decltype(auto)
	{
		return lhs = rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs += rhs)) -> decltype(auto)
	{
		return lhs += rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs -= rhs)) -> decltype(auto)
	{
		return lhs -= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs *= rhs)) -> decltype(auto)
	{
		return lhs *= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs /= rhs)) -> decltype(auto)
	{
		return lhs /= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs %= rhs)) -> decltype(auto)
	{
		return lhs %= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs &= rhs)) -> decltype(auto)
	{
		return lhs &= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs |= rhs)) -> decltype(auto)
	{
		return lhs |= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs ^= rhs)) -> decltype(auto)
	{
		return lhs ^= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs <<= rhs)) -> decltype(auto)
	{
		return lhs <<= rhs;
	}
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
	static constexpr auto operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs >>= rhs)) -> decltype(auto)
	{
		return lhs >>= rhs;
	}
};
}
