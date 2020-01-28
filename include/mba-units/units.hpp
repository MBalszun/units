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

	constexpr CommonUnitBase() {}
	constexpr CommonUnitBase( UGen v )
		: value{v.value} {};

	constexpr explicit CommonUnitBase( double v )
		: value{v} {};

	// #### compund assignment operators ####
	// offset
	constexpr T& operator+=( T other )
	{
		value += other.value;
		return *static_cast<T*>( this );
	}
	constexpr T& operator-=( T other )
	{
		value -= other.value;
		return *static_cast<T*>( this );
	}

	// scaling
	constexpr T& operator*=( double other )
	{
		value *= other;
		return *static_cast<T*>( this );
	}
	constexpr T& operator/=( double other )
	{
		value /= other;
		return *static_cast<T*>( this );
	}

	// #### biniary operators ####
	friend constexpr auto operator+( T l, T r ) -> T { return T{l.value + r.value}; }
	friend constexpr auto operator-( T l, T r ) -> T { return T{l.value - r.value}; }

	friend constexpr auto operator-( T l ) -> T { return T{-l.value}; }
	friend constexpr auto operator+( T l ) -> T { return l; }

	friend constexpr auto operator*( T l, double r ) -> T { return T{l.value * r}; }
	friend constexpr auto operator*( double l, T r ) -> T { return T{l * r.value}; }

	// scaling
	friend constexpr auto operator/( T l, double r ) -> T { return T{l.value / r}; }
	// NOTE: double/Unit may not always make sense, so not implemented here

	friend constexpr auto abs( T l ) { return T( l.value < 0.0 ? -l.value : l.value ); }
	friend constexpr auto max( T l, T r ) { return T( l.value > r.value ? l.value : r.value ); }
	friend constexpr auto min( T l, T r ) { return T( l.value < r.value ? l.value : r.value ); }

	// comparison operators
	friend constexpr bool operator<( T l, T r ) { return {l.value < r.value}; }
	friend constexpr bool operator>( T l, T r ) { return {l.value > r.value}; }
	friend constexpr bool operator==( T l, T r ) { return {l.value == r.value}; }
	friend constexpr bool operator!=( T l, T r ) { return {l.value != r.value}; }
	friend constexpr bool operator<=( T l, T r ) { return {l.value <= r.value}; }
	friend constexpr bool operator>=( T l, T r ) { return {l.value >= r.value}; }
};

} // namespace detail

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

	constexpr Unit( double v )
		: Base{v} {};

	operator double() const { return value; }
};

struct UAngle : detail::CommonUnitBase<UAngle> {
	using Base = detail::CommonUnitBase<UAngle>;

	using Base::Base;

	constexpr explicit UAngle( double v )
		: Base( v ){};
	constexpr explicit UAngle( Unit<0, 0, 0> v )
		: Base( v.value ){};
	constexpr operator Unit<0, 0, 0>() const { return value; }

	friend constexpr double operator/( UAngle l, UAngle r ) { return l.value / r.value; }
};

//##### Operator overloads that involve different types or are #####
// operators, where the units can be different and the result (may) be another type yet again
template<int k1, int m1, int s1, int k2, int m2, int s2>
constexpr auto operator*( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r ) -> Unit<k1 + k2, m1 + m2, s1 + s2>
{
	return Unit<k1 + k2, m1 + m2, s1 + s2>{l.value * r.value};
};

template<int k1, int m1, int s1, int k2, int m2, int s2>
constexpr auto operator/( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r ) -> Unit<k1 - k2, m1 - m2, s1 - s2>
{
	return Unit<k1 - k2, m1 - m2, s1 - s2>{l.value / r.value};
};

template<int k2, int m2, int s2>
constexpr auto operator/( double l, Unit<k2, m2, s2> r ) -> Unit<0 - k2, 0 - m2, 0 - s2>
{
	return Unit<-k2, -m2, -s2>{l / r.value};
};

// more complex mathematical operations
template<int k, int m, int s>
constexpr bool canTakeSqrt( Unit<k, m, s> = {} )
{
	return ( ( k & 0x1 ) == 0 ) && ( ( m & 0x1 ) == 0 ) && ( ( s & 0x1 ) == 0 );
}

template<int k, int m, int s>
constexpr auto sqrt( Unit<k, m, s> l ) -> Unit<( k / 2 ), ( m / 2 ), ( s / 2 )>
{
	static_assert( canTakeSqrt( Unit<k, m, s>{} ), "Base units are not a power of 2" );
	return Unit<( k / 2 ), ( m / 2 ), ( s / 2 )>( std::sqrt( l.value ) );
}

template<int k, int m, int s>
constexpr auto square( Unit<k, m, s> l ) -> Unit<( k * 2 ), ( m * 2 ), ( s * 2 )>
{
	return Unit<( k * 2 ), ( m * 2 ), ( s * 2 )>( l.value * l.value );
}

inline double cos( UAngle l )
{
	return std::cos( l.value );
}

inline double sin( UAngle l )
{
	return std::sin( l.value );
}

inline UAngle atan2( double Y, double X )
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

} // namespace _unit_impl

template<class U1, class U2>
using UDivide_t = typename _unit_impl::UDivide<U1, U2>::type;

template<class U1, class U2>
using UMultiply_t = typename _unit_impl::UMultiply<U1, U2>::type;

template<class U1>
using UInverse_t = UDivide_t<Unit<0,0,0>, U1>;

//##### Operator overload for Angle #####

namespace _detail_angle {

constexpr long double pi_internal = 3.141592653589793238462643383279502884;
constexpr double      pi          = static_cast<double>( pi_internal );
constexpr double      pi2         = static_cast<double>( 2 * pi_internal );

constexpr double normNegPiPi( double angle )
{
	if( angle > -pi && angle < pi ) {
		return angle;
	}
	while( angle > pi ) {
		angle -= pi2;
	}
	while( angle < -pi ) {
		angle += pi2;
	}
	return angle;
}

constexpr double normNeg2Pi2Pi( double angle )
{
	if( angle > -pi2 && angle < pi2 ) {
		return angle;
	}
	while( angle > pi2 ) {
		angle -= pi2;
	}
	while( angle < -pi2 ) {
		angle += pi2;
	}
	return angle;
}

} // namespace _detail_angle

constexpr UAngle pi = UAngle{(double)_detail_angle::pi_internal};

constexpr UAngle normNegPiPi( UAngle angle )
{
	return UAngle{_detail_angle::normNegPiPi( angle.value )};
}

constexpr UAngle normNeg2Pi2Pi( UAngle angle )
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
constexpr UMass		operator""_kg	( long double t ) { return UMass { (double)t }; };
constexpr UPos		operator""_m	( long double t ) {	return UPos  { (double)t }; };
constexpr UTime		operator""_s	( long double t ) {	return UTime { (double)t }; };
constexpr UHerz		operator""_hz	( long double t ) {	return UHerz { (double)t }; };
constexpr USpeed	operator""_mps	( long double t ) { return USpeed{ (double)t }; };
constexpr UAccel	operator""_mps2	( long double t ) {	return UAccel{ (double)t }; };
constexpr UForce	operator""_n	( long double t ) {	return UForce{ (double)t };	};
constexpr UAngle	operator""_rad	( long double t ) {	return UAngle{ (double)t };	};
constexpr UAngle	operator""_deg	( long double t ) {	return UAngle{ (double)(t/_detail_angle::pi_internal) };	};
// clang-format on

} // namespace litterals


inline UAngle atan2( UPos Y, UPos X )
{
	return UAngle{std::atan2( Y.value, X.value )};
}




} // namespace mba::units
