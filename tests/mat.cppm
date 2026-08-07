module;

#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers.hpp"
#include "deps.def"

export module ndml.tests:mat;

import :common;

import ndml;
import std;

namespace
{
using Catch::Matchers::WithinAbs;

TEMPLATE_TEST_CASE("default construction yields zero matrix", "[mat][construct]", NDML_TEST_TYPES_ALL)
{
	auto const m = mat2x2<TestType>{};

	for (auto i = 0uz; i < mat2x2<TestType>::column_count; ++i)
	{
		for (auto j = 0uz; j < mat2x2<TestType>::row_count; ++j)
		{
			REQUIRE(m[i, j] == 0);
		}
	}
}

TEMPLATE_TEST_CASE("scale construction creates diagonal matrix", "[mat][construct]", NDML_TEST_TYPES_ALL)
{
	auto const scale = generate_number<TestType>();
	auto const m     = mat3x3<TestType>{scale};

	for (auto i = 0uz; i < m.column_count; ++i)
	{
		REQUIRE(m[i, i] == scale);
	}
}

TEMPLATE_TEST_CASE("scale construction respects rectangular matrix bounds", "[mat][construct]", NDML_TEST_TYPES_ALL)
{
	auto const wide = mat<4, 2, TestType>{3};

	REQUIRE(wide[0, 0] == 3);
	REQUIRE(wide[1, 1] == 3);
	REQUIRE(wide[2] == vec2<TestType>{});
	REQUIRE(wide[3] == vec2<TestType>{});

	auto const tall = mat<2, 4, TestType>{3};
	REQUIRE(tall[0] == vec4<TestType>{3, 0, 0, 0});
	REQUIRE(tall[1] == vec4<TestType>{0, 3, 0, 0});
}

TEMPLATE_TEST_CASE("construct from columns", "[mat][construct]", NDML_TEST_TYPES_ALL)
{
	auto const a = generate_vec<3, TestType>();
	auto const b = generate_vec<3, TestType>();
	auto const c = generate_vec<3, TestType>();

	auto m = mat3x3<TestType>{a, b, c};

	REQUIRE(m[0] == a);
	REQUIRE(m[1] == b);
	REQUIRE(m[2] == c);
}

TEMPLATE_TEST_CASE("construct from fewer columns with template deduction", "[mat][construct]", NDML_TEST_TYPES_ALL)
{
	auto const col0 = generate_vec<3, TestType>();
	auto const col1 = generate_vec<3, TestType>();

	auto const m = mat{col0, col1};

	static_assert(decltype(m)::column_count == 2);
	static_assert(decltype(m)::row_count == 3);

	REQUIRE(m[0] == col0);
	REQUIRE(m[1] == col1);
}

TEMPLATE_TEST_CASE("row and column counts", "[mat][dim]", NDML_TEST_TYPES_ALL)
{
	auto const m1 = mat<1, 4, TestType>();
	auto const m2 = mat<4, 2, TestType>();

	REQUIRE(m1.column_count == 1);
	REQUIRE(m1.row_count == 4);
	REQUIRE(m1.size() == 1);

	REQUIRE(m2.column_count == 4);
	REQUIRE(m2.row_count == 2);
	REQUIRE(m2.size() == 4);
}

TEMPLATE_TEST_CASE("element subscript with mutation", "[mat][access]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat<2, 3, TestType>();
	m[0, 0] = 42;
	m[1, 2] = -67;

	REQUIRE(m[0, 0] == static_cast<TestType>(42));
	REQUIRE(m[1, 2] == static_cast<TestType>(-67));
}

TEMPLATE_TEST_CASE("column subscript with mutation", "[mat][access]", NDML_TEST_TYPES_ALL)
{
	auto const c = vec<3, TestType>{generate_number<TestType>(), 8, 9};

	auto m = mat3x3<TestType>{};
	m[0]   = c;

	REQUIRE(m[0] == c);
}

TEMPLATE_TEST_CASE("iterate over all columns", "[mat][iter]", NDML_TEST_TYPES_ALL)
{
	auto const m = mat3x3<TestType>{};

	std::size_t col_count = 0;
	for (auto const& col : m)
	{
		REQUIRE(col.size() == 3);
		++col_count;
	}

	REQUIRE(col_count == mat3x3<TestType>::column_count);
}

TEMPLATE_TEST_CASE("equal matrices compare equal", "[mat][compare]", NDML_TEST_TYPES_ALL)
{
	auto const a = generate_vec<3, TestType>();
	auto const b = generate_vec<3, TestType>();
	auto const c = generate_vec<3, TestType>();

	auto const lhs = mat3x3<TestType>{a, b, c};
	auto const rhs = mat3x3<TestType>{a, b, c};

	REQUIRE(lhs == rhs);
}

TEMPLATE_TEST_CASE("distinct matrices compare unequal", "[mat][compare]", NDML_TEST_TYPES_ALL)
{
	auto const a = vec<3, TestType>{9};

	auto const lhs = mat3x3<TestType>{a, vec<3, TestType>(), vec<3, TestType>()};
	auto const rhs = mat3x3<TestType>{};

	REQUIRE(lhs != rhs);
}

TEMPLATE_TEST_CASE("swap two matrices", "[mat][swap]", NDML_TEST_TYPES_ALL)
{
	auto a  = mat3x3<TestType>();
	a[2, 2] = 6;

	auto b  = mat3x3<TestType>{};
	b[1, 1] = 7;

	swap(a, b);

	REQUIRE(a[1, 1] == 7);
	REQUIRE(b[2, 2] == 6);
}

TEMPLATE_TEST_CASE("matrix unary plus", "[mat][unary]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = -1;

	auto const p = +m;

	REQUIRE(p[0, 0] == static_cast<TestType>(-1));
}

TEMPLATE_TEST_CASE("matrix unary negation", "[mat][neg]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = 5;
	m[2, 2] = -6;

	auto const n = -m;

	REQUIRE(n[0, 0] == static_cast<TestType>(-5));
	REQUIRE(n[2, 2] == static_cast<TestType>(6));
}

TEMPLATE_TEST_CASE("matrix addition", "[mat][add]", NDML_TEST_TYPES_ALL)
{
	auto a  = mat3x3<TestType>();
	a[0, 0] = 1;
	a[1, 1] = 2;

	auto b  = mat3x3<TestType>();
	b[0, 0] = 10;
	b[1, 1] = 20;

	auto const sum = a + b;

	REQUIRE(sum[0, 0] == 11);
	REQUIRE(sum[1, 1] == 22);
}

TEMPLATE_TEST_CASE("matrix addition assignment", "[mat][add]", NDML_TEST_TYPES_ALL)
{
	auto lhs  = mat3x3<TestType>();
	lhs[0, 0] = 1;

	auto rhs  = mat3x3<TestType>();
	rhs[0, 0] = 5;

	lhs += rhs;

	REQUIRE(lhs[0, 0] == 6);
}

TEMPLATE_TEST_CASE("matrix subtraction", "[mat][sub]", NDML_TEST_TYPES_ALL)
{
	auto a  = mat3x3<TestType>();
	a[0, 0] = 11;
	a[1, 1] = 22;

	auto b  = mat3x3<TestType>();
	b[0, 0] = 5;
	b[1, 1] = 6;

	auto const diff = a - b;

	REQUIRE(diff[0, 0] == 6);
}

TEMPLATE_TEST_CASE("matrix subtraction assignment", "[mat][sub]", NDML_TEST_TYPES_ALL)
{
	auto lhs  = mat3x3<TestType>();
	lhs[0, 0] = 11;
	lhs[1, 1] = 22;

	auto rhs  = mat3x3<TestType>();
	rhs[0, 0] = 5;
	rhs[1, 1] = 6;

	lhs -= rhs;

	REQUIRE(lhs[0, 0] == 6);
}

TEMPLATE_TEST_CASE("multiplication of compatible matrices", "[mat][mul]", NDML_TEST_TYPES_FLOAT)
{
	static constexpr auto check_for_c_r_k = []<std::size_t C, std::size_t R, std::size_t K>
	{
		auto const a = generate_mat<C, R, TestType, 64, false>();
		auto const b = generate_mat<K, C, TestType, 64, false>();

		auto const p = a * b;

		static_assert(decltype(p)::column_count == K);
		static_assert(decltype(p)::row_count == R);

		for (auto i = 0uz; i < a.row_count; ++i)
		{
			for (auto j = 0uz; j < b.column_count; ++j)
			{
				auto element = TestType{0};

				for (auto k = 0uz; k < a.column_count; ++k)
				{
					element += a[k, i] * b[j, k];
				}

				REQUIRE_THAT(element, WithinRelTypeBound<TestType>(p[j, i]));
			}
		}
	};

	static constexpr auto check_for_c_r = []<std::size_t C, std::size_t R>
	{
		check_for_c_r_k.template operator()<C, R, 1>();
		check_for_c_r_k.template operator()<C, R, 2>();
		check_for_c_r_k.template operator()<C, R, 3>();
		check_for_c_r_k.template operator()<C, R, 4>();
	};

	static constexpr auto check_for_c = []<std::size_t C>
	{
		check_for_c_r.template operator()<C, 1>();
		check_for_c_r.template operator()<C, 2>();
		check_for_c_r.template operator()<C, 3>();
		check_for_c_r.template operator()<C, 4>();
	};

	check_for_c.template operator()<1>();
	check_for_c.template operator()<2>();
	check_for_c.template operator()<3>();
	check_for_c.template operator()<4>();
}

TEMPLATE_TEST_CASE("known rectangular matrix multiplication", "[mat][mul]", NDML_TEST_TYPES_ALL)
{
	// Conventional shapes: 2x3 multiplied by 3x2 yields 2x2.
	auto const lhs = mat<3, 2, TestType>{
		vec2<TestType>{1, 4},
		vec2<TestType>{2, 5},
		vec2<TestType>{3, 6}
	};
	auto const rhs = mat<2, 3, TestType>{
		vec3<TestType>{7,  9, 11},
		vec3<TestType>{8, 10, 12}
	};

	auto const product = lhs * rhs;

	static_assert(decltype(product)::column_count == 2);
	static_assert(decltype(product)::row_count == 2);
	REQUIRE(product == mat2x2<TestType>{
				   vec2<TestType>{58, 139},
				   vec2<TestType>{64, 154}
        });
}

TEMPLATE_TEST_CASE("identity matrix multiplication", "[mat][mul][identity]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>();
	m[0, 0] = 2;
	m[1, 1] = 3;
	m[2, 2] = 4;

	auto const identity = mat3x3<TestType>{1};

	auto const p = m * identity;

	REQUIRE(p[0, 0] == 2);
	REQUIRE(p[1, 1] == 3);
	REQUIRE(p[2, 2] == 4);
}

TEMPLATE_TEST_CASE("matrix multiplication assignment", "[mat][mul]", NDML_TEST_TYPES_ALL)
{
	auto lhs  = mat3x3<TestType>{};
	lhs[0, 0] = 1;
	lhs[1, 1] = 2;

	auto rhs  = mat3x3<TestType>{1};
	rhs[1, 1] = 5;

	lhs *= rhs;

	REQUIRE(lhs[0, 0] == 1);
	REQUIRE(lhs[1, 1] == 10);
}

TEMPLATE_TEST_CASE("rectangular matrix multiplication assignment", "[mat][mul]", NDML_TEST_TYPES_ALL)
{
	auto lhs = mat<2, 3, TestType>{
		vec3<TestType>{1, 2, 3},
		vec3<TestType>{4, 5, 6}
	};
	auto rhs = mat2x2<TestType>{
		vec2<TestType>{2, 0},
		vec2<TestType>{0, 3}
	};

	lhs *= rhs;

	REQUIRE(lhs[0] == vec3<TestType>{2, 4, 6});
	REQUIRE(lhs[1] == vec3<TestType>{12, 15, 18});
}


TEMPLATE_TEST_CASE("multiply matrix by vector", "[mat][mul][vec]", NDML_TEST_TYPES_ALL)
{
	auto       m = generate_mat<3, 3, TestType>();
	auto const v = generate_vec<3, TestType>();

	auto const res = m * v;

	REQUIRE(res[0] == dot(v, vec<3, TestType>{m[0, 0], m[1, 0], m[2, 0]}));
}

TEMPLATE_TEST_CASE("multiply rectangular matrix by compatible vector", "[mat][mul][vec]", NDML_TEST_TYPES_ALL)
{
	auto const m = mat<2, 3, TestType>{
		vec3<TestType>{1, 2, 3},
		vec3<TestType>{4, 5, 6}
	};
	auto const v = vec2<TestType>{7, 8};

	auto const result = m * v;

	static_assert(decltype(result)::dimension == 3);
	REQUIRE(result == vec3<TestType>{39, 54, 69});
}

TEMPLATE_TEST_CASE("matrix by scalar", "[mat][mul][scalar]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>();
	m[0, 0] = 2;
	m[1, 1] = 3;

	auto const p = m * 4;

	REQUIRE(p[0, 0] == 8);
	REQUIRE(p[1, 1] == 12);
}

TEMPLATE_TEST_CASE("scalar by matrix", "[mat][mul][scalar]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>{};
	m[1, 1] = 5;

	auto const p = 10 * m;

	REQUIRE(p[1, 1] == 50);
}

TEMPLATE_TEST_CASE("scalar multiply assignment", "[mat][mul][scalar]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = 2;
	m[1, 1] = 3;

	m *= 5;

	REQUIRE(m[0, 0] == 10);
	REQUIRE(m[1, 1] == 15);
}

TEMPLATE_TEST_CASE("divide matrix by scalar", "[mat][div]", NDML_TEST_TYPES_ALL)
{
	auto const m = generate_mat<3, 3, TestType>();
	auto const q = m / 5;
	for (auto i = 0uz; i < m.column_count; ++i)
	{
		for (auto j = 0uz; j < m.row_count; ++j)
		{
			REQUIRE_THAT((q[i, j]), (WithinRelTypeBound<TestType>(m[i, j] / 5)));
		}
	}
}

TEMPLATE_TEST_CASE("transpose swaps rows and columns", "[mat][trans]", NDML_TEST_TYPES_ALL)
{
	auto       m = generate_mat<2, 4, TestType>();
	auto const t = transpose(m);

	static_assert(m.column_count == t.row_count);
	static_assert(m.row_count == t.column_count);

	for (auto i = 0uz; i < m.column_count; ++i)
	{
		for (auto j = 0uz; j < m.row_count; ++j)
		{
			REQUIRE(m[i, j] == t[j, i]);
		}
	}
}

TEMPLATE_TEST_CASE("double transpose is identity transform", "[mat][trans]", NDML_TEST_TYPES_ALL)
{
	auto const m = generate_mat<3, 2, TestType>();
	REQUIRE(m == transpose(transpose(m)));
}

TEMPLATE_TEST_CASE("transpose has a known rectangular result", "[mat][trans]", NDML_TEST_TYPES_ALL)
{
	auto const m = mat<2, 3, TestType>{
		vec3<TestType>{1, 2, 3},
		vec3<TestType>{4, 5, 6}
	};

	auto const expected = mat<3, 2, TestType>{
		vec2<TestType>{1, 4},
		vec2<TestType>{2, 5},
		vec2<TestType>{3, 6}
	};

	REQUIRE(transpose(m) == expected);
}


TEMPLATE_TEST_CASE("row echelon on identity is unchanged", "[mat][ref]", NDML_TEST_TYPES_FLOAT)
{
	auto const identity = mat3x3<TestType>{1};
	REQUIRE(row_echelon_form(identity) == identity);
}

TEMPLATE_TEST_CASE("row echelon performs elimination after a pivot swap", "[mat][ref]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = mat3x3<TestType>{
		vec3<TestType>{0, 1, 0},
		vec3<TestType>{2, 3, 0},
		vec3<TestType>{1, 4, 5}
	};
	auto const expected = mat3x3<TestType>{
		vec3<TestType>{1, 0, 0},
		vec3<TestType>{3, 2, 0},
		vec3<TestType>{4, 1, 5}
	};

