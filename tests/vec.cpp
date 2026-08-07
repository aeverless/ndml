#include "catch2/matchers/catch_matchers_floating_point.hpp"
#include "common.hpp"

#include <cmath>
#include <concepts>

namespace {
using Catch::Matchers::WithinAbs;

TEMPLATE_TEST_CASE("1d construction", "[vec][1d]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const v = vec1<TestType>{x};

	REQUIRE(v.x == x);
	REQUIRE(v[0] == x);
}

TEMPLATE_TEST_CASE("2d construction", "[vec][2d]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();

	auto const v = vec2<TestType>{x, y};

	REQUIRE(v.x == x);
	REQUIRE(v[0] == x);

	REQUIRE(v.y == y);
	REQUIRE(v[1] == y);
}

TEMPLATE_TEST_CASE("3d construction", "[vec][3d]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto const v = vec3<TestType>{x, y, z};

	REQUIRE(v.x == x);
	REQUIRE(v[0] == x);

	REQUIRE(v.y == y);
	REQUIRE(v[1] == y);

	REQUIRE(v.z == z);
	REQUIRE(v[2] == z);
}

TEMPLATE_TEST_CASE("boolean contexts", "[vec][bool]", NDML_TEST_TYPES)
{
	REQUIRE(!vec3<TestType>{0, 0, 0});

	REQUIRE(vec3<TestType>{1, 0, 0});
	REQUIRE(vec3<TestType>{0, 1, 0});
	REQUIRE(vec3<TestType>{0, 0, 1});
	REQUIRE(vec3<TestType>{1, 1, 0});
	REQUIRE(vec3<TestType>{0, 1, 1});
	REQUIRE(vec3<TestType>{1, 0, 1});

	auto const x = generate_number<TestType, 64, false>();
	auto const y = generate_number<TestType, 64, false>();
	auto const z = generate_number<TestType, 64, false>();

	auto const v = vec3<TestType>{x, y, z};

	REQUIRE(bool(v) == (x && y && z));
}

TEMPLATE_TEST_CASE("iterating", "[vec][iter]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType, 128, false>();
	auto const y = generate_number<TestType, 128, false>();
	auto const z = generate_number<TestType, 128, false>();
	auto const w = generate_number<TestType, 128, false>();

	auto const check_iteration_coherency = [x, y, z, w]<std::size_t N>(vec<N, TestType> const& v) -> void
	{
		REQUIRE(std::ranges::equal(
			v,
			std::span<TestType const>{
				{x, y, z, w}
                } | std::views::take(N)
		));

		for (auto i = 0uz; i < N; ++i)
		{
			auto const& component = v[i];

			switch (i)
			{
			case 0:
				REQUIRE(x == component);
				break;

			case 1:
				REQUIRE(y == component);
				break;

			case 2:
				REQUIRE(z == component);
				break;

			case 3:
				REQUIRE(w == component);
				break;

			default:
				FAIL("vectors only support up to 4 dimensions");
				break;
			}
		}
	};

	auto const v1 = vec1<TestType>{x};
	auto const v2 = vec2<TestType>{x, y};
	auto const v3 = vec3<TestType>{x, y, z};
	auto const v4 = vec4<TestType>{x, y, z, w};

	check_iteration_coherency(v1);
	check_iteration_coherency(v2);
	check_iteration_coherency(v3);
	check_iteration_coherency(v4);
}

TEMPLATE_TEST_CASE("4d construction", "[vec][4d]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType, 64>();
	auto const y = generate_number<TestType, 64>();
	auto const z = generate_number<TestType, 64>();
	auto const w = generate_number<TestType, 64>();

	auto const v = vec4<TestType>{x, y, z, w};

	REQUIRE(v.x == x);
	REQUIRE(v[0] == x);

	REQUIRE(v.y == y);
	REQUIRE(v[1] == y);

	REQUIRE(v.z == z);
	REQUIRE(v[2] == z);

	REQUIRE(v.w == w);
	REQUIRE(v[3] == w);
}

TEMPLATE_TEST_CASE("copy construction", "[vec][copy]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto const original = vec3<TestType>{x, y, z};
	auto const copy     = original;

	REQUIRE(copy.x == original.x);
	REQUIRE(copy.y == original.y);
	REQUIRE(copy.z == original.z);
}

TEMPLATE_TEST_CASE("move construction", "[vec][move]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto original = vec3<TestType>{x, y, z};
	auto moved    = std::move(original);

	REQUIRE(moved.x == x);
	REQUIRE(moved.y == y);
	REQUIRE(moved.z == z);
}

TEMPLATE_TEST_CASE("construction from smaller vector", "[vec][convert]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();

	auto const small = vec2<TestType>{x, y};
	auto const large = vec4<TestType>(small);

	REQUIRE(large.x == x);
	REQUIRE(large.y == y);
	REQUIRE(large.z == 0.0);
	REQUIRE(large.w == 0.0);
}

TEMPLATE_TEST_CASE("construction from different type", "[vec][convert]", NDML_TEST_TYPES)
{
	auto const v = vec3<TestType>(vec3<std::int32_t>(1, 2, 3));

	REQUIRE(v.x == static_cast<TestType>(1));
	REQUIRE(v.y == static_cast<TestType>(2));
	REQUIRE(v.z == static_cast<TestType>(3));
}

TEMPLATE_TEST_CASE("copy assignment", "[vec][assign]", NDML_TEST_TYPES)
{
	auto const a = generate_number<TestType>();
	auto const b = generate_number<TestType>();
	auto const c = generate_number<TestType>();

	auto const d = generate_number<TestType>();
	auto const e = generate_number<TestType>();
	auto const f = generate_number<TestType>();

	auto       lhs = vec3<TestType>{a, b, c};
	auto const rhs = vec3<TestType>{d, e, f};

	lhs = rhs;

	REQUIRE(lhs.x == d);
	REQUIRE(lhs.y == e);
	REQUIRE(lhs.z == f);
}

TEMPLATE_TEST_CASE("move assignment", "[vec][assign]", NDML_TEST_TYPES)
{
	auto x = generate_number<TestType>();
	auto y = generate_number<TestType>();
	auto z = generate_number<TestType>();

	vec3<TestType> target{10, 20, 30};

	auto source = vec3<TestType>{x, y, z};
	target      = std::move(source);

	REQUIRE((target.x == x || source.x == x));
	REQUIRE((target.y == y || source.y == y));
	REQUIRE((target.z == z || source.z == y));
}

TEMPLATE_TEST_CASE("scalar construction", "[vec][scalar]", NDML_TEST_TYPES)
{
	auto const s = generate_number<TestType, 64>();

	auto const v = vec3<TestType>{s};

	REQUIRE(v.x == s);
	REQUIRE(v.y == s);
	REQUIRE(v.z == s);
}

TEMPLATE_TEST_CASE("vector-scalar comparison equality", "[vec][compare]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto const lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{x, y, z};

	REQUIRE(lhs == rhs);
}

TEMPLATE_TEST_CASE("vector addition", "[vec][add]", NDML_TEST_TYPES)
{
	auto const x1 = generate_number<TestType>();
	auto const y1 = generate_number<TestType>();
	auto const z1 = generate_number<TestType>();

	auto const x2 = generate_number<TestType>();
	auto const y2 = generate_number<TestType>();
	auto const z2 = generate_number<TestType>();

	auto const lhs = vec3<TestType>{x1, y1, z1};
	auto const rhs = vec3<TestType>{x2, y2, z2};

	auto const sum = lhs + rhs;

	REQUIRE(sum.x == static_cast<TestType>(x1 + x2));
	REQUIRE(sum.y == static_cast<TestType>(y1 + y2));
	REQUIRE(sum.z == static_cast<TestType>(z1 + z2));
}

TEMPLATE_TEST_CASE("vector subtraction", "[vec][sub]", NDML_TEST_TYPES)
{
	auto const x1 = generate_number<TestType>();
	auto const y1 = generate_number<TestType>();
	auto const z1 = generate_number<TestType>();

	auto const x2 = generate_number<TestType>();
	auto const y2 = generate_number<TestType>();
	auto const z2 = generate_number<TestType>();

	auto const lhs = vec3<TestType>{x1, y1, z1};
	auto const rhs = vec3<TestType>{x2, y2, z2};

	auto const diff = lhs - rhs;

	REQUIRE(diff.x == static_cast<TestType>(x1 - x2));
	REQUIRE(diff.y == static_cast<TestType>(y1 - y2));
	REQUIRE(diff.z == static_cast<TestType>(z1 - z2));
}

TEMPLATE_TEST_CASE("vector multiplication", "[vec][mul]", NDML_TEST_TYPES)
{
	auto const x1 = generate_number<TestType>();
	auto const y1 = generate_number<TestType>();
	auto const z1 = generate_number<TestType>();

	auto const x2 = generate_number<TestType>();
	auto const y2 = generate_number<TestType>();
	auto const z2 = generate_number<TestType>();

	auto const lhs = vec3<TestType>{x1, y1, z1};
	auto const rhs = vec3<TestType>{x2, y2, z2};

	auto const prod = lhs * rhs;

	REQUIRE(prod.x == static_cast<TestType>(x1 * x2));
	REQUIRE(prod.y == static_cast<TestType>(y1 * y2));
	REQUIRE(prod.z == static_cast<TestType>(z1 * z2));
}

TEMPLATE_TEST_CASE("vector division", "[vec][div]", NDML_TEST_TYPES)
{
	auto const x1 = generate_number<TestType, 64>();
	auto const y1 = generate_number<TestType, 64>();
	auto const z1 = generate_number<TestType, 64>();

	auto const x2 = generate_number<TestType, 64>();
	auto const y2 = generate_number<TestType, 64>();
	auto const z2 = generate_number<TestType, 64>();

	auto const lhs = vec3<TestType>{x1, y1, z1};
	auto const rhs = vec3<TestType>{x2, y2, z2};

	auto const quot = lhs / rhs;

	REQUIRE_THAT(quot.x, WithinRelTypeBound<TestType>(x1 / x2));
	REQUIRE_THAT(quot.y, WithinRelTypeBound<TestType>(y1 / y2));
	REQUIRE_THAT(quot.z, WithinRelTypeBound<TestType>(z1 / z2));
}

TEMPLATE_TEST_CASE("scalar-vector multiplication", "[vec][mul][scalar]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType, 64>();
	auto const y = generate_number<TestType, 64>();
	auto const z = generate_number<TestType, 64>();

	auto const s = generate_number<TestType, 64>() + static_cast<TestType>(1);

	auto       v    = vec3<TestType>{x, y, z};
	auto const prod = v * s;

	REQUIRE(prod.x == static_cast<TestType>(x * s));
	REQUIRE(prod.y == static_cast<TestType>(y * s));
	REQUIRE(prod.z == static_cast<TestType>(z * s));
}

TEMPLATE_TEST_CASE("scalar-vector multiplication commutative", "[vec][mul][scalar]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType, 64>();
	auto const y = generate_number<TestType, 64>();
	auto const z = generate_number<TestType, 64>();

	auto const s = generate_number<TestType, 64>() + static_cast<TestType>(1);

	auto       v    = vec3<TestType>{x, y, z};
	auto const prod = s * v;

	REQUIRE(prod.x == static_cast<TestType>(s * x));
	REQUIRE(prod.y == static_cast<TestType>(s * y));
	REQUIRE(prod.z == static_cast<TestType>(s * z));
}

TEMPLATE_TEST_CASE("scalar-vector division", "[vec][div][scalar]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType, 64>();
	auto const y = generate_number<TestType, 64>();
	auto const z = generate_number<TestType, 64>();

	auto const s = generate_number<TestType>();

	auto const v    = vec3<TestType>{x, y, z};
	auto const quot = v / s;

	REQUIRE_THAT(quot.x, WithinRelTypeBound<TestType>(v.x / s));
	REQUIRE_THAT(quot.y, WithinRelTypeBound<TestType>(v.y / s));
	REQUIRE_THAT(quot.z, WithinRelTypeBound<TestType>(v.z / s));
}

TEMPLATE_TEST_CASE("addition assignment", "[vec][add]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{1, 2, 3};

	lhs += rhs;

	REQUIRE(lhs.x == static_cast<TestType>(x + 1));
	REQUIRE(lhs.y == static_cast<TestType>(y + 2));
	REQUIRE(lhs.z == static_cast<TestType>(z + 3));
}

TEMPLATE_TEST_CASE("subtraction assignment", "[vec][sub]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{1, 2, 3};

	lhs -= rhs;

	REQUIRE(lhs.x == static_cast<TestType>(x - 1));
	REQUIRE(lhs.y == static_cast<TestType>(y - 2));
	REQUIRE(lhs.z == static_cast<TestType>(z - 3));
}

TEMPLATE_TEST_CASE("multiplication assignment", "[vec][mul]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{2, 3, 4};

	lhs *= rhs;

	REQUIRE_THAT(lhs.x, WithinRelTypeBound<TestType>(x * 2));
	REQUIRE_THAT(lhs.y, WithinRelTypeBound<TestType>(y * 3));
	REQUIRE_THAT(lhs.z, WithinRelTypeBound<TestType>(z * 4));
}

TEMPLATE_TEST_CASE("division assignment", "[vec][div]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType, 256>();
	auto const y = generate_number<TestType, 256>();
	auto const z = generate_number<TestType, 256>();

	auto       lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{2, 4, 8};

	lhs /= rhs;

	REQUIRE_THAT(static_cast<TestType>(lhs.x - x / 2), WithinRelTypeBound(0.0));
	REQUIRE_THAT(static_cast<TestType>(lhs.y - y / 4), WithinRelTypeBound(0.0));
	REQUIRE_THAT(static_cast<TestType>(lhs.z - z / 8), WithinRelTypeBound(0.0));
}

TEMPLATE_TEST_CASE("scalar multiplication assignment", "[vec][mul][scalar]", NDML_TEST_TYPES)
{
	auto const s = generate_number<TestType, 64>() + static_cast<TestType>(1);

	auto v = vec3<TestType>{s, s, s};

	v *= s;

	REQUIRE(v.x == static_cast<TestType>(s * s));
	REQUIRE(v.y == static_cast<TestType>(s * s));
	REQUIRE(v.z == static_cast<TestType>(s * s));
}

TEMPLATE_TEST_CASE("scalar division assignment", "[vec][div][scalar]", NDML_TEST_TYPES)
{
	auto const s = generate_number<TestType, 256>();

	auto v = vec3<TestType>{s, s, s};

	v /= s;

	REQUIRE_THAT(v.x, WithinRelTypeBound<TestType>(1.0));
	REQUIRE_THAT(v.y, WithinRelTypeBound<TestType>(1.0));
	REQUIRE_THAT(v.z, WithinRelTypeBound<TestType>(1.0));
}

TEMPLATE_TEST_CASE("unary plus", "[vec][unary]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       v = vec3<TestType>{x, y, z};
	auto const p = +v;

	REQUIRE(p.x == x);
	REQUIRE(p.y == y);
	REQUIRE(p.z == z);
}

TEMPLATE_TEST_CASE("unary negation", "[vec][neg]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       v = vec3<TestType>{x, y, z};
	auto const n = -v;

	REQUIRE(n.x == static_cast<TestType>(-x));
	REQUIRE(n.y == static_cast<TestType>(-y));
	REQUIRE(n.z == static_cast<TestType>(-z));
}

TEMPLATE_TEST_CASE("swap", "[vec][swap]", NDML_TEST_TYPES)
{
	auto const a = generate_number<TestType>();
	auto const b = generate_number<TestType>();
	auto const c = generate_number<TestType>();

	auto const d = generate_number<TestType>();
	auto const e = generate_number<TestType>();
	auto const f = generate_number<TestType>();

	auto lhs = vec3<TestType>{a, b, c};
	auto rhs = vec3<TestType>{d, e, f};

	swap(lhs, rhs);

	REQUIRE((lhs.x == d && lhs.y == e && lhs.z == f));
	REQUIRE((rhs.x == a && rhs.y == b && rhs.z == c));
}

TEMPLATE_TEST_CASE("transform", "[vec][transform]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto const v = vec3<TestType>{x, y, z};

	static constexpr auto f = [](auto arg)
	{
		return arg + arg;
	};

	auto const t = transform(v, f);

	REQUIRE_THAT(t.x, WithinRelTypeBound<TestType>(v.x * 2));
	REQUIRE_THAT(t.y, WithinRelTypeBound<TestType>(v.y * 2));
	REQUIRE_THAT(t.z, WithinRelTypeBound<TestType>(v.z * 2));
}

TEMPLATE_TEST_CASE("for_each", "[vec][for_each]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto v = vec3<TestType>{x, y, z};

	static constexpr auto f = [](auto& arg)
	{
		arg *= static_cast<TestType>(2);
	};

	for_each(v, f);

	REQUIRE(v.x == static_cast<TestType>(x + x));
	REQUIRE(v.y == static_cast<TestType>(y + y));
	REQUIRE(v.z == static_cast<TestType>(z + z));
}

TEMPLATE_TEST_CASE("dot product", "[vec][dot]", NDML_TEST_TYPES)
{
	auto const x1 = generate_number<TestType>();
	auto const y1 = generate_number<TestType>();
	auto const z1 = generate_number<TestType>();

	auto const x2 = generate_number<TestType>();
	auto const y2 = generate_number<TestType>();
	auto const z2 = generate_number<TestType>();

	auto const lhs = vec3<TestType>{x1, y1, z1};
	auto const rhs = vec3<TestType>{x2, y2, z2};

	auto const result   = dot(lhs, rhs);
	auto const expected = static_cast<TestType>(x1 * x2 + y1 * y2 + z1 * z2);

	REQUIRE(result == expected);
}

TEMPLATE_TEST_CASE("cross product", "[vec][cross]", NDML_TEST_TYPES)
{
	auto const x1 = generate_number<TestType>();
	auto const y1 = generate_number<TestType>();
	auto const z1 = generate_number<TestType>();

	auto const x2 = generate_number<TestType>();
	auto const y2 = generate_number<TestType>();
	auto const z2 = generate_number<TestType>();

	auto const lhs = vec3<TestType>{x1, y1, z1};
	auto const rhs = vec3<TestType>{x2, y2, z2};

	auto const c = cross(lhs, rhs);

	auto const exp_x = y1 * z2 - z1 * y2;
	auto const exp_y = z1 * x2 - x1 * z2;
	auto const exp_z = x1 * y2 - y1 * x2;

	REQUIRE(c.x == exp_x);
	REQUIRE(c.y == exp_y);
	REQUIRE(c.z == exp_z);
}

TEMPLATE_TEST_CASE("cross product with self is zero", "[vec][cross]", NDML_TEST_TYPES)
{
	auto       v = generate_vec<3, TestType, 128, false, 1e3>();
	auto const c = cross(v, v);

	REQUIRE_THAT(norm_squared(c), WithinAbs(0.0, 0.01));
}

TEMPLATE_TEST_CASE("cross product orthogonality", "[vec][cross]", NDML_TEST_TYPES)
{
	auto x_axis = vec3<TestType>{1, 0, 0};
	auto y_axis = vec3<TestType>{0, 1, 0};

	auto const cr = cross(x_axis, y_axis);

	REQUIRE_THAT(cr.x, WithinRelTypeBound(0.0));
	REQUIRE_THAT(cr.y, WithinRelTypeBound(0.0));
	REQUIRE_THAT(cr.z, WithinRelTypeBound<TestType>(1.0));
}

TEMPLATE_TEST_CASE("squared norm", "[vec][norm]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto const v = vec3<TestType>{x, y, z};

	auto const result   = norm_squared(v);
	auto const expected = static_cast<TestType>(x * x + y * y + z * z);

	REQUIRE(result == expected);
}

TEMPLATE_TEST_CASE("norm", "[vec][norm]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto const squared = static_cast<double>(x) * x + static_cast<double>(y) * y + static_cast<double>(z) * z;
	if (!std::isfinite(squared) || squared <= eps<double>)
	{
		return;
	}

	auto const expected = std::sqrt(squared);
	REQUIRE_THAT(norm(vec3{x, y, z}), WithinRelTypeBound(expected));
}

TEMPLATE_TEST_CASE("normal - normalized vector", "[vec][normal]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const x = generate_number<TestType>();
		auto const y = generate_number<TestType>();
		auto const z = generate_number<TestType>();

		auto const v   = vec3<TestType>{x, y, z};
		auto const len = norm(v);
		auto const n   = normal(v);

		REQUIRE_THAT(n.x, WithinRelTypeBound(x / len));
		REQUIRE_THAT(n.y, WithinRelTypeBound(y / len));
		REQUIRE_THAT(n.z, WithinRelTypeBound(z / len));
	}
}

