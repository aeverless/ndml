export module ndml.mat:operations;

import :types;
import std;

export namespace ndml
{
/**
 * @brief Checks if the matrix is singular.
 *
 * @tparam N matrix dimensions
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return @c true if @c determinant(m) is 0, @c false otherwise
 */
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto is_singular(
	mat<N, N, T> const& m,
	typename mat<N, N, T>::value_type const& tolerance = std::numeric_limits<T>::epsilon() * static_cast<T>(4 * N * N)
) noexcept
	-> bool
{
	return std::abs(determinant(m)) <= tolerance;
}

/**
 * @brief The determinant of a matrix.
 *
 * This calculates the determinant of a matrix.
 *
 * @tparam N matrix dimensions
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the determinant of @p m
 */
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto determinant(mat<N, N, T> const& m) noexcept -> mat<N, N, T>::value_type
{
	auto t{m};
	T    det{1};

	for (auto i = 0uz; i < t.column_count; ++i)
	{
		auto pivot_row = i;

		for (auto j = i + 1; j < t.row_count; ++j)
		{
			if (std::abs(t[i, j]) > std::abs(t[i, pivot_row]))
			{
				pivot_row = j;
			}
		}

		if (i != pivot_row)
		{
			det *= -1;

			for (auto k = 0uz; k < t.column_count; ++k)
			{
				std::swap(t[k, i], t[k, pivot_row]);
			}
		}

		if (t[i, i] == T{})
		{
			return T{};
		}

		for (auto j = i + 1; j < t.row_count; ++j)
		{
			auto const scale = t[i, j] / t[i, i];

			for (auto k = i; k < t.column_count; ++k)
			{
				t[k, j] -= scale * t[k, i];
			}
		}

		det *= t[i, i];
	}

	return det;
}

template <typename T>
constexpr auto determinant(mat<1, 1, T> const& m) noexcept -> mat<1, 1, T>::value_type
{
	return m[0, 0];
}

template <typename T>
constexpr auto determinant(mat<2, 2, T> const& m) noexcept -> mat<2, 2, T>::value_type
{
	auto const& m0 = m[0];
	auto const& m1 = m[1];

	return m0[0] * m1[1] - m0[1] * m1[0];
}

template <typename T>
constexpr auto determinant(mat<3, 3, T> const& m) noexcept -> mat<3, 3, T>::value_type
{
	auto const& m0  = m[0];
	auto const& m00 = m0[0];
	auto const& m01 = m0[1];
	auto const& m02 = m0[2];

	auto const& m1  = m[1];
	auto const& m10 = m1[0];
	auto const& m11 = m1[1];
	auto const& m12 = m1[2];

	auto const& m2  = m[2];
	auto const& m20 = m2[0];
	auto const& m21 = m2[1];
	auto const& m22 = m2[2];

	return (m02 * m10 * m21) + (m00 * m11 * m22) + (m01 * m12 * m20) - (m01 * m10 * m22) - (m02 * m11 * m20) - (m00 * m12 * m21);
}

/**
 * @brief The transpose of a matrix.
 *
 * This calculates the transpose of @p m, i.e. @p m with rows and columns swapped in order.
 *
 * @tparam R number of rows
 * @tparam C number of columns
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the transpose of @p m
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto transpose(mat<C, R, T> const& m) noexcept -> mat<R, C, T>
{
	mat<R, C, T> t;

	for (auto i = 0uz; i < m.column_count; ++i)
	{
		for (auto j = 0uz; j < m.row_count; ++j)
		{
			t[j, i] = m[i, j];
		}
	}

	return t;
}

/**
 * @brief The row echelon form of a non-singular matrix.
 *
 * This calculates the row echelon form of @p m, i.e. @p m with first non-zero entries
 * of all rows forming a staircase pattern.
 *
 * @tparam N matrix dimensions
 * @tparam T element type
 *
 * @param m matrix
 *
 * @pre @p m is not singular.
 *
 * @return the row echelon form of @p m
 */
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto row_echelon_form(mat<N, N, T> const& m) noexcept -> mat<N, N, T> pre(!is_singular(m))
{
	mat ref{m};

	for (auto i = 0uz; i < ref.column_count; ++i)
	{
		auto pivot_row = i;

		for (auto j = i + 1; j < ref.row_count; ++j)
		{
			if (std::abs(ref[i, j]) > std::abs(ref[i, pivot_row]))
			{
				pivot_row = j;
			}
		}

		if (i != pivot_row)
		{
			for (auto k = 0uz; k < ref.column_count; ++k)
			{
				std::swap(ref[k, i], ref[k, pivot_row]);
			}
		}

		auto const pivot = ref[i, i];
		[[assume(pivot != T{})]];

		for (auto j = i + 1; j < ref.row_count; ++j)
		{
			auto const scale = ref[i, j] / pivot;

			for (auto k = i; k < ref.column_count; ++k)
			{
				ref[k, j] -= scale * ref[k, i];
			}
		}
	}

	return ref;
}

/**
 * @brief The inverse of a non-singular matrix.
 *
 * This calculates the inverse of a matrix via a method called Gauss-Jordan elimination.
 *
 * @tparam N matrix dimensions
 * @tparam T element type
 *
 * @param m matrix
 *
 * @pre @p m is not singular.
 *
 * @return the inverse of @p m
 */
template <std::size_t N, std::floating_point T>
[[nodiscard]]
constexpr auto inverse(mat<N, N, T> const& m) noexcept -> mat<N, N, T> pre(!is_singular(m))
{
	mat<N, N, T> inv{1};
	mat          id{m};

	for (auto i = 0uz; i < id.column_count; ++i)
	{
		auto pivot_row = i;

		for (auto j = i + 1; j < id.row_count; ++j)
		{
			if (std::abs(id[i, j]) > std::abs(id[i, pivot_row]))
			{
				pivot_row = j;
			}
		}

		if (i != pivot_row)
		{
			for (auto k = 0uz; k < id.column_count; ++k)
			{
				std::swap(id[k, i], id[k, pivot_row]);
				std::swap(inv[k, i], inv[k, pivot_row]);
			}
		}

		auto const pivot = id[i, i];
		[[assume(pivot != T{})]];

		for (auto j = 0uz; j < id.column_count; ++j)
		{
			id[j, i]  /= pivot;
			inv[j, i] /= pivot;
		}

		for (auto j = 0uz; j < id.row_count; ++j)
		{
			if (i == j)
			{
				continue;
			}

			auto const scale = id[i, j];

			for (auto k = 0uz; k < id.column_count; ++k)
			{
				id[k, j]  -= scale * id[k, i];
				inv[k, j] -= scale * inv[k, i];
			}
		}
	}

	return inv;
}

template <std::floating_point T>
constexpr auto inverse(mat<1, 1, T> const& m) noexcept -> mat<1, 1, T> pre(!is_singular(m))
{
	return mat<1, 1, T>{T{1} / m[0, 0]};
}

template <std::floating_point T>
constexpr auto inverse(mat<2, 2, T> const& m) noexcept -> mat<2, 2, T> pre(!is_singular(m))
{
	auto const& m0  = m[0];
	auto const& m00 = m0[0];
	auto const& m01 = m0[1];

	auto const& m1  = m[1];
	auto const& m10 = m1[0];
	auto const& m11 = m1[1];

	auto const det = m00 * m11 - m01 * m10;

	return mat{
		       vec{ m11, -m01},
		       vec{-m10,  m00},
        } /
	       det;
}

/**
 * @brief The trace of a matrix.
 *
 * This calculates the trace of a matrix by summing up its diagonal elements.
 *
 * @tparam N matrix dimensions
 * @tparam T element type
 *
 * @param m matrix
 *
 * @return the trace of @p m
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto trace(mat<N, N, T> const& m) noexcept -> mat<N, N, T>::value_type
{
	T tr{};

	for (auto i = 0uz; i < m.column_count; ++i)
	{
		tr += m[i, i];
	}

	return tr;
}

/**
 * @brief Matrix addition assignment operator.
 *
 * Adds columns of @p rhs to respective columns of @p lhs.
 */
template <std::size_t C, std::size_t R, typename T>
constexpr auto operator+=(mat<C, R, T>& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>&
{
	for (auto i = 0uz; i < C; ++i)
	{
		lhs[i] += rhs[i];
	}

	return lhs;
}

/**
 * @brief Matrix subtraction assignment operator.
 *
 * Subtracts columns of @p rhs from respective columns of @p lhs.
 */
template <std::size_t C, std::size_t R, typename T>
constexpr auto operator-=(mat<C, R, T>& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>&
{
	for (auto i = 0uz; i < C; ++i)
	{
		lhs[i] -= rhs[i];
	}

	return lhs;
}

/**
 * @brief Matrix multiplication assignment operator.
 *
 * Performs matrix multiplication for @p lhs and @p rhs and assigns the result to @p lhs.
 */
template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*=(mat<C, R, T>& lhs, mat<C, C, T> const& rhs) noexcept -> mat<C, R, T>&
{
	lhs = lhs * rhs;
	return lhs;
}

/**
 * @brief Matrix-scalar multiplication assignment operator.
 *
 * Multiplies columns of @p lhs by @p scale.
 */
template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*=(mat<C, R, T>& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>&
{
	std::ranges::for_each(
		m,
		[&scale](auto& column)
		{
			return column *= scale;
		}
	);
	return m;
}

/**
 * @brief Matrix-scalar division assignment operator.
 *
 * Divides columns of @p lhs by @p scale.
 */
template <std::size_t C, std::size_t R, typename T>
constexpr auto operator/=(mat<C, R, T>& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>& pre(scale != T{})
{
	std::ranges::for_each(
		m,
		[&scale](auto& column)
		{
			return column /= scale;
		}
	);
	return m;
}

/**
 * @brief Matrix equality comparison operator.
 *
 * Compares columns of @p lhs with columns of @p rhs.
 *
 * @return @c true if two columns are equal, @c false otherwise
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator==(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> bool
{
	return std::ranges::equal(lhs, rhs);
}

/**
 * @brief Matrix inequality comparison operator.
 *
 * Compares columns of @p lhs with columns of @p rhs.
 *
 * @return @c false if two columns are equal, @c true otherwise
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator!=(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> bool
{
	return !(lhs == rhs);
}

/**
 * @brief Matrix promotion operator.
 *
 * Returns a copy of @p m with unary promotion applied to each of its columns.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator+(mat<C, R, T> const& m) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	for (auto& column : tmp)
	{
		column = +column;
	}

	return tmp;
}

/**
 * @brief Matrix promotion operator.
 *
 * Returns a copy of @p m with unary negation applied to each of its columns.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator-(mat<C, R, T> const& m) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	for (auto& column : tmp)
	{
		column = -column;
	}

	return tmp;
}

/**
 * @brief Matrix addition operator.
 *
 * Adds columns of @p rhs to respective columns of a copy of @p lhs and returns the result.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator+(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>
{
	auto tmp{lhs};
	return tmp += rhs;
}

/**
 * @brief Matrix subtraction operator.
 *
 * Subtracts columns of @p rhs from respective columns of a copy of @p lhs and returns the result.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator-(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>
{
	auto tmp{lhs};
	return tmp -= rhs;
}

/**
 * @brief Matrix multiplication operator.
 *
 * Performs matrix multiplication for @p lhs and @p rhs and returns the result.
 */
template <std::size_t N, std::size_t M, std::size_t K, typename T>
[[nodiscard]]
constexpr auto operator*(mat<N, M, T> const& lhs, mat<K, N, T> const& rhs) noexcept -> mat<K, M, T>
{
	mat<K, M, T> p{};

	for (auto i = 0uz; i < lhs.row_count; ++i)
	{
		for (auto j = 0uz; j < rhs.column_count; ++j)
		{
			for (auto k = 0uz; k < lhs.column_count; ++k)
			{
				p[j, i] += lhs[k, i] * rhs[j, k];
			}
		}
	}

	return p;
}

/**
 * @brief Matrix-vector multiplication operator.
 *
 * Multiplies @p m by @p v and returns the result.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator*(mat<C, R, T> const& m, vec<C, T> const& v) noexcept -> vec<R, T>
{
	vec<R, T> p;

	for (std::size_t c = 0; c < m.column_count; ++c)
	{
		for (std::size_t r = 0; r < m.row_count; ++r)
		{
			p[r] += m[c, r] * v[c];
		}
	}

	return p;
}

/**
 * @brief Matrix-scalar multiplication operator.
 *
 * Multiplies @p m by @p scale and returns the result.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator*(mat<C, R, T> const& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	return tmp *= scale;
}

/**
 * @brief Scalar-matrix multiplication operator.
 *
 * Multiplies @p m by @p scale and returns the result.
 */
template <std::size_t C, std::size_t R, typename T>
[[nodiscard]]
constexpr auto operator*(typename mat<C, R, T>::value_type const& scale, mat<C, R, T> const& m) noexcept -> mat<C, R, T>
{
	return m * scale;
}

/**
 * @brief Matrix-scalar division operator.
 *
 * Divides @p m by @p scale and returns the result.
 */
template <std::size_t C, std::size_t R, typename T>
constexpr auto operator/(mat<C, R, T> const& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T> pre(scale != T{})
{
	auto tmp{m};
	return tmp /= scale;
}
}