	REQUIRE(equal(row_echelon_form(m), expected));
}


TEMPLATE_TEST_CASE("determinant of 1x1 is the element", "[mat][det]", NDML_TEST_TYPES_ALL)
{
	auto const m = generate_mat<1, 1, TestType>();
	REQUIRE(determinant(m) == m[0][0]);
}

TEMPLATE_TEST_CASE("determinant of 2x2 identity is 1", "[mat][det]", NDML_TEST_TYPES_ALL)
{
	auto const m = mat2x2<TestType>{1};
	REQUIRE_THAT(determinant(m), WithinRelTypeBound(1));
}

TEMPLATE_TEST_CASE("determinant of a known 2x2 matrix", "[mat][det]", NDML_TEST_TYPES_ALL)
{
	auto const m = mat2x2<TestType>{
		vec2<TestType>{1, 3},
		vec2<TestType>{2, 4}
	};

	REQUIRE(determinant(m) == static_cast<TestType>(-2));
}

TEMPLATE_TEST_CASE("determinant of a known 3x3 matrix", "[mat][det]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = mat3x3<TestType>{
		vec3<TestType>{6,  4, 2},
		vec3<TestType>{1, -2, 8},
		vec3<TestType>{1,  5, 7}
	};

	REQUIRE_THAT(determinant(m), WithinRelTypeBound<TestType>(-306));
}

