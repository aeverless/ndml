#include <cmath>
#include <limits>

namespace ndml
{
template <typename T>
constexpr auto conjugate(quat<T> const& q) noexcept -> quat<T>
{
	return {-q.x, -q.y, -q.z, q.w};
}

template <typename T>
constexpr auto inverse(quat<T> const& q) noexcept -> quat<T>
{
	return conjugate(q) / norm_squared(q);
}

template <typename T>
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

template <typename T>
constexpr auto operator*=(quat<T>& lhs, quat<T> const& rhs) noexcept -> quat<T>&
{
	lhs = lhs * rhs;
	return lhs;
}

template <typename T>
constexpr auto operator*(quat<T> const& lhs, quat<T> const& rhs) noexcept -> quat<T>
{
	return {
		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
		lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
	};
}

template <typename T>
constexpr auto operator*(quat<T> const& q, vec<3, T> const& v) noexcept -> vec<3, T>
{
	vec<3, T> const imag{q.x, q.y, q.z};
	vec<3, T> const ort{cross(imag, v)};

	return v + static_cast<T>(2) * (q.w * ort + cross(imag, ort));
}
}