TEMPLATE_TEST_CASE("normal really is normal", "[vec][normal]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const x = generate_number<TestType, 128>();
		auto const y = generate_number<TestType, 128>();
		auto const z = generate_number<TestType, 128>();

		auto const v = vec3<TestType>{x, y, z};

		if (norm(v) < eps<double>)
		{
			return;
		}

		auto const n = normal(v);

		REQUIRE_THAT(norm(n), WithinRelTypeBound(1.0));
	}
}

TEMPLATE_TEST_CASE("projection", "[vec][proj]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const x = static_cast<TestType>(5.0);
		auto const y = static_cast<TestType>(3.0);

		auto const v    = vec2<TestType>{x, y};
		auto const axis = vec2<TestType>{1, 0};

		auto const proj = projection(v, axis);

		REQUIRE_THAT(proj.x - x, WithinRelTypeBound(0.0));
		REQUIRE_THAT(proj.y, WithinRelTypeBound(0.0));
	}
}

TEMPLATE_TEST_CASE("reciprocal", "[vec][recip]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const x = static_cast<TestType>(2.0);
		auto const y = static_cast<TestType>(4.0);
		auto const z = static_cast<TestType>(8.0);

		auto const v = vec3<TestType>{x, y, z};
		auto const r = reciprocal(v);

		REQUIRE_THAT(r.x - 0.5, WithinRelTypeBound(0.0));
		REQUIRE_THAT(r.y - 0.25, WithinRelTypeBound(0.0));
		REQUIRE_THAT(r.z - 0.125, WithinRelTypeBound(0.0));
	}
}

