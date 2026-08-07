#ifndef NDML_TESTS_COMMON_HPP
#define NDML_TESTS_COMMON_HPP

#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers.hpp"
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_range.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <limits>
#include <ranges>

#include "ndml/vec.hpp"
#include "ndml/mat.hpp"
#include "ndml/quat.hpp"

#define NDML_TEST_TYPES std::uint8_t, std::int8_t, std::uint16_t, std::int16_t, std::uint32_t, std::int32_t, std::uint64_t, std::int64_t, float, double, long double

using ndml::vec;
using ndml::mat;
using ndml::quat;

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

template <typename T = double, typename R = double>
[[nodiscard]]
auto WithinRelTypeBound(T const& value) noexcept;

template <typename T, std::size_t Takes = 16, bool DisallowZero = true, double MaxAbsValue = 1e6>
[[nodiscard]]
constexpr auto generate_number() -> T;

template <std::size_t N, typename T, std::size_t Takes = 16, bool DisallowZeroes = true, double MaxAbsValue = 1e6>
[[nodiscard]]
constexpr auto generate_vec() -> vec<N, T>;

template <std::size_t C, std::size_t R, typename T, std::size_t Takes = 16, bool DisallowZeroes = true, double MaxAbsValue = 1e6>
[[nodiscard]]
constexpr auto generate_mat() -> mat<C, R, T>;

template <typename L, typename R>
[[nodiscard]]
constexpr auto equal(L const& lhs, R const& r) -> bool;

#include "common.inl"

#endif
