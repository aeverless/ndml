#include <algorithm>
#include <ranges>


template <typename T, typename R>
auto WithinRelTypeBound(T const& value) noexcept
{
	return Catch::Matchers::WithinRel(R(value), 1e9 * eps<double>);
}

template <typename T, std::size_t Takes, bool DisallowZero, double MaxAbsValue>
constexpr auto generate_number() -> T
{
	auto const value = static_cast<T>(GENERATE(take(Takes, random(-MaxAbsValue, MaxAbsValue))));

	if constexpr (DisallowZero)
	{
		if (WithinRelTypeBound(0.0).match(value) || !value)
		{
			return static_cast<T>(value + 1);
		}
	}

	return value;
}

template <std::size_t N, typename T, std::size_t Takes, bool DisallowZeroes, double MaxAbsValue>
constexpr auto generate_vec() -> vec<N, T>
{
	if constexpr (N == 1)
	{
		return vec<N, T>{generate_number<T, Takes, DisallowZeroes, MaxAbsValue>()};
	}
	else if constexpr (N == 2)
	{
		return vec<N, T>{generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(), generate_number<T, Takes, DisallowZeroes, MaxAbsValue>()};
	}
	else if constexpr (N == 3)
	{
		return vec<N, T>{
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>()
		};
	}
	else
	{
		return vec<N, T>{
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>()
		};
	}
}

template <std::size_t C, std::size_t R, typename T, std::size_t Takes, bool DisallowZeroes, double MaxAbsValue>
constexpr auto generate_mat() -> mat<C, R, T>
{
	if constexpr (C == 1)
	{
		return mat<C, R, T>{generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>()};
	}
	else if constexpr (C == 2)
	{
		return mat<C, R, T>{generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(), generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>()};
	}
	else if constexpr (C == 3)
	{
		return mat<C, R, T>{
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>()
		};
	}
	else
	{
		return mat<C, R, T>{
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>()
		};
	}
}

template <typename L, typename R>
constexpr auto equal(L const& lhs, R const& rhs) -> bool
{
	return WithinRelTypeBound<R>(lhs).match(rhs);
}

template <std::ranges::range L, std::ranges::range R>
constexpr auto equal(L const& lhs, R const& rhs) -> bool
{
	return std::ranges::all_of(
		std::views::zip(lhs, rhs),
		[](auto&& p) static noexcept -> bool
		{
			return equal(std::get<0>(p), std::get<1>(p));
		}
	);
}

