namespace ndml::meta
{
template <typename T, typename U>
constexpr auto copy_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs = rhs)) -> decltype(auto)
{
	return lhs = rhs;
}

template <typename T, typename U>
constexpr auto addition_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs += rhs)) -> decltype(auto)
{
	return lhs += rhs;
}

template <typename T, typename U>
constexpr auto subtraction_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs -= rhs)) -> decltype(auto)
{
	return lhs -= rhs;
}

template <typename T, typename U>
constexpr auto multiplication_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs *= rhs)) -> decltype(auto)
{
	return lhs *= rhs;
}

template <typename T, typename U>
constexpr auto division_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs /= rhs)) -> decltype(auto)
{
	return lhs /= rhs;
}

template <typename T, typename U>
constexpr auto remainder_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs %= rhs)) -> decltype(auto)
{
	return lhs %= rhs;
}

template <typename T, typename U>
constexpr auto bit_and_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs &= rhs)) -> decltype(auto)
{
	return lhs &= rhs;
}

template <typename T, typename U>
constexpr auto bit_or_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs |= rhs)) -> decltype(auto)
{
	return lhs |= rhs;
}

template <typename T, typename U>
constexpr auto bit_xor_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs ^= rhs)) -> decltype(auto)
{
	return lhs ^= rhs;
}

template <typename T, typename U>
constexpr auto left_shift_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs <<= rhs)) -> decltype(auto)
{
	return lhs <<= rhs;
}

template <typename T, typename U>
constexpr auto right_shift_assignment<T, U>::operator()(T& lhs, U const& rhs) noexcept(noexcept(lhs >>= rhs)) -> decltype(auto)
{
	return lhs >>= rhs;
}
}