TEMPLATE_TEST_CASE("generic determinant accounts for a pivot row swap", "[mat][det]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = mat4x4<TestType>{
		vec4<TestType>{0, 3, 0, 0},
		vec4<TestType>{2, 0, 0, 0},
		vec4<TestType>{0, 0, 4, 0},
		vec4<TestType>{0, 0, 0, 5}
	};

	REQUIRE_THAT(determinant(m), WithinRelTypeBound<TestType>(-120));
}

TEMPLATE_TEST_CASE("singularity predicate distinguishes singular matrices", "[mat][det][predicate]", NDML_TEST_TYPES_FLOAT)
{
	auto singular = mat3x3<TestType>{1};
	singular[1]   = singular[0];

	REQUIRE(is_singular(singular));
	REQUIRE(!is_singular(mat3x3<TestType>{1}));
}

TEMPLATE_TEST_CASE("trace sums diagonal", "[mat][trc]", NDML_TEST_TYPES_ALL)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = 1;
	m[1, 1] = 2;
	m[2, 2] = 3;

	REQUIRE_THAT(trace(m), WithinRelTypeBound(6));
}

TEMPLATE_TEST_CASE("inverse of identity is identity", "[mat][inv]", NDML_TEST_TYPES_FLOAT)
{
	auto const identity = mat4x4<TestType>{1};
	REQUIRE(identity == inverse(identity));
}

