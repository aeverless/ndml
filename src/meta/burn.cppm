export module ndml.meta:burn;

export namespace ndml::meta
{
/**
 * @brief Burn type.
 *
 * An instance of this type can be constructed from any parameters, but cannot be assigned to.
 * It is used, for example, in @c ndml::vec for representing unusable components.
 */
template <typename>
struct burn_t
{
	constexpr burn_t(burn_t const&) noexcept = default;
	constexpr burn_t(burn_t&&) noexcept      = default;

	/**
	 * @brief Constructor from any parameters.
	 *
	 * It can be used to initialize an instance of burn type with any parameters.
	 *
	 * @tparam Ts types of parameters
	 */
	template <typename... Ts>
	constexpr burn_t(Ts const&...) noexcept
	{}

	/**
	 * @brief Assignment operator.
	 *
	 * It is deleted to disallow assignments.
	 *
	 * @tparam T parameter type
	 */
	template <typename T>
	constexpr auto operator=(T) noexcept -> burn_t& = delete;

	constexpr auto operator=(burn_t const&) noexcept -> burn_t& = delete;
	constexpr auto operator=(burn_t&&) noexcept -> burn_t&      = delete;
};
}
