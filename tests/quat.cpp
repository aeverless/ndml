#include "common.hpp"
#include "ndml/mat/transform.hpp"
#include "ndml/quat/operation.hpp"
#include "ndml/vec/vec.hpp"
#include <concepts>
#include <numbers>

namespace
{

TEMPLATE_TEST_CASE("construction from vector", "[quat]", NDML_TEST_TYPES)
{
	auto const from = generate_vec<4, TestType>();
	auto const q    = quat{from};

	REQUIRE(q.x == from.x);
	REQUIRE(q.y == from.y);
	REQUIRE(q.z == from.z);
	REQUIRE(q.w == from.w);
}

TEMPLATE_TEST_CASE("construction from 3-vector and scalar", "[quat]", NDML_TEST_TYPES)
{
	auto const v = generate_vec<3, TestType>();
	auto const w = generate_number<TestType>();

	auto const q = quat{v, w};

	REQUIRE(q.x == v.x);
	REQUIRE(q.y == v.y);
	REQUIRE(q.z == v.z);
	REQUIRE(q.w == w);
}

TEMPLATE_TEST_CASE("construction from components", "[quat]", NDML_TEST_TYPES)
{
	auto const v = generate_vec<4, TestType>();
	auto const q = quat{v.x, v.y, v.z, v.w};

	REQUIRE(q.x == v.x);
	REQUIRE(q.y == v.y);
	REQUIRE(q.z == v.z);
	REQUIRE(q.w == v.w);
}

TEMPLATE_TEST_CASE("conjugate", "[quat][conj]", NDML_TEST_TYPES)
{
	auto const q = quat{generate_vec<4, TestType>()};
	auto const c = conjugate(q);

	REQUIRE(q.x == static_cast<TestType>(-c.x));
	REQUIRE(q.y == static_cast<TestType>(-c.y));
	REQUIRE(q.z == static_cast<TestType>(-c.z));
	REQUIRE(q.w == c.w);
}

TEMPLATE_TEST_CASE("inverse", "[quat][inv]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const q = quat{generate_vec<4, TestType>()};
		auto const i = inverse(q);

		REQUIRE(equal(q * i, i * q));
		REQUIRE(equal(q * i, quat<TestType>{0, 0, 0, 1}));
	}
}

TEMPLATE_TEST_CASE("versors", "[quat][versor]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const axis = normal(generate_vec<3, TestType>());
		REQUIRE_THAT(norm(axis), WithinRelTypeBound(1.0));

		auto const angle = generate_number<TestType, 128, false, std::numbers::pi>();

		auto const q_manual = quat{axis * std::sin(angle / 2), std::cos(angle / 2)};
		auto const q_versor = versor(axis, angle);

		REQUIRE((equal(q_manual, q_versor) || equal(-q_manual, q_versor)));
	}
}

TEMPLATE_TEST_CASE("rotations", "[quat][rot]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const axis  = normal(generate_vec<3, TestType>());
		auto const angle = generate_number<TestType, 128, false, std::numbers::pi>();

		auto const q = versor(axis, angle);

		auto const q_matrix = rotation(q);
		auto const r_matrix = rotation(axis, angle);

		auto m = generate_mat<4, 4, TestType, 512>();

		m[3]    = vec4<TestType>{};
		m[3][3] = 1;
		m[0][3] = 0;
		m[1][3] = 0;
		m[2][3] = 0;

		REQUIRE(equal(q_matrix * m, r_matrix * m));
	}
}
}