TEMPLATE_TEST_CASE("inverse * matrix = identity", "[mat][inv]", NDML_TEST_TYPES_FLOAT)
{
	auto m = generate_mat<4, 4, TestType, 64, false, 10.0>();
	for (auto i = 0uz; i < m.column_count; ++i)
	{
		m[i, i] += TestType{42};
	}

	REQUIRE(equal(m * inverse(m), mat4x4<TestType>{1}));
}

TEMPLATE_TEST_CASE("inverse overloads have known results", "[mat][inv]", NDML_TEST_TYPES_FLOAT)
{
	auto const one = mat<1, 1, TestType>{4};
	REQUIRE(equal(inverse(one), mat<1, 1, TestType>{TestType{0.25}}));

	auto const two = mat2x2<TestType>{
		vec2<TestType>{4, 2},
		vec2<TestType>{7, 6}
	};
	auto const expected = mat2x2<TestType>{
		vec2<TestType>{  TestType{3} / TestType{5}, -TestType{1} / TestType{5}},
		vec2<TestType>{-TestType{7} / TestType{10},  TestType{2} / TestType{5}}
	};

	REQUIRE(equal(inverse(two), expected));
}

TEMPLATE_TEST_CASE("generic inverse performs a pivot row swap", "[mat][inv]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = mat3x3<TestType>{
		vec3<TestType>{0, 1, 0},
		vec3<TestType>{1, 0, 0},
		vec3<TestType>{0, 0, 2}
	};
	auto const expected = mat3x3<TestType>{
		vec3<TestType>{0, 1,             0},
		vec3<TestType>{1, 0,             0},
		vec3<TestType>{0, 0, TestType{0.5}}
	};

	REQUIRE(equal(inverse(m), expected));
	REQUIRE(equal(m * inverse(m), mat3x3<TestType>{1}));
}

