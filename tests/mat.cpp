#include "catch2/matchers/catch_matchers.hpp"
#include "common.hpp"
#include <concepts>
#include <numbers>
#include <numeric>

namespace
{
TEMPLATE_TEST_CASE("default construction yields zero matrix", "[mat][construct]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("scale construction creates diagonal matrix", "[mat][construct]", NDML_TEST_TYPES)
{
	auto const scale = generate_number<TestType>();
	auto const m     = mat3x3<TestType>{scale};

	for (auto i = 0uz; i < m.column_count; ++i)
	{
		REQUIRE(m[i, i] == scale);
	}
}

TEMPLATE_TEST_CASE("construct from columns", "[mat][construct]", NDML_TEST_TYPES)
{
	auto const a = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};
	auto const b = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};
	auto const c = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};

	auto m = mat3x3<TestType>{a, b, c};

	REQUIRE(m[0] == a);
	REQUIRE(m[1] == b);
	REQUIRE(m[2] == c);
}

TEMPLATE_TEST_CASE("construct from fewer columns with template deduction", "[mat][construct]", NDML_TEST_TYPES)
{
	auto const col0 = vec<2, TestType>{generate_number<TestType>(), generate_number<TestType>()};
	auto const col1 = vec<2, TestType>{generate_number<TestType>(), generate_number<TestType>()};

	mat<2, 2, TestType> m{col0, col1};

	REQUIRE(m[0] == col0);
	REQUIRE(m[1] == col1);
}

TEMPLATE_TEST_CASE("row and column counts", "[mat][dim]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("element subscript with mutation", "[mat][access]", NDML_TEST_TYPES)
{
	auto m  = mat<2, 3, TestType>();
	m[0, 0] = 42;
	m[1, 2] = -67;

	REQUIRE(m[0, 0] == static_cast<TestType>(42));
	REQUIRE(m[1, 2] == static_cast<TestType>(-67));
}

TEMPLATE_TEST_CASE("column subscript with mutation", "[mat][access]", NDML_TEST_TYPES)
{
	auto const c = vec<3, TestType>{generate_number<TestType>(), 8, 9};

	auto m = mat3x3<TestType>{};
	m[0]   = c;

	REQUIRE(m[0] == c);
}

TEMPLATE_TEST_CASE("iterate over all columns", "[mat][iter]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("equal matrices compare equal", "[mat][compare]", NDML_TEST_TYPES)
{
	auto const a = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};
	auto const b = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};
	auto const c = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};

	auto const lhs = mat3x3<TestType>{a, b, c};
	auto const rhs = mat3x3<TestType>{a, b, c};

	REQUIRE(lhs == rhs);
}

TEMPLATE_TEST_CASE("distinct matrices compare unequal", "[mat][compare]", NDML_TEST_TYPES)
{
	auto const a = vec<3, TestType>{9};

	auto const lhs = mat3x3<TestType>{a, vec<3, TestType>(), vec<3, TestType>()};
	auto const rhs = mat3x3<TestType>{};

	REQUIRE(lhs != rhs);
}

TEMPLATE_TEST_CASE("swap two matrices", "[mat][swap]", NDML_TEST_TYPES)
{
	auto a  = mat3x3<TestType>();
	a[2, 2] = 6;

	auto b  = mat3x3<TestType>{};
	b[1, 1] = 7;

	swap(a, b);

	REQUIRE(a[1, 1] == 7);
	REQUIRE(b[2, 2] == 6);
}

TEMPLATE_TEST_CASE("unary plus", "[mat][unary]", NDML_TEST_TYPES)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = -1;

	auto const p = +m;

	REQUIRE(p[0, 0] == static_cast<TestType>(-1));
}

TEMPLATE_TEST_CASE("unary negation", "[mat][neg]", NDML_TEST_TYPES)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = 5;
	m[2, 2] = -6;

	auto const n = -m;

	REQUIRE(n[0, 0] == static_cast<TestType>(-5));
	REQUIRE(n[2, 2] == static_cast<TestType>(6));
}

