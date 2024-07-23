#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace ndml
{
template <std::size_t N, typename T>
template <typename... FromTs>
constexpr vec<N, T>::vec(FromTs... components) noexcept
	requires (sizeof...(FromTs) <= N && (std::convertible_to<FromTs, value_type> && ...))
{
	std::size_t i = 0;
	(((*this)[i++] = std::move(components)), ...);
}

template <std::size_t N, typename T>
template <typename FromT>
constexpr vec<N, T>::vec(FromT const& scale) noexcept
	requires std::convertible_to<FromT, value_type>
{
	std::ranges::fill(*this, static_cast<value_type>(scale));
}

template <std::size_t N, typename T>
template <std::size_t FromN, typename FromT>
constexpr vec<N, T>::vec(vec<FromN, FromT> const& v) noexcept
	requires (FromN <= N && std::convertible_to<FromT, value_type>)
{
	for (std::size_t i = 0; i < FromN; ++i)
	{
		(*this)[i] = static_cast<value_type>(v[i]);
	}
}

template <std::size_t N, typename T>
template <std::size_t FromN, typename FromT>
constexpr vec<N, T>::vec(vec<FromN, FromT>&& v) noexcept
	requires (FromN <= N && std::convertible_to<FromT, value_type>)
{
	for (std::size_t i = 0; i < FromN; ++i)
	{
		(*this)[i] = static_cast<value_type>(std::move(v[i]));
	}
}

template <std::size_t N, typename T>
consteval auto vec<N, T>::size() noexcept -> std::size_t
{
	return N;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::operator[](this V&& self, std::size_t i) -> subscript_result<V>
{
	switch (i)
	{
	case 0:
		if constexpr (0 < N)
		{
			return self.x;
		}
		else
		{
			[[fallthrough]];
		}

	case 1:
		if constexpr (1 < N)
		{
			return self.y;
		}
		else
		{
			[[fallthrough]];
		}

	case 2:
		if constexpr (2 < N)
		{
			return self.z;
		}
		else
		{
			[[fallthrough]];
		}

	case 3:
		if constexpr (3 < N)
		{
			return self.w;
		}
		else
		{
			[[fallthrough]];
		}

	default:
		throw std::out_of_range("index out of range in call to vec subscript operator");
	}
}

template <std::size_t N, typename T>
constexpr vec<N, T>::operator bool(this auto const& self) noexcept
{
	return std::ranges::any_of(self, std::identity{});
}

template <std::size_t N, typename T>
constexpr auto vec<N, T>::begin(this auto&& self) noexcept -> decltype(auto)
{
	return iterator(self, 0);
}

template <std::size_t N, typename T>
constexpr auto vec<N, T>::end(this auto&& self) noexcept -> decltype(auto)
{
	return iterator(self, N);
}

template <std::size_t N, typename T>
constexpr auto vec<N, T>::cbegin(this auto const& self) noexcept -> decltype(auto)
{
	return self.begin();
}

template <std::size_t N, typename T>
constexpr auto vec<N, T>::cend(this auto const& self) noexcept -> decltype(auto)
{
	return self.end();
}

template <std::size_t N, typename T>
constexpr auto swap(vec<N, T>& lhs, vec<N, T>& rhs) noexcept -> void
{
	std::ranges::swap_ranges(lhs, rhs);
}

template <std::size_t N, typename T>
template <typename V>
constexpr vec<N, T>::iterator<V>::iterator(V& v, std::size_t pos) noexcept
	: v_{&v}
	, pos_{pos}
{
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator*(this auto&& self) -> decltype(auto)
{
	return (*self.v_)[self.pos_];
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator++(this iterator& self) noexcept -> iterator&
{
	++self.pos_;
	return self;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator++(this iterator& self, int) noexcept -> iterator
{
	auto tmp{self};
	++tmp;
	return tmp;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator--(this iterator& self) noexcept -> iterator&
{
	--self.pos_;
	return self;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator--(this iterator& self, int) noexcept -> iterator
{
	auto tmp{self};
	--tmp;
	return tmp;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator+=(this iterator& self, difference_type inc) noexcept -> iterator&
{
	self.pos_ += inc;
	return self;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator-=(this iterator& self, difference_type dec) noexcept -> iterator&
{
	self.pos_ -= dec;
	return self;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator+(this iterator const& self, difference_type inc) noexcept -> iterator
{
	auto tmp{self};
	tmp += inc;
	return tmp;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator-(this iterator const& self, difference_type dec) noexcept -> iterator
{
	auto tmp{self};
	tmp -= dec;
	return tmp;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator==(this iterator const& self, iterator const& other) noexcept -> bool
{
	return self.v_ == other.v_ && self.pos_ == other.pos_;
}

template <std::size_t N, typename T>
template <typename V>
constexpr auto vec<N, T>::iterator<V>::operator!=(this iterator const& self, iterator const& other) noexcept -> bool
{
	return !(self == other);
}
}
