#include "ndml/meta/functional.hpp"

#include <algorithm>
#include <cmath>

namespace ndml
{
template <std::size_t N, typename T, typename UnaryFn>
constexpr auto transform(vec<N, T>& v, UnaryFn const& f) noexcept(noexcept(std::ranges::for_each(v, f))) -> vec<N, T>&
{
	std::ranges::for_each(v, f);
	return v;
}

template <std::size_t N, typename T, typename BinaryFn>
constexpr auto zip_transform(vec<N, T>& lhs, vec<N, T> const& rhs, BinaryFn const& f) noexcept(noexcept(f(lhs[0], rhs[0]))) -> vec<N, T>&
{
	for (std::size_t i = 0; i < N; ++i)
	{
		f(lhs[i], rhs[i]);
	}

	return lhs;
}

template <std::size_t N, typename T>
constexpr auto dot(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>::value_type
{
	typename vec<N, T>::value_type s{};

	for (std::size_t i = 0; i < N; ++i)
	{
		s += lhs[i] * rhs[i];
	}

	return s;
}

template <typename T>
constexpr auto cross(vec<3, T> const& lhs, vec<3, T> const& rhs) noexcept -> vec<3, T>
{
	return {
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x,
	};
}

template <std::size_t N, typename T>
constexpr auto norm_squared(vec<N, T> const& v) noexcept -> vec<N, T>::value_type
{
	return dot(v, v);
}

template <std::size_t N, typename T>
constexpr auto norm(vec<N, T> const& v) noexcept -> double
{
	return std::sqrt(norm_squared(v));
}

template <std::size_t N, typename T>
constexpr auto normal(vec<N, T> const& v) noexcept -> vec<N, T>
{
	return v / norm(v);
}

template <std::size_t N, typename T>
constexpr auto projection(vec<N, T> const& v, vec<N, T> const& axis) noexcept -> vec<N, T>
{
	return axis * dot(v, axis) / dot(axis, axis);
}

template <std::size_t N, typename T>
constexpr auto reciprocal(vec<N, T> const& v) noexcept -> vec<N, T>
{
	auto r{v};
	transform(r, [](T& component) static noexcept { component = static_cast<T>(1) / component; });

	return r;
}

template <std::size_t N, typename T>
constexpr auto operator+=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_transform(lhs, rhs, meta::addition_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator-=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_transform(lhs, rhs, meta::subtraction_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_transform(lhs, rhs, meta::multiplication_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_transform(lhs, rhs, meta::division_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&
{
	return transform(v, [&scale](auto& component) { return component *= scale; });
}

template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&
{
	return transform(v, [&scale](auto& component) { return component /= scale; });
}

template <std::size_t N, typename T>
constexpr auto operator==(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> bool
{
	return std::ranges::equal(lhs, rhs);
}

template <std::size_t N, typename T>
constexpr auto operator!=(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> bool
{
	return !(lhs == rhs);
}

template <std::size_t N, typename T>
constexpr auto operator+(vec<N, T> const& v) noexcept -> vec<N, T>
{
	auto tmp{v};
	return transform(tmp, [](auto& component) { return component = +component; });
}

template <std::size_t N, typename T>
constexpr auto operator-(vec<N, T> const& v) noexcept -> vec<N, T>
{
	auto tmp{v};
	return transform(tmp, [](auto& component) { return component = -component; });
}

template <std::size_t N, typename T>
constexpr auto operator+(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	auto tmp{lhs};
	return tmp += rhs;
}

template <std::size_t N, typename T>
constexpr auto operator-(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	auto tmp{lhs};
	return tmp -= rhs;
}

template <std::size_t N, typename T>
constexpr auto operator*(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	auto tmp{lhs};
	return tmp *= rhs;
}

template <std::size_t N, typename T>
constexpr auto operator/(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	auto tmp{lhs};
	return tmp /= rhs;
}

template <std::size_t N, typename T>
constexpr auto operator*(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>
{
	auto tmp{v};
	return tmp *= scale;
}

template <std::size_t N, typename T>
constexpr auto operator*(typename vec<N, T>::value_type const& scale, vec<N, T> const& v) noexcept -> vec<N, T>
{
	return v * scale;
}

template <std::size_t N, typename T>
constexpr auto operator/(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>
{
	auto tmp{v};
	return tmp /= scale;
}
}