TEMPLATE_TEST_CASE("matrix addition", "[mat][add]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("addition assignment", "[mat][add]", NDML_TEST_TYPES)
{
	auto lhs  = mat3x3<TestType>();
	lhs[0, 0] = 1;

	auto rhs  = mat3x3<TestType>();
	rhs[0, 0] = 5;

	lhs += rhs;

	REQUIRE(lhs[0, 0] == 6);
}

TEMPLATE_TEST_CASE("matrix subtraction", "[mat][sub]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("subtraction assignment", "[mat][sub]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("multiplication of compatible matrices", "[mat][mul]", NDML_TEST_TYPES)
{
	if constexpr (std::floating_point<TestType>)
	{
		static constexpr auto check_for_c_r_k = []<std::size_t C, std::size_t R, std::size_t K>
		{
			auto const a = generate_mat<C, R, TestType, 64, false>();
			auto const b = generate_mat<K, C, TestType, 64, false>();

			auto const p = a * b;

			for (auto i = 0uz; i < a.row_count; ++i)
			{
				for (auto j = 0uz; j < b.column_count; ++j)
				{
					auto element = TestType{0};

					for (auto k = 0uz; k < a.column_count; ++k)
					{
						element += a[k, i] * b[j, k];
					}

					REQUIRE_THAT(element, WithinRelTypeBound<TestType>(p[i, j]));
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
}

TEMPLATE_TEST_CASE("identity matrix multiplication", "[mat][mul][identity]", NDML_TEST_TYPES)
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

TEMPLATE_TEST_CASE("multiplication assignment", "[mat][mul]", NDML_TEST_TYPES)
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


TEMPLATE_TEST_CASE("multiply matrix by vector", "[mat][mul][vec]", NDML_TEST_TYPES)
{
	auto       m = generate_mat<3, 3, TestType>();
	auto const v = vec<3, TestType>{generate_number<TestType>(), generate_number<TestType>(), generate_number<TestType>()};

	auto const res = m * v;

	REQUIRE(res[0] == dot(v, vec<3, TestType>{m[0, 0], m[1, 0], m[2, 0]}));
}

TEMPLATE_TEST_CASE("matrix by scalar", "[mat][mul][scalar]", NDML_TEST_TYPES)
{
	auto m  = mat3x3<TestType>();
	m[0, 0] = 2;
	m[1, 1] = 3;

	auto const p = m * 4;

	REQUIRE(p[0, 0] == 8);
	REQUIRE(p[1, 1] == 12);
}

TEMPLATE_TEST_CASE("scalar by matrix", "[mat][mul][scalar]", NDML_TEST_TYPES)
{
	auto m  = mat3x3<TestType>{};
	m[1, 1] = 5;

	auto const p = 10 * m;

	REQUIRE(p[1, 1] == 50);
}

TEMPLATE_TEST_CASE("scalar multiply assignment", "[mat][mul][scalar]", NDML_TEST_TYPES)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = 2;
	m[1, 1] = 3;

	m *= 5;

	REQUIRE(m[0, 0] == 10);
	REQUIRE(m[1, 1] == 15);
}

TEMPLATE_TEST_CASE("divide matrix by scalar", "[mat][div]", NDML_TEST_TYPES)
{
	if constexpr (std::is_floating_point_v<TestType>)
	{
		auto const m = mat3x3<TestType>{generate_vec<3, TestType>()};
		auto const q = m / 5;
		for (auto i = 0uz; i < m.column_count; ++i)
		{
			for (auto j = 0uz; j < m.row_count; ++j)
			{
				REQUIRE_THAT((q[i, j]), (WithinRelTypeBound<TestType>(m[i, j] / 5)));
			}
		}
	}
}

TEMPLATE_TEST_CASE("transpose swaps rows and columns", "[mat][trans]", NDML_TEST_TYPES)
{
	auto       m = generate_mat<4, 4, TestType>();
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

TEMPLATE_TEST_CASE("double transpose is identity transform", "[mat][trans]", NDML_TEST_TYPES)
{
	auto const m = generate_mat<4, 4, TestType>();
	REQUIRE(m == transpose(transpose(m)));
}


TEMPLATE_TEST_CASE("row echelon on identity is unchanged", "[mat][ref]", NDML_TEST_TYPES)
{
	auto const identity = mat3x3<TestType>{1};
	REQUIRE(row_echelon_form(identity) == identity);
}


TEMPLATE_TEST_CASE("determinant of 1x1 is the element", "[mat][det]", NDML_TEST_TYPES)
{
	auto const m = generate_mat<1, 1, TestType>();
	REQUIRE(determinant(m) == m[0][0]);
}

TEMPLATE_TEST_CASE("determinant of 2x2 identity is 1", "[mat][det]", NDML_TEST_TYPES)
{
	auto const m = mat2x2<TestType>{1};
	REQUIRE_THAT(determinant(m), WithinRelTypeBound(1));
}

TEMPLATE_TEST_CASE("trace sums diagonal", "[mat][trc]", NDML_TEST_TYPES)
{
	auto m  = mat3x3<TestType>{};
	m[0, 0] = 1;
	m[1, 1] = 2;
	m[2, 2] = 3;

	REQUIRE_THAT(trace(m), WithinRelTypeBound(6));
}

TEMPLATE_TEST_CASE("inverse of identity is identity", "[mat][inv]", NDML_TEST_TYPES)
{
	if constexpr (std::is_floating_point_v<TestType>)
	{
		auto const identity = mat4x4<TestType>{1};
		REQUIRE(identity == inverse(identity));
	}
}

TEMPLATE_TEST_CASE("inverse * matrix = identity", "[mat][inv]", NDML_TEST_TYPES)
{
	if constexpr (std::is_floating_point_v<TestType>)
	{
		auto const m = generate_mat<4, 4, TestType, 256>();
		if (WithinRelTypeBound(0.0).match(determinant(m)))
		{
			return;
		}

		if (row_echelon_form(m) != mat4x4<TestType>{1})
		{
			return;
		}

		auto const prod = m * inverse(m);

		for (auto i = 0uz; i < mat2x2<TestType>::column_count; ++i)
		{
			for (auto j = 0uz; j < mat2x2<TestType>::row_count; ++j)
			{
				REQUIRE_THAT((prod[i, j]), WithinRelTypeBound(i == j ? 1 : 0));
			}
		}
	}
}


TEMPLATE_TEST_CASE("construct 4x4 from 2x2", "[mat][convert]", NDML_TEST_TYPES)
{
	if constexpr (std::is_floating_point_v<TestType>)
	{
		auto small  = mat2x2<TestType>{};
		small[0, 0] = generate_number<TestType>();

		auto large = mat4x4<TestType>(small);

		REQUIRE(large[0, 0] == small[0, 0]);
	}
}

TEMPLATE_TEST_CASE("construct from different type", "[mat][convert]", NDML_TEST_TYPES)
{
	if constexpr (std::is_floating_point_v<TestType>)
	{
		auto src  = mat2x2<std::int16_t>{};
		src[0, 0] = 5;

		auto const dst = mat2x2<TestType>{src};

		REQUIRE(dst[0, 0] == 5);
	}
}
}