TEMPLATE_TEST_CASE("construct 4x4 from 2x2", "[mat][convert]", NDML_TEST_TYPES_ALL)
{
	auto const small = mat2x2<TestType>{
		vec2<TestType>{1, 2},
		vec2<TestType>{3, 4}
	};

	auto const large = mat4x4<TestType>(small);

	REQUIRE(large[0] == vec4<TestType>{1, 2, 0, 0});
	REQUIRE(large[1] == vec4<TestType>{3, 4, 0, 0});
	REQUIRE(large[2] == vec4<TestType>{});
	REQUIRE(large[3] == vec4<TestType>{});
}

TEMPLATE_TEST_CASE("construct from different type", "[mat][convert]", NDML_TEST_TYPES_ALL)
{
	auto src  = mat2x2<std::int16_t>{};
	src[0, 0] = 5;

	auto const dst = mat2x2<TestType>{src};

	REQUIRE(dst[0, 0] == 5);
}

TEMPLATE_TEST_CASE("cross matrix reproduces cross product", "[mat][transform][cross]", NDML_TEST_TYPES_ALL)
{
	auto const v = generate_vec<3, TestType>();
	auto const u = generate_vec<3, TestType>();

	auto const result   = cross_matrix(v) * u;
	auto const expected = cross(v, u);

	for (auto i = 0uz; i < 3; ++i)
	{
		REQUIRE(result[i] == expected[i]);
	}
}

