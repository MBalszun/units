#include <mba-units/fmt.hpp>

namespace {

template<class Unit>
void print( std::ostream& out, Unit u )
{
	out << u << std::endl;
	out << sformat( u ) << std::endl;
}

// so far only check compilation
void foo( std::ostream& out )
{
	using namespace mba::units;
	print( out, UMass{1.0} );
	print( out, UPos{1.0} );
	print( out, UTime{1.0} );

	print( out, UForce{1.0} );
	print( out, UAngle{1.0} );
	print( out, UNone{-1.0} );
}

} // namespace

// template<int k, int m, int s>
// struct FormattedUnit {
//	Unit<k, m, s> u;
//};
//
// struct FormattedAngle {
//	UAngle u;
//};
//
// template<int k, int m, int s>
// FormattedUnit<k, m, s> sformat( Unit<k, m, s> value )
//{
//	return {value};
//}
//
// inline FormattedAngle sformat( UAngle value )
//{
//	return {value};
//}
//
// template<int k, int m, int s>
// std::ostream& operator<<( std::ostream& out, const FormattedUnit<k, m, s>& u )
//{
//	out << u.u.value << "";
//	// clang-format off
//	if constexpr( k != 0 ) {
//		out << "kg";
//		if constexpr( k != 1 ) { out << "^" << k; }
//		if constexpr( m != 0 || s != 0 ) {out << "_"; }
//	}
//	if constexpr( m != 0 ) {
//		out << "m";
//		if constexpr( m != 1 ) { out << "^" << m; }
//		if constexpr( s != 0 ) { out << "_"; }
//	}
//	if constexpr( s != 0 ) {
//		out << "s";
//		if constexpr( s != 1 ) { out << "^" << s; }
//	}
//	//clang-format on
//	return out;
//}
//
// template<int k, int m, int s>
// std::ostream& operator<<( std::ostream& out, const Unit<k, m, s> u )
//{
//	out << u.value;
//	return out;
//}
//
// inline std::ostream& operator<<( std::ostream& out, const UAngle u )
//{
//	out << u.value;
//	return out;
//}
//
// inline std::ostream& operator<<( std::ostream& out, const FormattedAngle u )
//{
//	out << u.u << "rad";
//	return out;
//}
