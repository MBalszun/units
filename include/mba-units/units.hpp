#pragma once

#include <chrono>
#include <cmath>
#include <type_traits>

namespace mba::units {

// For initializing a Unit where you are too lazy to specify the exact type
struct UGen {
	double value;
};

template<int k, int m, int s>
struct Unit {
	using TYPE = Unit<k, m, s>;
	double value;

	constexpr Unit()
		: value{}
	{
	}

	constexpr Unit( UGen v )
		: value{v.value} {};

	constexpr explicit Unit( double v )
		: value( v ){};

	constexpr Unit<k, m, s>& operator+=( Unit<k, m, s> other )
	{
		value += other.value;
		return *this;
	}

	constexpr Unit<k, m, s>& operator-=( Unit<k, m, s> other )
	{
		value -= other.value;
		return *this;
	}

	// we can't specify operator*= and /= in terms of Unit<0, 0, 0>
	// as specialization for that type follows later
	constexpr Unit<k, m, s>& operator*=( double other )
	{
		value *= other;
		return *this;
	}

	constexpr Unit<k, m, s>& operator/=( double other )
	{
		value /= other;
		return *this;
	}
};

// Diminsionless type can be implicitly converted to and from plain double
template<>
struct Unit<0, 0, 0> {
	using TYPE = Unit<0, 0, 0>;
	double value;
	constexpr Unit()
		: value{}
	{
	}

	constexpr Unit( UGen v )
		: value{v.value} {};

	constexpr Unit( double v )
		: value( v ){};

	constexpr Unit( const Unit<0, 0, 0>& other ) = default;

	operator double() const { return value; }

	constexpr Unit<0, 0, 0>& operator=( const Unit<0, 0, 0>& other ) = default;

	constexpr Unit<0, 0, 0>& operator=( double other )
	{
		value = other;
		return *this;
	}

	constexpr Unit<0, 0, 0>& operator=( UGen other )
	{
		value = other.value;
		return *this;
	}

	constexpr Unit<0, 0, 0>& operator+=( Unit<0, 0, 0> other )
	{
		value += other.value;
		return *this;
	}

	constexpr Unit<0, 0, 0>& operator-=( Unit<0, 0, 0> other )
	{
		value -= other.value;
		return *this;
	}

	constexpr Unit<0, 0, 0>& operator*=( Unit<0, 0, 0> other )
	{
		value *= other.value;
		return *this;
	}

	constexpr Unit<0, 0, 0>& operator/=( Unit<0, 0, 0> other )
	{
		value /= other.value;
		return *this;
	}
};

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

namespace litterals {

// clang-format off
constexpr UMass		operator""_kg	( long double t ) { return UMass { (double)t }; };
constexpr UPos		operator""_m	( long double t ) {	return UPos  { (double)t }; };
constexpr UTime		operator""_s	( long double t ) {	return UTime { (double)t }; };
constexpr UHerz		operator""_hz	( long double t ) {	return UHerz { (double)t }; };
constexpr USpeed	operator""_mps	( long double t ) { return USpeed{ (double)t }; };
constexpr UAccel	operator""_mps2	( long double t ) {	return UAccel{ (double)t }; };
constexpr UForce	operator""_n	( long double t ) {	return UForce{ (double)t };	};
// clang-format on

} // namespace litterals

template<class Rep, class Period>
constexpr UTime from_std_duration( std::chrono::duration<Rep, Period> d )
{
	using std_float_dur_type = std::chrono::duration<double, std::ratio<1>>;
	return UTime{std::chrono::duration_cast<std_float_dur_type>( d ).count()};
}

} // namespace default_unit_definitions

//##### Operator overload for Unit #####
// operators, where the units can be different and the result (may) be another type yet again
template<int k1, int m1, int s1, int k2, int m2, int s2>
constexpr Unit<k1 + k2, m1 + m2, s1 + s2> operator*( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r )
{
	return Unit<k1 + k2, m1 + m2, s1 + s2>{l.value * r.value};
};

template<int k1, int m1, int s1, int k2, int m2, int s2>
constexpr Unit<k1 - k2, m1 - m2, s1 - s2> operator/( Unit<k1, m1, s1> l, Unit<k2, m2, s2> r )
{
	return Unit<k1 - k2, m1 - m2, s1 - s2>{l.value / r.value};
};

// operations, where both operators have to be of the same type
template<int k, int m, int s>
constexpr auto operator+( Unit<k, m, s> l, Unit<k, m, s> r ) -> Unit<k, m, s>
{
	return Unit<k, m, s>{l.value + r.value};
}

template<int k, int m, int s>
constexpr auto operator-( Unit<k, m, s> l, Unit<k, m, s> r ) -> Unit<k, m, s>
{
	return Unit<k, m, s>{l.value - r.value};
}

// operations with scalars
template<int k, int m, int s>
constexpr auto operator*( Unit<k, m, s> l, double r ) -> Unit<k, m, s>
{
	return Unit<k, m, s>{l.value * r};
}

template<int k, int m, int s>
constexpr auto operator*( double l, Unit<k, m, s> r ) -> Unit<k, m, s>
{
	return Unit<k, m, s>{l * r.value};
}

template<int k, int m, int s>
constexpr auto operator/( Unit<k, m, s> l, double r ) -> Unit<k, m, s>
{
	return Unit<k, m, s>{l.value / r};
}

template<int k, int m, int s>
constexpr auto operator/( double l, Unit<k, m, s> r ) -> Unit<-k, -m, -s>
{
	return Unit<-k, -m, -s>{l / r.value};
}

// unary operators
template<int k, int m, int s>
constexpr auto operator-( Unit<k, m, s> l ) -> Unit<k, m, s>
{
	return Unit<k, m, s>{-l.value};
}

