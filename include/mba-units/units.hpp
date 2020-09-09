#pragma once

#include <cmath> //sqrt, cos, sin, tan ...

namespace mba::units {

// For initializing a Unit where you are too lazy to specify the exact type
struct UGen {
	double value;
};

namespace detail {

// CRTP implementing common functionality for all unit types
template<class T>
struct CommonUnitBase {
	double value{};

	constexpr CommonUnitBase() noexcept = default;
	constexpr CommonUnitBase( UGen v ) noexcept
		: value{v.value} {};

	constexpr explicit CommonUnitBase( double v ) noexcept
		: value{v} {};

	// #### compund assignment operators ####
	// offset
	// clang-format off
	constexpr T& operator+=( T other ) noexcept { value += other.value; return *static_cast<T*>( this ); }
	constexpr T& operator-=( T other ) noexcept { value -= other.value;	return *static_cast<T*>( this ); }

	// scaling
	constexpr T& operator*=( double other )	noexcept { value *= other; return *static_cast<T*>( this ); }
	constexpr T& operator/=( double other )	noexcept { value /= other; return *static_cast<T*>( this ); }
	// clang-format on

	// #### biniary operators ####
	friend constexpr auto operator+( T l, T r ) noexcept -> T { return T{l.value + r.value}; }
	friend constexpr auto operator-( T l, T r ) noexcept -> T { return T{l.value - r.value}; }

	friend constexpr auto operator-( T l ) noexcept -> T { return T{-l.value}; }
	friend constexpr auto operator+( T l ) noexcept -> T { return l; }

	friend constexpr auto operator*( T l, double r ) noexcept -> T { return T{l.value * r}; }
	friend constexpr auto operator*( double l, T r ) noexcept -> T { return T{l * r.value}; }

	// scaling
	friend constexpr auto operator/( T l, double r ) noexcept -> T { return T{l.value / r}; }
	// NOTE: double/Unit may not always make sense, so not implemented here

	friend constexpr auto abs( T l ) noexcept { return T( l.value < 0.0 ? -l.value : l.value ); }
	friend constexpr auto max( T l, T r ) noexcept { return T( l.value > r.value ? l.value : r.value ); }
	friend constexpr auto min( T l, T r ) noexcept { return T( l.value < r.value ? l.value : r.value ); }
	friend inline auto    fmod( T l, T r ) noexcept { return T( std::fmod( l.value, r.value ) ); }

	// comparison operators
	friend constexpr bool operator<( T l, T r ) noexcept { return {l.value < r.value}; }
	friend constexpr bool operator>( T l, T r ) noexcept { return {l.value > r.value}; }
	friend constexpr bool operator==( T l, T r ) noexcept { return {l.value == r.value}; }
	friend constexpr bool operator!=( T l, T r ) noexcept { return {l.value != r.value}; }
	friend constexpr bool operator<=( T l, T r ) noexcept { return {l.value <= r.value}; }
	friend constexpr bool operator>=( T l, T r ) noexcept { return {l.value >= r.value}; }
};

} // namespace detail

// ##### Definition of user facing types ###############

template<int k, int m, int s>
struct Unit : detail::CommonUnitBase<Unit<k, m, s>> {
	using Base = detail::CommonUnitBase<Unit<k, m, s>>;

	using Base::Base;
};

// Diminsionless type can be implicitly converted to and from plain double
template<>
struct Unit<0, 0, 0> : detail::CommonUnitBase<Unit<0, 0, 0>> {
	using Base = detail::CommonUnitBase<Unit<0, 0, 0>>;

	using Base::Base;

	constexpr Unit( double v ) noexcept
		: Base{v} {};

	operator double() const noexcept { return value; }
};

struct UAngle : detail::CommonUnitBase<UAngle> {
	using Base = detail::CommonUnitBase<UAngle>;

	using Base::Base;

	constexpr explicit UAngle( double v ) noexcept
		: Base( v ){};
	constexpr explicit UAngle( Unit<0, 0, 0> v ) noexcept
		: Base( v.value ){};
	constexpr operator Unit<0, 0, 0>() const noexcept { return value; }

