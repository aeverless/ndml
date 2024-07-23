#include <cmath>

namespace ndml
{
template <typename T>
constexpr auto cross_matrix(vec<3, T> const& v) noexcept -> mat<3, 3, T>
{
	auto const& v0 = v[0];
	auto const& v1 = v[1];
	auto const& v2 = v[2];

	return {
		vec{T{0},   v2,  -v1},
		vec{ -v2, T{0},   v0},
		vec{  v1,  -v0, T{0}},
	};
}

template <std::size_t N, std::size_t M, typename T>
constexpr auto outer_product(vec<N, T> const& lhs, vec<M, T> const& rhs) noexcept -> mat<M, N, T>
{
	mat<M, N, T> m;

	for (std::size_t row = 0; row < N; ++row)
	{
		for (std::size_t col = 0; col < M; ++col)
		{
			m[col, row] = lhs[row] * rhs[col];
		}
	}

	return m;
}

template <std::size_t N, typename T>
constexpr auto scale(vec<N, T> const& v) noexcept -> mat<N + 1, N + 1, T>
{
	mat<N + 1, N + 1, T> s;
	s[N, N] = T{1};

	for (std::size_t i = 0; i < N; ++i)
	{
		s[i, i] = v[i];
	}

	return s;
}

template <std::size_t N, typename T>
constexpr auto translation(vec<N, T> const& v) noexcept -> mat<N + 1, N + 1, T>
{
	mat<N + 1, N + 1, T> t{T{1}};

	for (std::size_t i = 0; i < N; ++i)
	{
		t[N][i] = v[i];
	}

	return t;
}

template <typename T>
constexpr auto rotation(T const& angle) noexcept -> mat<3, 3, T>
{
	auto const cos_angle = static_cast<T>(std::cos(angle));
	auto const sin_angle = static_cast<T>(std::sin(angle));

	return {
		vec{ cos_angle, sin_angle, T{0}},
		vec{-sin_angle, cos_angle, T{0}},
		vec{      T{0},      T{0}, T{1}},
	};
}

template <typename T>
constexpr auto rotation(vec<3, T> const& axis, typename vec<3, T>::value_type const& angle) noexcept -> mat<4, 4, T>
{
	auto const cos_angle = std::cos(angle);
	auto const sin_angle = std::sin(angle);
	auto const cross     = cross_matrix(axis);

	mat<4, 4, T> r{mat<3, 3, T>{1} + (T{1} - cos_angle) * cross * cross + sin_angle * cross};
	r[3, 3] = T{1};

	return r;
}

template <typename T>
constexpr auto look_at(vec<3, T> const& eye, vec<3, T> const& target, vec<3, T> const& up) noexcept -> mat<4, 4, T>
{
	auto const f{normal(target - eye)};
	auto const r{normal(cross(f, up))};
	auto const u{cross(r, f)};

	return {
		vec{r.x, u.x, -f.x, T{0}},
		vec{r.y, u.y, -f.y, T{0}},
		vec{r.z, u.z, -f.z, T{0}},
		vec{-dot(r, eye), -dot(u, eye), dot(f, eye), T{1}},
	};
}

template <typename T>
constexpr auto ortho(T const& left, T const& right, T const& bottom, T const& up, T const& near, T const& far) noexcept -> mat<4, 4, T>
{
	auto const dx{right - left};
	auto const dy{up - bottom};
	auto const dz{far - near};

	return {
		vec{           T{2} / dx,                T{0},               T{0}, T{0}},
		vec{                T{0},           T{2} / dy,               T{0}, T{0}},
		vec{                T{0},                T{0},         -T{2} / dz, T{0}},
		vec{-(right + left) / dx, -(up + bottom) / dy, -(far + near) / dz, T{1}},
	};
}

template <typename T>
constexpr auto perspective(T const& vertical_fov, T const& aspect_ratio, T const& near, T const& far) noexcept -> mat<4, 4, T>
{
	T const tan_half_fov{std::tan(vertical_fov / T{2})};

	auto const dx{tan_half_fov * aspect_ratio};
	auto const dy{tan_half_fov};
	auto const dz{far - near};

	return {
		vec{T{1} / dx,      T{0},                    T{0},  T{0}},
		vec{     T{0}, T{1} / dy,                    T{0},  T{0}},
		vec{     T{0},      T{0},      -(far + near) / dz, -T{1}},
		vec{     T{0},      T{0}, -T{2} * far * near / dz,  T{0}},
	};
}
}
