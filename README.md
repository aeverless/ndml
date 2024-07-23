# ndml

**ndml** (_n-dimensional mathematics library_) is a general purpose header-only C++23 linear algebra library with a focus on simplicity and standard-compliance.

Main features include _vectors_, _matrices_, and _quaternions_, operations over them and transformations.

## Library overview

### Vectors

The library's implementation of a mathematical vector is `ndml::vec<N, T>`, given `N` - number of elements, and `T` - element type.

Vector components can be accessed either directly via member variables `x`, `y`, `z`, and `w`, or via a subscript operator taking a component index as its sole parameter. Indexing an out of range component will result in `std::out_of_range` thrown.

Vectors are supported for up to four dimensions so that each component may be accessed via struct member variables.

#### Operations

- dot product;
- cross product;
- norm calculation;
- normalization;
- projection;
- reciprocal calculation;
- general transformation.

### Matrices

The library's implementation of a matrix is `ndml::mat<R, C, T>`, given `R` - number of rows, `C` - number of columns, and `T` - element type. Matrices are column-major, so given a matrix `m`, `m[c]` will return a reference to column of `m` at index `c`.

Multi-parameter subscript is also available, so `m[c, r]` will return a reference to the element at the intersection of column `c` and row `r`.

Note that there is no boundary checks for indexing columns, and so it is undefined behavior to access an out of range column.

#### Operations

- transposition;
- row echelon form calculation;
- determinant calculation;
- inverse calculation via Gauss-Jordan elimination;
- trace calculation.

#### Transformations

- vector cross matrix;
- vector outer product matrix;
- scale matrix;
- translation matrix;
- rotation matrices;
- look-at matrix;
- orthogonal projection matrix;
- perspective projection matrix.

### Quaternions

The library's implementation of a quaternion is `ndml::quat<T>`, given `T` - element type.

They are represented as an extension of four-dimensional vectors, and so vector operations are also available to them.

#### Operations

- conjugate calculation;
- inverse calculation;
- axis-angle extraction;
- Hamilton multiplication of quaternions;
- conjugation of a vector by a quaternion.

#### Transformations

- construction of a versor (unit quaternion) from axis and angle;
- conversion of a quaternion to an identical three-dimensional rotation matrix.

### Metaprogramming

The library provides several features usable in metaprogramming, such as a burn type and assignment functors.

#### Burn type

Burn type is available as `ndml::meta::burn_t<T>`, where `T` is any type.

It can be constructed from any parameters, but cannot be assigned to. It is used in place of unusable vector components, e.g. the Z or W components for one- or two-dimensional vectors. Trying to use this type in almost any manner will result in a compile-time error.

#### Assignment functors

Assignment functors are also available for use. These are functors that provide a static binary `operator()` such that when it's called, an assignment operator is called for its parameters.

For instance, `ndml::meta::addition_assignment<T, U>` will perform `lhs += rhs` when invoked.

## Requirements

The following requirements must be met to use the library:

- C++23 support;
- CMake 3.26 or newer.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for the contribution guidelines.

## License

The library is licensed under the MIT License.
A copy of the license is available in the [LICENSE](LICENSE) file.

## Authors

A.A.A. (contact at aeverless dot dev)
