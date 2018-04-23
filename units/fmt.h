#pragma once

#include <iostream>
#include "units.h"

namespace units {

	template<int k, int m, int s>
	struct FormattedUnit {
		Unit<k, m, s> u;
	};

	struct FormattedAngle {
		UAngle u;
	};

	template<int k, int m, int s>
	FormattedUnit<k, m, s> sformat(Unit<k, m, s> value) {
		return { value };
	}

	FormattedAngle sformat(UAngle value) {
		return { value };
	}

	template<int k, int m, int s>
	std::ostream& operator<<(std::ostream& out, const FormattedUnit<k, m, s>& u) {
		out << u.u.value << "";
		if constexpr (k == 1) { out << "kg" << " "; } else if constexpr (k != 0) { out << "kg^" << k << " "; }
		if constexpr (m == 1) { out << "m"  << " "; } else if constexpr (m != 0) { out << "m^" << m << " "; }
		if constexpr (s == 1) { out << "s"  << " "; } else if constexpr (s != 0) { out << "s^" << s << " "; }
		std::cout << "";

		return out;
	}

	template<int k, int m, int s>
	std::ostream& operator<<(std::ostream& out, const Unit<k, m, s> u) {
		out << u.value;
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const UAngle u) {
		out << u.value;
		return out;
	}

	std::ostream& operator<<(std::ostream& out, const FormattedAngle u) {
		out << u;
		return out;
	}


}