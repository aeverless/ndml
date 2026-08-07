export module ndml.quat:transforms;

import ndml.mat;

import :types;
import :operations;
import std;

export namespace ndml
{
/**
 * @brief Versor from axis and angle.
 *
 * This initializes a quaternion equal to @f$ \cos \frac \theta 2 + u \sin \frac \theta 2 @f$,
 * where @f$ u @f$ is equal to @p axis and @f$ \theta @f$ is equal to @p angle.
 *
 * @param axis  versor axis
 * @param angle angle in radians
 *
 * @pre @p axis is normalized
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto versor(vec<3, T> const& axis, typename vec<3, T>::value_type const& angle) noexcept -> quat<T> pre(is_normal(axis))
{
	static constexpr auto half{static_cast<T>(1) / static_cast<T>(2)};

	auto const half_angle = half * angle;
	return {axis * std::sin(half_angle), std::cos(half_angle)};
}

/**
 * @brief Quaternion to matrix conversion.
 *
 * This extracts axis and angle from @p q and creates a rotation matrix from them.
 *
 * @pre @p q is normalized
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto rotation(quat<T> const& q) noexcept -> mat<4, 4, T> pre(is_normal(q))
{
	auto const [axis, angle] = axis_angle(q);
	if (angle <= std::numeric_limits<T>::epsilon())
	{
		return mat<4, 4, T>{1};
	}

	return rotation(axis, angle);
}
}
