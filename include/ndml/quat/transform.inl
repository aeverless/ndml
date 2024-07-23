#include <limits>

namespace ndml
{
template <typename T>
constexpr auto versor(vec<3, T> const& axis, typename vec<3, T>::value_type const& angle) noexcept -> quat<T>
{
	static constexpr auto half{static_cast<T>(1) / static_cast<T>(2)};

	auto const half_angle = half * angle;
	return {axis * std::sin(half_angle), std::cos(half_angle)};
}

template <typename T>
constexpr auto rotation(quat<T> const& q) noexcept -> mat<4, 4, T>
{
	auto const [axis, angle] = axis_angle(q);
	if (angle <= std::numeric_limits<T>::epsilon())
	{
		return mat<4, 4, T>{1};
	}

	return rotation(axis, angle);
}
}
