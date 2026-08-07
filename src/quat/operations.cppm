export module ndml.quat:operations;

import :types;
import std;

export namespace ndml
{
/**
 * @brief Conjugate of a quaternion.
 *
 * This calculates a quaternion of form @f$ (-x, -y, -z, w) @f$.
 *
 * @return the conjugate of @p q
 */
template <typename T>
[[nodiscard]]
constexpr auto conjugate(quat<T> const& q) noexcept -> quat<T>
{
	return {static_cast<T>(-q.x), static_cast<T>(-q.y), static_cast<T>(-q.z), q.w};
}

/**
 * @brief Inverse of a quaternion.
 *
 * This calculates a quaternion such that multiplication of @p q by it
 * results in an identity quaternion.
 *
 * @pre @p q is not zero
 *
 * @return the conjugate of @p q divided by its squared norm
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto inverse(quat<T> const& q) noexcept -> quat<T> pre(!is_zero(q))
{
	return conjugate(q) / norm_squared(q);
}

/**
 * @brief Axis and angle of a versor.
 *
 * This extracts the axis and angle out of a versor.
 * For quaternions with an imaginary part nearing zero, return value is value-initialized.
 *
 * @return the axis-angle pair of @p q
 */
template <std::floating_point T>
[[nodiscard]]
constexpr auto axis_angle(quat<T> const& q) noexcept -> std::pair<vec<3, T>, T>
{
	vec<3, T> const imag{q.x, q.y, q.z};

	auto const imag_norm = norm(imag);

	if (imag_norm <= std::numeric_limits<T>::epsilon())
	{
		return {};
	}

	return {imag / imag_norm, 2 * std::atan2(imag_norm, q.w)};
}

/**
 * @brief Quaternion Hamilton multiplication assignment operator.
 *
 * Calculates the Hamilton product of @p lhs and @p rhs and assigns the result to @p lhs.
 */
template <typename T>
constexpr auto operator*=(quat<T>& lhs, quat<T> const& rhs) noexcept -> quat<T>&
{
	lhs = lhs * rhs;
	return lhs;
}

/**
 * @brief Quaternion Hamilton multiplication operator.
 *
 * Calculates the Hamilton product of @p lhs and @p rhs.
 */
template <typename T>
[[nodiscard]]
constexpr auto operator*(quat<T> const& lhs, quat<T> const& rhs) noexcept -> quat<T>
{
	return {
		static_cast<T>(lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y),
		static_cast<T>(lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x),
		static_cast<T>(lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w),
		static_cast<T>(lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z),
	};
}

/**
 * @brief Conjugation of a vector by quaternion.
 *
 * Conjugates @p v by @p q, effectively rotating @p v if @p q is a versor.
 *
 * @pre @p q is normalized
 */
template <typename T>
[[nodiscard]]
constexpr auto operator*(quat<T> const& q, vec<3, T> const& v) noexcept -> vec<3, T> pre(is_normal(q))
{
	vec<3, T> const imag{q.x, q.y, q.z};
	vec<3, T> const ort{cross(imag, v)};

	return v + static_cast<T>(2) * (q.w * ort + cross(imag, ort));
}
}
