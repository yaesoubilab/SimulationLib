#include "IncidenceStatistic.h"

namespace SimulationLib
{

	IncidenceStatistic::IncidenceStatistic(string inName) : name(inName) {
		mean = 0;
		variance = 0;
		min = std::numeric_limits<double>::min();
		max = std::numeric_limits<double>::max();
		total = 0;
		count = 0;
	}

}