TEMPLATE_TEST_CASE("cross matrix has zero diagonal and is skew symmetric", "[mat][transform][cross]", NDML_TEST_TYPES_ALL)
{
	auto const v = generate_vec<3, TestType>();
	auto const m = cross_matrix(v);

	for (auto i = 0uz; i < 3; ++i)
	{
		REQUIRE(m[i, i] == 0);

		for (auto j = i + 1; j < 3; ++j)
		{
			REQUIRE(m[i, j] == static_cast<TestType>(-m[j, i]));
		}
	}
}

TEMPLATE_TEST_CASE("outer product has expected entries", "[mat][transform][outer]", NDML_TEST_TYPES_ALL)
{
	auto const lhs = vec3<TestType>{1, 2, 3};
	auto const rhs = vec2<TestType>{4, 5};
	auto const m   = outer_product(lhs, rhs);

	static_assert(decltype(m)::column_count == 2);
	static_assert(decltype(m)::row_count == 3);

	for (auto row = 0uz; row < 3; ++row)
	{
		for (auto col = 0uz; col < 2; ++col)
		{
			REQUIRE(m[col, row] == lhs[row] * rhs[col]);
		}
	}
}

TEMPLATE_TEST_CASE("outer product maps a vector to a scaled left operand", "[mat][transform][outer]", NDML_TEST_TYPES_FLOAT)
{
	auto const lhs = generate_vec<3, TestType>();
	auto const rhs = generate_vec<3, TestType>();
	auto const v   = generate_vec<3, TestType>();

	auto const result   = outer_product(lhs, rhs) * v;
	auto const expected = lhs * dot(rhs, v);

	for (auto i = 0uz; i < 3; ++i)
	{
		auto const magnitude = std::abs(lhs[i]) * (std::abs(rhs.x * v.x) + std::abs(rhs.y * v.y) + std::abs(rhs.z * v.z));
		auto const tolerance = TestType{32} * eps<TestType> * magnitude;
		REQUIRE(std::abs(result[i] - expected[i]) <= tolerance);
	}
}

TEMPLATE_TEST_CASE("scale creates a homogeneous diagonal matrix", "[mat][transform][scale]", NDML_TEST_TYPES_ALL)
{
	auto const factors = vec3<TestType>{2, 3, 4};
	auto const s       = scale(factors);

	REQUIRE(s[0, 0] == 2);
	REQUIRE(s[1, 1] == 3);
	REQUIRE(s[2, 2] == 4);
	REQUIRE(s[3, 3] == 1);

	for (auto col = 0uz; col < 4; ++col)
	{
		for (auto row = 0uz; row < 4; ++row)
		{
			if (col != row)
			{
				REQUIRE(s[col, row] == 0);
			}
		}
	}
}

TEMPLATE_TEST_CASE("scale transforms homogeneous coordinates component-wise", "[mat][transform][scale]", NDML_TEST_TYPES_ALL)
{
	auto const factors = vec3<TestType>{2, 3, 4};
	auto const v       = vec4<TestType>{5, 6, 7, 1};

	REQUIRE(equal(scale(factors) * v, vec4<TestType>{10, 18, 28, 1}));
}

TEMPLATE_TEST_CASE("translation creates a homogeneous translation matrix", "[mat][transform][translation]", NDML_TEST_TYPES_ALL)
{
	auto const offset = vec3<TestType>{2, 3, 4};
	auto const t      = translation(offset);

	REQUIRE(t[0, 0] == 1);
	REQUIRE(t[1, 1] == 1);
	REQUIRE(t[2, 2] == 1);
	REQUIRE(t[3, 3] == 1);
	REQUIRE(t[3, 0] == 2);
	REQUIRE(t[3, 1] == 3);
	REQUIRE(t[3, 2] == 4);

	for (auto col = 0uz; col < 4; ++col)
	{
		for (auto row = 0uz; row < 4; ++row)
		{
			if (col != row && !(col == 3 && row < 3))
			{
				REQUIRE(t[col, row] == 0);
			}
		}
	}
}

TEMPLATE_TEST_CASE("translation adds offset to a homogeneous point", "[mat][transform][translation]", NDML_TEST_TYPES_ALL)
{
	auto const offset = vec3<TestType>{2, 3, 4};
	auto const point  = vec4<TestType>{5, 6, 7, 1};

	REQUIRE(equal(translation(offset) * point, vec4<TestType>{7, 9, 11, 1}));
}

TEMPLATE_TEST_CASE("2d zero rotation is identity", "[mat][transform][rotation]", NDML_TEST_TYPES_FLOAT)
{
	auto const r = ndml::rotation(TestType{0});
	REQUIRE(r == mat3x3<TestType>{1});
}