	friend constexpr double operator/( UAngle l, UAngle r ) noexcept { return l.value / r.value; }
};

//##### Operator overloads that involve different types #####

template<int k1, int m1, int s1, int k2, int m2, int s2>
constexpr auto operator*( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r ) noexcept -> Unit<k1 + k2, m1 + m2, s1 + s2>
{
	return Unit<k1 + k2, m1 + m2, s1 + s2>{l.value * r.value};
};

template<int k1, int m1, int s1, int k2, int m2, int s2>
constexpr auto operator/( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r ) noexcept -> Unit<k1 - k2, m1 - m2, s1 - s2>
{
	return Unit<k1 - k2, m1 - m2, s1 - s2>{l.value / r.value};
};

template<int k2, int m2, int s2>
constexpr auto operator/( double l, Unit<k2, m2, s2> r ) noexcept -> Unit<0 - k2, 0 - m2, 0 - s2>
{
	return Unit<-k2, -m2, -s2>{l / r.value};
};

// ######## more complex mathematical operations #############

template<int k, int m, int s>
constexpr bool canTakeSqrt( Unit<k, m, s> = {} ) noexcept
{
	return ( ( k & 0x1 ) == 0 ) && ( ( m & 0x1 ) == 0 ) && ( ( s & 0x1 ) == 0 );
}

template<int k, int m, int s>
constexpr auto sqrt( Unit<k, m, s> l ) noexcept -> Unit<( k / 2 ), ( m / 2 ), ( s / 2 )>
{
	static_assert( canTakeSqrt( Unit<k, m, s>{} ), "Base units are not a power of 2" );
	return Unit<( k / 2 ), ( m / 2 ), ( s / 2 )>( std::sqrt( l.value ) );
}

template<int k, int m, int s>
constexpr auto square( Unit<k, m, s> l ) noexcept -> Unit<( k * 2 ), ( m * 2 ), ( s * 2 )>
{
	return Unit<( k * 2 ), ( m * 2 ), ( s * 2 )>( l.value * l.value );
}

inline double cos( UAngle l ) noexcept
{
	return std::cos( l.value );
}

inline double sin( UAngle l ) noexcept
{
	return std::sin( l.value );
}

inline double tan( UAngle l ) noexcept
{
	return std::tan( l.value );
}

inline UAngle atan2( double Y, double X ) noexcept
{
	return UAngle{std::atan2( Y, X )};
}

// helper types to get the result type of a mathematical operation on units
namespace _unit_impl {

template<class U1, class U2>
struct UDivide {
};

template<class U1, class U2>
struct UMultiply {
};

template<int k1, int m1, int s1, int k2, int m2, int s2>
struct UDivide<Unit<k1, m1, s1>, Unit<k2, m2, s2>> {
	using type = Unit<k1 - k2, m1 - m2, s1 - s2>;
};

template<int k1, int m1, int s1>
struct UDivide<Unit<k1, m1, s1>, double> {
	using type = Unit<k1, m1, s1>;
};

template<int k2, int m2, int s2>
struct UDivide<double, Unit<k2, m2, s2>> {
	using type = Unit<-k2, -m2, -s2>;
};

template<>
struct UDivide<UAngle, UAngle> {
	using type = double;
};

template<>
struct UDivide<UAngle, double> {
	using type = UAngle;
};

template<int k1, int m1, int s1, int k2, int m2, int s2>
struct UMultiply<Unit<k1, m1, s1>, Unit<k2, m2, s2>> {
	using type = Unit<k1 + k2, m1 + m2, s1 + s2>;
};

template<int k2, int m2, int s2>
struct UMultiply<double, Unit<k2, m2, s2>> {
	using type = Unit<k2, m2, s2>;
};

template<int k1, int m1, int s1>
struct UMultiply<Unit<k1, m1, s1>, double> {
	using type = Unit<k1, m1, s1>;
};

template<>
struct UMultiply<UAngle, double> {
	using type = UAngle;
};

template<>
struct UMultiply<double, UAngle> {
	using type = UAngle;
};

} // namespace _unit_impl

template<class U1, class U2>
using UDivide_t = typename _unit_impl::UDivide<U1, U2>::type;

template<class U1, class U2>
using UMultiply_t = typename _unit_impl::UMultiply<U1, U2>::type;

template<class U1>
using UInverse_t = UDivide_t<Unit<0, 0, 0>, U1>;

//##### Operator overload for Angle #####

namespace _detail_angle {

constexpr long double pi_internal    = 3.141592653589793238462643383279502884L;
constexpr long double rad_per_degree = pi_internal / 180.0L;
constexpr double      pi             = static_cast<double>( pi_internal );
constexpr double      two_pi         = static_cast<double>( 2 * pi_internal );

constexpr double normNegPiPi( double angle ) noexcept
{
	if( angle > -pi && angle < pi ) {
		return angle;
	}
	while( angle > pi ) {
		angle -= two_pi;
	}
	while( angle < -pi ) {
		angle += two_pi;
	}
	return angle;
}

constexpr double normNeg2Pi2Pi( double angle ) noexcept
{
	if( angle > -two_pi && angle < two_pi ) {
		return angle;
	}
	while( angle > two_pi ) {
		angle -= two_pi;
	}
	while( angle < -two_pi ) {
		angle += two_pi;
	}
	return angle;
}

} // namespace _detail_angle

constexpr UAngle pi = UAngle{(double)_detail_angle::pi_internal};

/*
 * normalize angle to interval [-pi, pi]
 */
constexpr UAngle normNegPiPi( UAngle angle ) noexcept
{
	return UAngle{_detail_angle::normNegPiPi( angle.value )};
}

/*
 * normalize angle to interval [- 2*pi, 2*pi]
 *
 * contrary to normNegPiPi, this preserves the sign
 */
constexpr UAngle normNeg2Pi2Pi( UAngle angle ) noexcept
{
	return UAngle{_detail_angle::normNeg2Pi2Pi( angle.value )};
}

// Convenience definitions
inline namespace default_unit_definitions {

using UNone   = Unit<0, 0, 0>;
using UMass   = Unit<1, 0, 0>;
using UPos    = Unit<0, 1, 0>;
using UTime   = Unit<0, 0, 1>;
using UHerz   = Unit<0, 0, -1>;
using USpeed  = Unit<0, 1, -1>;
using UAccel  = Unit<0, 1, -2>;
using UForce  = Unit<1, 1, -2>;
using UTorque = Unit<1, 2, -2>;
using ::mba::units::UGen;

template<class BaseUnit>
using UPseudoEnergy = decltype( BaseUnit{} * BaseUnit{} * UTime{} );

} // namespace default_unit_definitions

namespace litterals {

// clang-format off
constexpr UMass		operator""_kg	( long double t ) noexcept { return UMass { (double)t }; };
constexpr UPos		operator""_m	( long double t ) noexcept { return UPos  { (double)t }; };
constexpr UTime		operator""_s	( long double t ) noexcept { return UTime { (double)t }; };
constexpr UHerz		operator""_hz	( long double t ) noexcept { return UHerz { (double)t }; };
constexpr USpeed	operator""_mps	( long double t ) noexcept { return USpeed{ (double)t }; };
constexpr UAccel	operator""_mps2	( long double t ) noexcept { return UAccel{ (double)t }; };
constexpr UForce	operator""_n	( long double t ) noexcept { return UForce{ (double)t }; };
constexpr UAngle	operator""_rad	( long double t ) noexcept { return UAngle{ (double)t }; };
constexpr UAngle	operator""_deg	( long double t ) noexcept { return UAngle{ (double)(t*_detail_angle::rad_per_degree) };	};
// clang-format on

} // namespace litterals

inline UAngle atan2( UPos Y, UPos X )
{
	return UAngle{std::atan2( Y.value, X.value )};
}

} // namespace mba::units