template<int k, int m, int s>
constexpr auto operator+( Unit<k, m, s> l ) -> Unit<k, m, s>
{
	return l;
}

// relational operators
template<int k, int m, int s>
constexpr bool operator<( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return {l.value < r.value};
}

template<int k, int m, int s>
constexpr bool operator>( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return {l.value > r.value};
}

template<int k, int m, int s>
constexpr bool operator==( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return {l.value == r.value};
}

template<int k, int m, int s>
constexpr bool operator!=( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return {l.value != r.value};
}

template<int k, int m, int s>
constexpr bool operator<=( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return {l.value <= r.value};
}

template<int k, int m, int s>
constexpr bool operator>=( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return {l.value >= r.value};
}

// helper types to get the result of a mathematical operation on units
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
using UInverse_t = UDivide_t<UNone, U1>;

// more complex mathematical operations
template<int k, int m, int s>
constexpr bool canTakeSqrt( Unit<k, m, s> = {} )
{
	return ( ( k & 0x1 ) == 0 ) && ( ( m & 0x1 ) == 0 ) && ( ( s & 0x1 ) == 0 );
}

// c++14: constexpr auto sqrt(Unit<k, m, s> l) -> std::enable_if_t < canTakeSqrt(Unit<k, m, s>{}), Unit < (k / 2), (m /
// 2), (s / 2) >> ::type{
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

template<int k, int m, int s>
constexpr auto abs( Unit<k, m, s> l )
{
	return Unit<k, m, s>( std::abs( l.value ) );
}

template<int k, int m, int s>
constexpr auto max( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return Unit<k, m, s>( l.value > r.value ? l.value : r.value );
}

template<int k, int m, int s>
constexpr auto min( Unit<k, m, s> l, Unit<k, m, s> r )
{
	return Unit<k, m, s>( l.value < r.value ? l.value : r.value );
}

//##### Operator overload for Angle #####

namespace _detail_angle {

constexpr long double pi_internal = 3.141592653589793238462643383279502884;
constexpr double      pi          = static_cast<double>( pi_internal );
constexpr double      pi2         = static_cast<double>( 2 * pi_internal );

constexpr auto modulo2Pi( const double& a )
{
	return a - ( pi2 * static_cast<long long>( a / pi2 ) );
}

constexpr double normNegPiPi( double angle )
{
	// TODO: use c++14 constexpr syntax
	return angle < -pi ? normNegPiPi( angle + pi2 ) : angle > pi ? normNegPiPi( angle - pi2 ) : angle;
}
constexpr double normNeg2Pi2Pi( double angle )
{
	// TODO: use c++14 constexpr syntax
	return normNegPiPi( modulo2Pi( angle ) );
}

} // namespace _detail_angle

struct UAngle {
	double value;
	constexpr UAngle()
		: value{}
	{
	}
	constexpr explicit UAngle( double v )
		: value( v ){};
	constexpr explicit UAngle( Unit<0, 0, 0> v )
		: value( v.value ){};
	constexpr operator Unit<0, 0, 0>() const { return value; }

	constexpr UAngle& operator+=( UAngle other )
	{
		value += other.value;
		return *this;
	}

	constexpr UAngle& operator-=( UAngle other )
	{
		value -= other.value;
		return *this;
	}

	constexpr UAngle& operator*=( double other )
	{
		value *= other;
		return *this;
	}

	constexpr UAngle& operator/=( double other )
	{
		value /= other;
		return *this;
	}
};

constexpr UAngle pi = static_cast<UAngle>( (double)_detail_angle::pi_internal );

constexpr UAngle normNegPiPi( UAngle angle )
{
	return UAngle{_detail_angle::normNegPiPi( angle.value )};
}

constexpr UAngle normNeg2Pi2Pi( UAngle angle )
{
	return UAngle{_detail_angle::normNeg2Pi2Pi( angle.value )};
}

// clang-format off
// binary math operators
constexpr auto operator*( UAngle l, double r ) { return UAngle{l.value * r}; }
constexpr auto operator*( double l, UAngle r ) { return UAngle{l * r.value}; }

constexpr auto operator/( UAngle l, UAngle r ) { return l.value / r.value;	 }
constexpr auto operator/( UAngle l, double r ) { return UAngle{l.value / r}; }
constexpr auto operator/( double l, UAngle r ) { return UAngle{l / r.value}; }

constexpr auto operator+( UAngle l, UAngle r ){	return UAngle{l.value + r.value}; }
constexpr auto operator-( UAngle l, UAngle r ){	return UAngle{l.value - r.value}; }

// unary math operators
constexpr auto operator-( UAngle angle ){ return UAngle{-angle.value}; }
constexpr auto operator+( UAngle angle ){ return angle; }

// comparison operators
constexpr bool operator< ( UAngle l, UAngle r ) { return l.value < r.value;  }
constexpr bool operator> ( UAngle l, UAngle r ) { return l.value > r.value;	 }
constexpr bool operator==( UAngle l, UAngle r ) { return l.value == r.value; }
constexpr bool operator!=( UAngle l, UAngle r ) { return l.value != r.value; }
constexpr bool operator<=( UAngle l, UAngle r ) { return l.value <= r.value; }
constexpr bool operator>=( UAngle l, UAngle r )	{ return l.value >= r.value; }

// clang-format on

inline auto abs( UAngle l )
{
	return UAngle( std::abs( l.value ) );
}

inline auto cos( UAngle l )
{
	return std::cos( l.value );
}

inline auto sin( UAngle l )
{
	return std::sin( l.value );
}

inline UAngle atan2( UPos Y, UPos X )
{
	return UAngle{std::atan2( Y.value, X.value )};
}

} // namespace mba::units
