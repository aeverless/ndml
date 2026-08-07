export module ndml.mat:transforms;

import :types;
import :operations;
import std;

export namespace ndml
{
/**
 * @brief Vector cross product matrix.
 *
 * This calculates a matrix @c m such that multiplying it by a vector @c u
 * gives cross product of @p v and @c u.
 *
 * @tparam T element type
 *
 * @param v vector
 *
 * @return the cross matrix of @p v
 */
template <typename T>
[[nodiscard]]
constexpr auto cross_matrix(vec<3, T> const& v) noexcept -> mat<3, 3, T>
{
	auto const& v0 = v[0];
	auto const& v1 = v[1];
	auto const& v2 = v[2];

	return {
		vec<3, T>{T{0},   v2,  -v1},
		vec<3, T>{ -v2, T{0},   v0},
		vec<3, T>{  v1,  -v0, T{0}},
	};
}

/**
 * @brief Vector outer product matrix.
 *
 * This calculates the outer product matrix of @p lhs and @p rhs.
 *
 * @tparam T element type
 *
 * @param lhs left-side vector
 * @param rhs right-side vector
 *
 * @return the outer product of @p lhs and @p rhs
 */
template <std::size_t N, std::size_t M, typename T>
[[nodiscard]]
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

/**
 * @brief Scale matrix.
 *
 * This calculates the matrix such that multiplying it by a vector @c u
 * gives a vector @c u scaled component-wise by a vector @c v.
 *
 * @tparam T element type
 *
 * @param v scale vector
 *
 * @return the scale matrix of @p v
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto scale(vec<N, T> const& v) noexcept -> mat<N + 1, N + 1, T>
{
	mat<N + 1, N + 1, T> s;
	s[N, N] = T{1};

	for (auto i = 0uz; i < N; ++i)
	{
		s[i, i] = v[i];
	}

	return s;
}

/**
 * @brief Translation matrix.
 *
 * This calculates the matrix such that multiplying it by a vector @c u
 * gives a vector @f$ u + v @f$.
 *
 * @tparam T element type
 *
 * @param v translation vector
 *
 * @return the translation matrix of @p v
 */
template <std::size_t N, typename T>
[[nodiscard]]
constexpr auto translation(vec<N, T> const& v) noexcept -> mat<N + 1, N + 1, T>
{
	mat<N + 1, N + 1, T> t{1};

	for (auto i = 0uz; i < N; ++i)
	{
		t[N, i] = v[i];
	}

	return t;
}

/**
 * @brief Two-dimensional rotation matrix.
 *
 * This calculates the matrix such that multiplying it by a two-dimensional homogeneous vector @c u
 * gives a vector @c u rotated @c angle radians in the two-dimensional Cartesian plane.
 *
 * @tparam T element type
 *
 * @param angle angle in radians
 *
 * @return the two-dimensional rotation by angle @p angle matrix
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto rotation(T const& angle) noexcept -> mat<3, 3, T>
{
	auto const cos_angle = static_cast<T>(std::cos(angle));
	auto const sin_angle = static_cast<T>(std::sin(angle));

	return {
		vec<3, T>{ cos_angle, sin_angle, T{0}},
		vec<3, T>{-sin_angle, cos_angle, T{0}},
		vec<3, T>{      T{0},      T{0}, T{1}},
	};
}

/**
 * @brief Three-dimensional rotation matrix.
 *
 * This calculates the matrix such that multiplying it by a three-dimensional homogeneous vector @c u
 * gives a vector @c u rotated @c angle radians along the axis @p axis.
 *
 * @tparam T element type
 *
 * @param axis  rotation axis
 * @param angle angle in radians
 *
 * @pre @p axis is normalized
 *
 * @return the three-dimensional rotation by angle @p angle along axis @p axis matrix
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto rotation(vec<3, T> const& axis, typename vec<3, T>::value_type const& angle) noexcept -> mat<4, 4, T> pre(is_normal(axis))
{
	auto const cos_angle = std::cos(angle);
	auto const sin_angle = std::sin(angle);
	auto const cross     = cross_matrix(axis);

	auto r  = mat<4, 4, T>{mat<3, 3, T>{1} + (T{1} - cos_angle) * cross * cross + sin_angle * cross};
	r[3, 3] = T{1};

	return r;
}

/**
 * @brief Look-at matrix.
 *
 * This calculates the change of basis matrix such that multiplying it by a vector @p u
 * gives the representation of @p u in the standard orthonormal basis.
 *
 * @tparam T element type
 *
 * @param eye    sight origin
 * @param target sight target
 * @param up     normal sight up direction
 *
 * @pre @p up is normalized
 * @pre @p eye and @p target do not coincide
 * @pre @p up and @code target - eye @endcode are not parallel
 *
 * @return the look-at matrix pointing at @p target from @p eye with its up direction equal to @p up
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto look_at(vec<3, T> const& eye, vec<3, T> const& target, vec<3, T> const& up) noexcept
	-> mat<4, 4, T> pre(is_normal(up) && eye != target && !is_zero(cross(target - eye, up)))
{
	auto const f{normal(target - eye)};
	auto const r{normal(cross(f, up))};
	auto const u{cross(r, f)};

	return {
		vec<4, T>{r.x, u.x, -f.x, T{0}},
		vec<4, T>{r.y, u.y, -f.y, T{0}},
		vec<4, T>{r.z, u.z, -f.z, T{0}},
		vec<4, T>{-dot(r, eye), -dot(u, eye), dot(f, eye), T{1}},
	};
}

/**
 * @brief Orthogonal projection matrix.
 *
 * This calculates the orthogonal projection matrix delimited by near and far planes and rectangular bounds.
 *
 * @tparam T element type
 *
 * @param left   left bound
 * @param right  right bound
 * @param bottom bottom bound
 * @param up     up bound
 * @param near   near plane depth
 * @param far    far plane depth
 *
 * @return orthogonal projection matrix
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto ortho(T const& left, T const& right, T const& bottom, T const& up, T const& near, T const& far) noexcept -> mat<4, 4, T>
{
	auto const dx{right - left};
	auto const dy{up - bottom};
	auto const dz{far - near};

	return {
		vec<4, T>{           T{2} / dx,                T{0},               T{0}, T{0}},
		vec<4, T>{                T{0},           T{2} / dy,               T{0}, T{0}},
		vec<4, T>{                T{0},                T{0},         -T{2} / dz, T{0}},
		vec<4, T>{-(right + left) / dx, -(up + bottom) / dy, -(far + near) / dz, T{1}},
	};
}

/**
 * @brief Perspective projection matrix.
 *
 * This calculates the perspective projection matrix delimited by near and far planes
 * given a vertical FOV and aspect ratio.
 *
 * @tparam T element type
 *
 * @param vertical_fov vertical field of view in radians
 * @param aspect_ratio width to height ratio
 * @param near         near plane depth
 * @param far          far plane depth
 *
 * @return perspective projection matrix
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto perspective(T const& vertical_fov, T const& aspect_ratio, T const& near, T const& far) noexcept -> mat<4, 4, T>
{
	T const tan_half_fov{std::tan(vertical_fov / T{2})};

	auto const dx{tan_half_fov * aspect_ratio};
	auto const dy{tan_half_fov};
	auto const dz{far - near};

	return {
		vec<4, T>{T{1} / dx,      T{0},                    T{0},  T{0}},
		vec<4, T>{     T{0}, T{1} / dy,                    T{0},  T{0}},
		vec<4, T>{     T{0},      T{0},      -(far + near) / dz, -T{1}},
		vec<4, T>{     T{0},      T{0}, -T{2} * far * near / dz,  T{0}},
	};
}
}
