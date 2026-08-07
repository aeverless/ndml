module;

#include "deps.def"

export module ndml.tests:quat;

import :common;

import ndml;
import std;

namespace
{

TEMPLATE_TEST_CASE("construction from vector", "[quat]", NDML_TEST_TYPES_ALL)
{
	auto const from = generate_vec<4, TestType>();
	auto const q    = quat{from};

	REQUIRE(q.x == from.x);
	REQUIRE(q.y == from.y);
	REQUIRE(q.z == from.z);
	REQUIRE(q.w == from.w);
}

TEMPLATE_TEST_CASE("construction from 3-vector and scalar", "[quat]", NDML_TEST_TYPES_ALL)
{
	auto const v = generate_vec<3, TestType>();
	auto const w = generate_number<TestType>();

	auto const q = quat{v, w};

	REQUIRE(q.x == v.x);
	REQUIRE(q.y == v.y);
	REQUIRE(q.z == v.z);
	REQUIRE(q.w == w);
}

TEMPLATE_TEST_CASE("construction from components", "[quat]", NDML_TEST_TYPES_ALL)
{
	auto const v = generate_vec<4, TestType>();
	auto const q = quat{v.x, v.y, v.z, v.w};

	REQUIRE(q.x == v.x);
	REQUIRE(q.y == v.y);
	REQUIRE(q.z == v.z);
	REQUIRE(q.w == v.w);
}

TEMPLATE_TEST_CASE("conjugate", "[quat][conj]", NDML_TEST_TYPES_ALL)
{
	auto const q = quat{generate_vec<4, TestType>()};
	auto const c = conjugate(q);

	REQUIRE(q.x == static_cast<TestType>(-c.x));
	REQUIRE(q.y == static_cast<TestType>(-c.y));
	REQUIRE(q.z == static_cast<TestType>(-c.z));
	REQUIRE(q.w == c.w);
	REQUIRE(equal(conjugate(c), q));
}

TEMPLATE_TEST_CASE("Hamilton product follows quaternion basis identities", "[quat][mul]", NDML_TEST_TYPES_FLOAT)
{
	auto const i = quat<TestType>{1, 0, 0, 0};
	auto const j = quat<TestType>{0, 1, 0, 0};
	auto const k = quat<TestType>{0, 0, 1, 0};

	REQUIRE(equal(i * j, k));
	REQUIRE(equal(j * i, quat<TestType>{0, 0, -1, 0}));
	REQUIRE(equal(i * i, quat<TestType>{0, 0, 0, -1}));
}

TEMPLATE_TEST_CASE("Hamilton multiplication assignment", "[quat][mul][assign]", NDML_TEST_TYPES_FLOAT)
{
	auto       lhs      = quat<TestType>{1, 2, 3, 4};
	auto const rhs      = quat<TestType>{5, 6, 7, 8};
	auto const expected = lhs * rhs;

	auto& result = lhs *= rhs;

	REQUIRE(&result == &lhs);
	REQUIRE(equal(lhs, expected));
}

TEMPLATE_TEST_CASE("inverse", "[quat][inv]", NDML_TEST_TYPES_FLOAT)
{
	auto const q = quat{generate_vec<4, TestType>()};
	auto const i = inverse(q);

	REQUIRE(equal(q * i, i * q));
	REQUIRE(equal(q * i, quat<TestType>{0, 0, 0, 1}));
}

TEMPLATE_TEST_CASE("versors", "[quat][versor]", NDML_TEST_TYPES_FLOAT)
{
	auto const axis = normal(generate_vec<3, TestType>());
	REQUIRE_THAT(norm(axis), WithinRelTypeBound<TestType>(1.0));

	auto const angle = generate_number<TestType, 128, false, std::numbers::pi>();

	auto const q_manual = quat{axis * std::sin(angle / 2), std::cos(angle / 2)};
	auto const q_versor = versor(axis, angle);

	REQUIRE((equal(q_manual, q_versor) || equal(-q_manual, q_versor)));
}

TEMPLATE_TEST_CASE("axis-angle round trip", "[quat][axis_angle]", NDML_TEST_TYPES_FLOAT)
{
	auto const axis  = normal(vec3<TestType>{1, 2, 3});
	auto const angle = TestType{0.75};
	auto const q     = versor(axis, angle);

	auto const [extracted_axis, extracted_angle] = axis_angle(q);

	REQUIRE(equal(extracted_axis, axis));
	REQUIRE_THAT(extracted_angle, WithinRelTypeBound<TestType>(angle));
}

TEMPLATE_TEST_CASE("identity quaternion has zero axis-angle", "[quat][axis_angle]", NDML_TEST_TYPES_FLOAT)
{
	auto const [axis, angle] = axis_angle(quat<TestType>{0, 0, 0, 1});

	REQUIRE(axis == vec3<TestType>{});
	REQUIRE(angle == 0);
}

TEMPLATE_TEST_CASE("quaternion rotates vectors directly", "[quat][rot][vec]", NDML_TEST_TYPES_FLOAT)
{
	constexpr auto half_pi = std::numbers::pi_v<TestType> / TestType{2};
	auto const     q       = versor(vec3<TestType>{0, 0, 1}, half_pi);
	auto const     rotated = q * vec3<TestType>{1, 0, 0};

	REQUIRE_THAT(rotated.x, WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(rotated.y, WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT(rotated.z, WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(norm(rotated), WithinRelTypeBound<TestType>(1));
}

TEMPLATE_TEST_CASE("identity quaternion converts to identity matrix", "[quat][rot]", NDML_TEST_TYPES_FLOAT)
{
	auto const identity = quat<TestType>{0, 0, 0, 1};

	REQUIRE(rotation(identity) == mat4x4<TestType>{1});
}

TEMPLATE_TEST_CASE("rotations", "[quat][rot]", NDML_TEST_TYPES_FLOAT)
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
