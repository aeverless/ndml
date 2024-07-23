namespace ndml
{
template <typename T>
constexpr quat<T>::quat(value_type x, value_type y, value_type z, value_type w) noexcept
	: vec<4, T>{std::move(x), std::move(y), std::move(z), std::move(w)}
{
}

template <typename T>
constexpr quat<T>::quat(vec<3, value_type> v, value_type w) noexcept
	: quat{std::move(v.x), std::move(v.y), std::move(v.z), std::move(w)}
{
}

template <typename T>
constexpr quat<T>::quat(vec<4, T> v) noexcept
	: vec<4, value_type>{v}
{
}
}