TEMPLATE_TEST_CASE("reciprocal of all ones vector", "[vec][recip]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		auto const v = vec3<TestType>{1, 1, 1};
		auto const r = reciprocal(v);

		REQUIRE_THAT(r.x, WithinRelTypeBound<TestType>(1.0));
		REQUIRE_THAT(r.y, WithinRelTypeBound<TestType>(1.0));
		REQUIRE_THAT(r.z, WithinRelTypeBound<TestType>(1.0));
	}
}

TEMPLATE_TEST_CASE("dimension coherence", "[vec][dim]", NDML_TEST_TYPES)
{
	REQUIRE(vec<1, TestType>{}.size() == 1);
	REQUIRE(vec<2, TestType>{}.size() == 2);
	REQUIRE(vec<3, TestType>{}.size() == 3);
	REQUIRE(vec<4, TestType>{}.size() == 4);
}

TEMPLATE_TEST_CASE("zip transform", "[vec][transform]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{1, 2, 3};

	static constexpr auto f = [](auto a, auto b)
	{
		return a + static_cast<TestType>(b * 2);
	};

	lhs = zip_transform(lhs, rhs, f);

	REQUIRE(lhs.x == static_cast<TestType>(x + 2));
	REQUIRE(lhs.y == static_cast<TestType>(y + 4));
	REQUIRE(lhs.z == static_cast<TestType>(z + 6));
}

TEMPLATE_TEST_CASE("zip for each", "[vec][for_each]", NDML_TEST_TYPES)
{
	auto const x = generate_number<TestType>();
	auto const y = generate_number<TestType>();
	auto const z = generate_number<TestType>();

	auto       lhs = vec3<TestType>{x, y, z};
	auto const rhs = vec3<TestType>{1, 2, 3};

	static constexpr auto f = [](auto& a, auto b)
	{
		a += static_cast<TestType>(b * 2);
	};

	zip_for_each(lhs, rhs, f);

	REQUIRE(lhs.x == static_cast<TestType>(x + 2));
	REQUIRE(lhs.y == static_cast<TestType>(y + 4));
	REQUIRE(lhs.z == static_cast<TestType>(z + 6));
}

TEMPLATE_TEST_CASE("chained arithmetic operations", "[vec][chain]", NDML_TEST_TYPES)
{
	auto a = vec3<TestType>{1, 2, 3};
	auto b = vec3<TestType>{4, 5, 6};

	auto const result = (a + b) * static_cast<TestType>(2);

	REQUIRE(result.x == static_cast<TestType>(10));
	REQUIRE(result.y == static_cast<TestType>(14));
	REQUIRE(result.z == static_cast<TestType>(18));
}
} // namespace
