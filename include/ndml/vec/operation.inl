#include "ndml/meta/functional.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <ranges>

namespace ndml
{
template <std::size_t N, typename T, typename UnaryFn>
constexpr auto for_each(vec<N, T>& v, UnaryFn const& f) noexcept(noexcept(std::ranges::for_each(v, f))) -> vec<N, T>&
{
	std::ranges::for_each(v, f);
	return v;
}

template <std::size_t N, typename T, typename BinaryFn>
constexpr auto zip_for_each(vec<N, T>& lhs, vec<N, T> const& rhs, BinaryFn const& f) noexcept(noexcept(f(lhs[0], rhs[0]))) -> vec<N, T>&
{
	for (auto i = 0uz; i < N; ++i)
	{
		f(lhs[i], rhs[i]);
	}

	return lhs;
}

template <std::size_t N, typename T, typename UnaryFn>
constexpr auto transform(vec<N, T> const& v, UnaryFn const& f) noexcept(noexcept(f(v[0]))) -> vec<N, T>
{
	vec<N, T> result;

	for (auto i = 0uz; i < N; ++i)
	{
		result[i] = f(v[i]);
	}

	return result;
}

template <std::size_t N, typename T, typename BinaryFn>
constexpr auto zip_transform(vec<N, T> const& lhs, vec<N, T> const& rhs, BinaryFn const& f) noexcept(noexcept(f(lhs[0], rhs[0]))) -> vec<N, T>
{
	vec<N, T> result;

	for (auto i = 0uz; i < N; ++i)
	{
		result[i] = f(lhs[i], rhs[i]);
	}

	return result;
}

template <std::size_t N, typename T>
constexpr auto dot(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>::value_type
{
	typename vec<N, T>::value_type s{};

	for (auto i = 0uz; i < N; ++i)
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
	return std::sqrt(static_cast<double>(v.x) * v.x + static_cast<double>(v.y) * v.y + static_cast<double>(v.z) * v.z);
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
	return transform(v, [](T component) static noexcept { return static_cast<T>(1) / component; });
}

template <std::size_t N, typename T>
constexpr auto operator+=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::addition_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator-=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::subtraction_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::multiplication_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>&
{
	return zip_for_each(lhs, rhs, meta::division_assignment<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator*=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&
{
	return for_each(v, [&scale](auto& component) { component *= scale; });
}

template <std::size_t N, typename T>
constexpr auto operator/=(vec<N, T>& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>&
{
	assert(N != 0);
	return for_each(v, [&scale](auto& component) { component /= scale; });
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
	return transform(v, [](auto component) { return component = +component; });
}

template <std::size_t N, typename T>
constexpr auto operator-(vec<N, T> const& v) noexcept -> vec<N, T>
{
	return transform(v, [](auto component) { return component = -component; });
}

template <std::size_t N, typename T>
constexpr auto operator+(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::plus<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator-(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::minus<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator*(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::multiplies<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator/(vec<N, T> const& lhs, vec<N, T> const& rhs) noexcept -> vec<N, T>
{
	return zip_transform(lhs, rhs, std::divides<T>{});
}

template <std::size_t N, typename T>
constexpr auto operator*(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>
{
	return transform(v, [scale](T component) { return component * scale; });
}

template <std::size_t N, typename T>
constexpr auto operator*(typename vec<N, T>::value_type const& scale, vec<N, T> const& v) noexcept -> vec<N, T>
{
	return v * scale;
}

template <std::size_t N, typename T>
constexpr auto operator/(vec<N, T> const& v, typename vec<N, T>::value_type const& scale) noexcept -> vec<N, T>
{
	return transform(v, [scale](T component) { return component / scale; });
}
}
