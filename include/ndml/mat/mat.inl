#include "ndml/meta/functional.hpp"

#include <algorithm>

namespace ndml
{
template <std::size_t C, std::size_t R, typename T>
template <typename... FromTs>
constexpr mat<C, R, T>::mat(FromTs... columns) noexcept
	requires (std::same_as<FromTs, column_type> && ...)
	: columns_{std::move(columns)...}
{
}

template <std::size_t C, std::size_t R, typename T>
template <typename FromT>
constexpr mat<C, R, T>::mat(FromT const& scale) noexcept
	requires std::convertible_to<FromT, value_type>
{
	for (auto i = 0uz; i < column_count; ++i)
	{
		(*this)[i, i] = static_cast<value_type>(scale);
	}
}

template <std::size_t C, std::size_t R, typename T>
template <std::size_t FromC, std::size_t FromR, typename FromT>
constexpr mat<C, R, T>::mat(mat<FromC, FromR, FromT> const& m) noexcept
	requires (FromC <= C && FromR <= R && std::convertible_to<FromT, value_type>)
{
	for (auto i = 0uz; i < m.column_count; ++i)
	{
		(*this)[i] = static_cast<column_type>(m[i]);
	}
}

template <std::size_t C, std::size_t R, typename T>
template <std::size_t FromC, std::size_t FromR, typename FromT>
constexpr mat<C, R, T>::mat(mat<FromC, FromR, FromT>&& m) noexcept
	requires (FromC <= C && FromR <= R && std::convertible_to<FromT, value_type>)
{
	for (auto i = 0uz; i < m.column_count; ++i)
	{
		(*this)[i] = static_cast<column_type>(std::move(m[i]));
	}
}

template <std::size_t C, std::size_t R, typename T>
consteval auto mat<C, R, T>::size() noexcept -> std::size_t
{
	return C;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto mat<C, R, T>::operator[](this auto&& self, std::size_t column) noexcept -> decltype(auto)
{
	assert(column < C);
	return self.columns_[column];
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto mat<C, R, T>::operator[](this auto&& self, std::size_t column, std::size_t row) noexcept -> decltype(auto)
{
	assert(column < C);
	assert(row < R);

	return self[column][row];
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto mat<C, R, T>::begin(this auto&& self) noexcept -> decltype(auto)
{
	return self.columns_.begin();
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto mat<C, R, T>::end(this auto&& self) noexcept -> decltype(auto)
{
	return self.columns_.end();
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto mat<C, R, T>::cbegin(this auto const& self) noexcept -> decltype(auto)
{
	return self.columns_.cbegin();
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto mat<C, R, T>::cend(this auto const& self) noexcept -> decltype(auto)
{
	return self.columns_.cend();
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto swap(mat<C, R, T>& lhs, mat<C, R, T>& rhs) noexcept -> void
{
	std::ranges::swap_ranges(lhs, rhs);
}
}
