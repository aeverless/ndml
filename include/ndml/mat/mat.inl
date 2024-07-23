#include "ndml/meta/functional.hpp"

#include <algorithm>

namespace ndml
{
template <std::size_t R, std::size_t C, typename T>
template <typename... FromTs>
constexpr mat<R, C, T>::mat(FromTs... columns) noexcept
	requires (std::same_as<FromTs, column_type> && ...)
	: columns_{std::move(columns)...}
{
}

template <std::size_t R, std::size_t C, typename T>
template <typename FromT>
constexpr mat<R, C, T>::mat(FromT const& scale) noexcept
	requires std::convertible_to<FromT, value_type>
{
	for (std::size_t i = 0; i < column_count; ++i)
	{
		(*this)[i, i] = static_cast<value_type>(scale);
	}
}

template <std::size_t R, std::size_t C, typename T>
template <std::size_t FromR, std::size_t FromC, typename FromT>
constexpr mat<R, C, T>::mat(mat<FromR, FromC, FromT> const& m) noexcept
	requires (FromR <= R && FromC <= C && std::convertible_to<FromT, value_type>)
{
	for (std::size_t i = 0; i < m.column_count; ++i)
	{
		(*this)[i] = static_cast<column_type>(m[i]);
	}
}

template <std::size_t R, std::size_t C, typename T>
template <std::size_t FromR, std::size_t FromC, typename FromT>
constexpr mat<R, C, T>::mat(mat<FromR, FromC, FromT>&& m) noexcept
	requires (FromR <= R && FromC <= C && std::convertible_to<FromT, value_type>)
{
	for (std::size_t i = 0; i < m.column_count; ++i)
	{
		(*this)[i] = static_cast<column_type>(std::move(m[i]));
	}
}

template <std::size_t R, std::size_t C, typename T>
consteval auto mat<R, C, T>::size() noexcept -> std::size_t
{
	return C;
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto mat<R, C, T>::operator[](this auto&& self, std::size_t column) noexcept -> decltype(auto)
{
	return self.columns_[column];
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto mat<R, C, T>::operator[](this auto&& self, std::size_t column, std::size_t row) noexcept -> decltype(auto)
{
	return self[column][row];
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto mat<R, C, T>::begin(this auto&& self) noexcept -> decltype(auto)
{
	return self.columns_.begin();
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto mat<R, C, T>::end(this auto&& self) noexcept -> decltype(auto)
{
	return self.columns_.end();
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto mat<R, C, T>::cbegin(this auto const& self) noexcept -> decltype(auto)
{
	return self.columns_.cbegin();
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto mat<R, C, T>::cend(this auto const& self) noexcept -> decltype(auto)
{
	return self.columns_.cend();
}

template <std::size_t R, std::size_t C, typename T>
constexpr auto swap(mat<R, C, T>& lhs, mat<R, C, T>& rhs) noexcept -> void
{
	std::ranges::swap_ranges(lhs, rhs);
}
}
