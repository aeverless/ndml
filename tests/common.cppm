module;

#include "deps.def"

export module ndml.tests:common;

import ndml;
import std;

using ndml::mat;
using ndml::quat;
using ndml::vec;

template <typename T>
using vec1 = vec<1, T>;

template <typename T>
using vec2 = vec<2, T>;

template <typename T>
using vec3 = vec<3, T>;

template <typename T>
using vec4 = vec<4, T>;

template <typename T>
using mat2x2 = mat<2, 2, T>;

template <typename T>
using mat3x3 = mat<3, 3, T>;

template <typename T>
using mat4x4 = mat<4, 4, T>;

template <typename T>
constexpr auto eps = std::numeric_limits<T>::epsilon();

class TypeBoundMatcher : public Catch::Matchers::MatcherGenericBase
{
public:
	constexpr TypeBoundMatcher(long double expected, long double tolerance, bool relative) noexcept
		: expected_{expected}
		, tolerance_{tolerance}
		, relative_{relative}
	{}

	template <typename T>
	[[nodiscard]]
	constexpr auto match(T const& actual) const noexcept -> bool
	{
		auto const value = static_cast<long double>(actual);
		if (value == expected_)
		{
			return true;
		}

		auto tolerance = tolerance_;
		if (relative_)
		{
			tolerance *= std::max({1.0L, std::abs(value), std::abs(expected_)});
		}

		return std::abs(value - expected_) <= tolerance;
	}

	[[nodiscard]]
	auto describe() const -> std::string override
	{
		return relative_ ? "is within a type-scaled relative tolerance" : "is within a type-scaled absolute tolerance";
	}

private:
	long double expected_;
	long double tolerance_;
	bool        relative_;
};

template <typename T = double, typename R = T>
[[nodiscard]]
auto WithinRelTypeBound(R const& value) noexcept
{
	using tolerance_type = std::conditional_t<std::floating_point<T>, T, double>;
	auto const tolerance = static_cast<long double>(100) * static_cast<long double>(eps<tolerance_type>);

	return TypeBoundMatcher{static_cast<long double>(static_cast<T>(value)), tolerance, true};
}

template <typename T = double, typename R = T>
[[nodiscard]]
auto WithinAbsTypeBound(R const& value) noexcept
{
	using tolerance_type = std::conditional_t<std::floating_point<T>, T, double>;
	auto const tolerance = static_cast<long double>(100) * static_cast<long double>(eps<tolerance_type>);

	return TypeBoundMatcher{static_cast<long double>(static_cast<T>(value)), tolerance, false};
}

[[nodiscard]]
constexpr auto mix_seed(std::uint64_t value) noexcept -> std::uint64_t
{
	value += 0x9E'37'79'B9'7F'4A'7C'15ULL;
	value  = (value ^ (value >> 30)) * 0xBF'58'47'6D'1C'E4'E5'B9ULL;
	value  = (value ^ (value >> 27)) * 0x94'D0'49'BB'13'31'11'EBULL;
	return value ^ (value >> 31);
}

[[nodiscard]]
auto source_seed(std::source_location const& location) noexcept -> std::uint64_t
{
	auto seed = static_cast<std::uint64_t>(std::hash<std::string_view>{}(location.file_name()));
	seed     ^= static_cast<std::uint64_t>(location.line()) << 32;
	seed     ^= static_cast<std::uint64_t>(location.column());
	return mix_seed(seed);
}

template <typename T, std::size_t Takes = 16, bool DisallowZero = true, double MaxAbsValue = 100.0>
[[nodiscard]]
auto generate_number(std::uint64_t salt = 0, std::source_location const& location = std::source_location::current()) -> T
{
	auto const generated = GENERATE(take(Takes, random(std::uint32_t{0}, std::numeric_limits<std::uint32_t>::max() - 1)));
	auto const bits      = mix_seed(static_cast<std::uint64_t>(generated) ^ mix_seed(salt) ^ source_seed(location));

	static constexpr auto requested_max = static_cast<long double>(MaxAbsValue);
	auto const            lower         = std::max(static_cast<long double>(std::numeric_limits<T>::lowest()), -requested_max);
	auto const            upper         = std::min(static_cast<long double>(std::numeric_limits<T>::max()), requested_max);

	auto value = T{};
	if constexpr (std::integral<T>)
	{
		auto const width = static_cast<std::uint64_t>(upper - lower + 1);
		value            = static_cast<T>(lower + static_cast<long double>(bits % width));
	}
	else
	{
		static constexpr auto uint64_range = static_cast<long double>(std::numeric_limits<std::uint64_t>::max());

		auto const unit = static_cast<long double>(bits) / uint64_range;
		value           = static_cast<T>(lower + (upper - lower) * unit);
	}

	if constexpr (DisallowZero)
	{
		if (value == T{})
		{
			return T{1};
		}
	}

	return value;
}

template <std::size_t N, typename T, std::size_t Takes = 16, bool DisallowZeroes = true, double MaxAbsValue = 100.0>
[[nodiscard]]
auto generate_vec(std::uint64_t salt = 0, std::source_location const& location = std::source_location::current()) -> vec<N, T>
{
	auto const seed = mix_seed(salt ^ source_seed(location));

	if constexpr (N == 1)
	{
		return vec<N, T>{generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed)};
	}
	else if constexpr (N == 2)
	{
		return vec<N, T>{
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed + 1)
		};
	}
	else if constexpr (N == 3)
	{
		return vec<N, T>{
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed + 1),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed + 2)
		};
	}
	else
	{
		return vec<N, T>{
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed + 1),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed + 2),
			generate_number<T, Takes, DisallowZeroes, MaxAbsValue>(seed + 3)
		};
	}
}

template <std::size_t C, std::size_t R, typename T, std::size_t Takes = 16, bool DisallowZeroes = true, double MaxAbsValue = 100.0>
[[nodiscard]]
auto generate_mat(std::uint64_t salt = 0, std::source_location const& location = std::source_location::current()) -> mat<C, R, T>
{
	auto const seed = mix_seed(salt ^ source_seed(location));

	if constexpr (C == 1)
	{
		return mat<C, R, T>{generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed)};
	}
	else if constexpr (C == 2)
	{
		return mat<C, R, T>{
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed + 1)
		};
	}
	else if constexpr (C == 3)
	{
		return mat<C, R, T>{
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed + 1),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed + 2)
		};
	}
	else
	{
		return mat<C, R, T>{
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed + 1),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed + 2),
			generate_vec<R, T, Takes, DisallowZeroes, MaxAbsValue>(seed + 3)
		};
	}
}

template <typename L, typename R>
	requires (!std::ranges::range<L> && !std::ranges::range<R>)
[[nodiscard]]
constexpr auto equal(L const& lhs, R const& rhs) -> bool
{
	using common_type = std::common_type_t<L, R>;

	if constexpr (!std::floating_point<common_type>)
	{
		return lhs == rhs;
	}
	else
	{
		auto const left      = static_cast<long double>(lhs);
		auto const right     = static_cast<long double>(rhs);
		auto const tolerance = 100.0L * static_cast<long double>(eps<common_type>);

		return std::abs(left - right) <= tolerance * std::max({1.0L, std::abs(left), std::abs(right)});
	}
}

template <std::ranges::range L, std::ranges::range R>
[[nodiscard]]
constexpr auto equal(L const& lhs, R const& rhs) -> bool
{
	return std::ranges::equal(
		lhs,
		rhs,
		[](auto const& left, auto const& right)
		{
			return equal(left, right);
		}
	);
}
