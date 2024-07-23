#ifndef NDML_FWD_HPP
#define NDML_FWD_HPP

#include <cstddef>

namespace ndml
{
template <std::size_t N, typename T>
struct vec;

template <std::size_t R, std::size_t C, typename T>
struct mat;

template <typename T>
struct quat;
}

#endif
