namespace ndml::meta
{
template <typename T>
template <typename... Ts>
constexpr burn_t<T>::burn_t(Ts const&...) noexcept
{
}
}