TEMPLATE_TEST_CASE("2d quarter-turn rotation follows the library coordinate convention", "[mat][transform][rotation]", NDML_TEST_TYPES_FLOAT)
{
	constexpr auto half_pi = std::numbers::pi_v<TestType> / TestType{2};
	auto const     r       = ndml::rotation(half_pi);

	auto const x = r * vec3<TestType>{1, 0, 1};
	auto const y = r * vec3<TestType>{0, 1, 1};

	REQUIRE_THAT(x[0], WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(x[1], WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT(y[0], WithinRelTypeBound<TestType>(-1));
	REQUIRE_THAT(y[1], WithinAbsTypeBound<TestType>(0));
	REQUIRE(x[2] == 1);
	REQUIRE(y[2] == 1);
}

TEMPLATE_TEST_CASE("3d zero rotation is identity", "[mat][transform][rotation]", NDML_TEST_TYPES_FLOAT)
{
	auto const axis = vec3<TestType>{1, 0, 0};
	auto const r    = rotation(axis, TestType{0});

	REQUIRE(r == mat4x4<TestType>{1});
}

TEMPLATE_TEST_CASE("3d rotation about x axis matches a quarter turn", "[mat][transform][rotation]", NDML_TEST_TYPES_FLOAT)
{
	constexpr auto half_pi = std::numbers::pi_v<TestType> / TestType{2};
	auto const     r       = rotation(vec3<TestType>{1, 0, 0}, half_pi);

	auto const y = r * vec4<TestType>{0, 1, 0, 1};
	auto const z = r * vec4<TestType>{0, 0, 1, 1};

	REQUIRE_THAT(y[1], WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(y[2], WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT(z[1], WithinRelTypeBound<TestType>(-1));
	REQUIRE_THAT(z[2], WithinAbsTypeBound<TestType>(0));
}

TEMPLATE_TEST_CASE("3d arbitrary-axis rotation is orthogonal and fixes its axis", "[mat][transform][rotation]", NDML_TEST_TYPES_FLOAT)
{
	auto const axis  = normal(vec3<TestType>{1, 2, 3});
	auto const angle = TestType{0.7};
	auto const r     = rotation(axis, angle);

	REQUIRE(equal(r * transpose(r), mat4x4<TestType>{1}));
	REQUIRE_THAT(determinant(r), WithinRelTypeBound<TestType>(1));

	auto const homogeneous_axis = vec4<TestType>{axis.x, axis.y, axis.z, 0};
	REQUIRE(equal(r * homogeneous_axis, homogeneous_axis));
}

TEMPLATE_TEST_CASE("look-at at origin looking along negative z is identity", "[mat][transform][look_at]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = look_at(vec3<TestType>{0, 0, 0}, vec3<TestType>{0, 0, -1}, vec3<TestType>{0, 1, 0});

	REQUIRE(m == mat4x4<TestType>{1});
}

TEMPLATE_TEST_CASE("look-at translates the eye into view space", "[mat][transform][look_at]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = look_at(vec3<TestType>{0, 0, 1}, vec3<TestType>{0, 0, 0}, vec3<TestType>{0, 1, 0});

	auto const eye = m * vec4<TestType>{0, 0, 1, 1};

	REQUIRE_THAT(eye[0], WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(eye[1], WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(eye[2], WithinAbsTypeBound<TestType>(0));
	REQUIRE(eye[3] == 1);
	REQUIRE_THAT((m[3, 2]), WithinRelTypeBound<TestType>(-1));
}

TEMPLATE_TEST_CASE("look-at orients an arbitrary target along negative z", "[mat][transform][look_at]", NDML_TEST_TYPES_FLOAT)
{
	auto const eye    = vec3<TestType>{1, 2, 3};
	auto const target = vec3<TestType>{-2, 1, 0};
	auto const up     = vec3<TestType>{0, 1, 0};
	auto const m      = look_at(eye, target, up);

	auto const view_eye    = m * vec4<TestType>{eye.x, eye.y, eye.z, 1};
	auto const view_target = m * vec4<TestType>{target.x, target.y, target.z, 1};

	REQUIRE(equal(view_eye, vec4<TestType>{0, 0, 0, 1}));
	REQUIRE_THAT(view_target[0], WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(view_target[1], WithinAbsTypeBound<TestType>(0));
	REQUIRE_THAT(view_target[2], WithinRelTypeBound<TestType>(-norm(target - eye)));
	REQUIRE(view_target[3] == 1);
}

TEMPLATE_TEST_CASE("orthographic projection maps symmetric x and y bounds to unit coordinates", "[mat][transform][ortho]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = ndml::ortho(TestType{-2}, TestType{2}, TestType{-3}, TestType{3}, TestType{1}, TestType{11});

	auto const left   = m * vec4<TestType>{-2, 0, 1, 1};
	auto const right  = m * vec4<TestType>{2, 0, 1, 1};
	auto const bottom = m * vec4<TestType>{0, -3, 1, 1};
	auto const top    = m * vec4<TestType>{0, 3, 1, 1};

	REQUIRE_THAT(left[0], WithinRelTypeBound<TestType>(-1));
	REQUIRE_THAT(right[0], WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT(bottom[1], WithinRelTypeBound<TestType>(-1));
	REQUIRE_THAT(top[1], WithinRelTypeBound<TestType>(1));
}

TEMPLATE_TEST_CASE("orthographic projection maps near and far planes to opposite unit depths", "[mat][transform][ortho]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = ndml::ortho(TestType{-1}, TestType{1}, TestType{-1}, TestType{1}, TestType{1}, TestType{11});

	auto const near = m * vec4<TestType>{0, 0, -1, 1};
	auto const far  = m * vec4<TestType>{0, 0, -11, 1};

	REQUIRE_THAT(near[2], WithinRelTypeBound<TestType>(-1));
	REQUIRE_THAT(far[2], WithinRelTypeBound<TestType>(1));
}

TEMPLATE_TEST_CASE("orthographic projection maps asymmetric bounds", "[mat][transform][ortho]", NDML_TEST_TYPES_FLOAT)
{
	auto const m = ndml::ortho(TestType{2}, TestType{6}, TestType{-1}, TestType{3}, TestType{2}, TestType{10});

	auto const minimum = m * vec4<TestType>{2, -1, -2, 1};
	auto const maximum = m * vec4<TestType>{6, 3, -10, 1};
	auto const center  = m * vec4<TestType>{4, 1, -6, 1};

	REQUIRE(equal(minimum, vec4<TestType>{-1, -1, -1, 1}));
	REQUIRE(equal(maximum, vec4<TestType>{1, 1, 1, 1}));
	REQUIRE(equal(center, vec4<TestType>{0, 0, 0, 1}));
}

TEMPLATE_TEST_CASE("perspective projection has expected canonical coefficients", "[mat][transform][perspective]", NDML_TEST_TYPES_FLOAT)
{
	static constexpr auto fov = std::numbers::pi_v<TestType> / TestType{2};
	auto const            m   = ndml::perspective(fov, TestType{1}, TestType{1}, TestType{11});

	REQUIRE_THAT((m[0, 0]), WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT((m[1, 1]), WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT((m[2, 2]), WithinRelTypeBound<TestType>(-TestType{6} / TestType{5}));
	REQUIRE_THAT((m[3, 2]), WithinRelTypeBound<TestType>(-TestType{11} / TestType{5}));
	REQUIRE(m[2, 3] == -1);
	REQUIRE(m[3, 3] == 0);
}

TEMPLATE_TEST_CASE("perspective projection sends a point through the homogeneous divide", "[mat][transform][perspective]", NDML_TEST_TYPES_FLOAT)
{
	constexpr auto fov       = std::numbers::pi_v<TestType> / TestType{2};
	auto const     m         = ndml::perspective(fov, TestType{1}, TestType{1}, TestType{11});
	auto const     projected = m * vec4<TestType>{0, 0, -1, 1};

	REQUIRE_THAT(projected[2] / projected[3], WithinRelTypeBound<TestType>(-1));
}

TEMPLATE_TEST_CASE("perspective projection accounts for aspect ratio and maps the far plane", "[mat][transform][perspective]", NDML_TEST_TYPES_FLOAT)
{
	constexpr auto fov = std::numbers::pi_v<TestType> / TestType{2};
	auto const     m   = ndml::perspective(fov, TestType{2}, TestType{1}, TestType{11});

	auto const near_corner = m * vec4<TestType>{2, 1, -1, 1};
	auto const far_center  = m * vec4<TestType>{0, 0, -11, 1};

	REQUIRE_THAT(near_corner[0] / near_corner[3], WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT(near_corner[1] / near_corner[3], WithinRelTypeBound<TestType>(1));
	REQUIRE_THAT(near_corner[2] / near_corner[3], WithinRelTypeBound<TestType>(-1));
	REQUIRE_THAT(far_center[2] / far_center[3], WithinRelTypeBound<TestType>(1));
}
}
