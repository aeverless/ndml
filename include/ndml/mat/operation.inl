namespace ndml
{
template <std::size_t C, std::size_t R, typename T>
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

template <std::size_t N, typename T>
constexpr auto row_echelon_form(mat<N, N, T> const& m) noexcept -> mat<N, N, T>
{
	mat ref{m};

	for (auto i = 0uz; i < ref.column_count; ++i)
	{
		for (std::size_t j = i + 1; j < ref.row_count; ++j)
		{
			auto const scale = ref[i, j] / ref[i, i];

			for (std::size_t k = i; k < ref.column_count; ++k)
			{
				ref[k, j] -= scale * ref[k, i];
			}
		}
	}

	return ref;
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

template <std::size_t N, typename T>
constexpr auto determinant(mat<N, N, T> const& m) noexcept -> mat<N, N, T>::value_type
{
	T det{1};

	auto const ref = row_echelon_form(m);
	for (auto i = 0uz; i < N; ++i)
	{
		det *= ref[i, i];
	}

	return det;
}

template <typename T>
constexpr auto inverse(mat<1, 1, T> const& m) noexcept -> mat<1, 1, T>
{
	return {T{1} / m[0, 0]};
}

template <typename T>
constexpr auto inverse(mat<2, 2, T> const& m) noexcept -> mat<2, 2, T>
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

template <std::size_t N, typename T>
constexpr auto inverse(mat<N, N, T> const& m) noexcept -> mat<N, N, T>
{
	mat<N, N, T> inv{1};
	mat          id{m};

	for (auto i = 0uz; i < id.column_count; ++i)
	{
		auto const pivot = id[i, i];

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

template <std::size_t N, typename T>
constexpr auto trace(mat<N, N, T> const& m) noexcept -> mat<N, N, T>::value_type
{
	T tr{};

	for (auto i = 0uz; i < m.column_count; ++i)
	{
		tr += m[i, i];
	}

	return tr;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator+=(mat<C, R, T>& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>&
{
	for (auto i = 0uz; i < C; ++i)
	{
		lhs[i] += rhs[i];
	}

	return lhs;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator-=(mat<C, R, T>& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>&
{
	for (auto i = 0uz; i < C; ++i)
	{
		lhs[i] -= rhs[i];
	}

	return lhs;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*=(mat<C, R, T>& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>&
{
	lhs = lhs * rhs;
	return lhs;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*=(mat<C, R, T>& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>&
{
	std::ranges::for_each(m, [&scale](auto& column) { return column *= scale; });
	return m;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator/=(mat<C, R, T>& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>&
{
	std::ranges::for_each(m, [&scale](auto& column) { return column /= scale; });
	return m;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator==(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> bool
{
	return std::ranges::equal(lhs, rhs);
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator!=(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> bool
{
	return !(lhs == rhs);
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator+(mat<C, R, T> const& m) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	for (auto& column : tmp)
	{
		column = +column;
	}

	return tmp;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator-(mat<C, R, T> const& m) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	for (auto& column : tmp)
	{
		column = -column;
	}

	return tmp;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator+(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>
{
	auto tmp{lhs};
	return tmp += rhs;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator-(mat<C, R, T> const& lhs, mat<C, R, T> const& rhs) noexcept -> mat<C, R, T>
{
	auto tmp{lhs};
	return tmp -= rhs;
}

template <std::size_t N, std::size_t M, std::size_t K, typename T>
constexpr auto operator*(mat<N, M, T> const& lhs, mat<K, N, T> const& rhs) noexcept -> mat<M, K, T>
{
	mat<M, K, T> p;

	for (auto i = 0uz; i < lhs.row_count; ++i)
	{
		for (auto j = 0uz; j < rhs.column_count; ++j)
		{
			for (auto k = 0uz; k < lhs.column_count; ++k)
			{
				p[i, j] += lhs[k, i] * rhs[j, k];
			}
		}
	}

	return p;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*(mat<C, R, T> const& m, vec<R, T> const& v) noexcept -> vec<R, T>
{
	vec<R, T> p;

	for (std::size_t c = 0; c < m.column_count; ++c)
	{
		for (std::size_t r = 0; r < m.row_count; ++r)
		{
			p[c] += m[c, r] * v[r];
		}
	}

	return p;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*(mat<C, R, T> const& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	return tmp *= scale;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator*(typename mat<C, R, T>::value_type const& scale, mat<C, R, T> const& m) noexcept -> mat<C, R, T>
{
	return m * scale;
}

template <std::size_t C, std::size_t R, typename T>
constexpr auto operator/(mat<C, R, T> const& m, typename mat<C, R, T>::value_type const& scale) noexcept -> mat<C, R, T>
{
	auto tmp{m};
	return tmp /= scale;
}
}
