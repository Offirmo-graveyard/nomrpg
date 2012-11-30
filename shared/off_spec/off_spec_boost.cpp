
#include "off_spec_boost.hpp"

#include <cmath> // for correct overloads of abs for non int types


template<>
float off_spec_max_distance_considered_close(const boost::posix_time::ptime& useless_param_just_for_easy_templating)
{
	return 1000.; // 1 sec
}

template<>
float off_spec_distance(const boost::posix_time::ptime& elem1, const boost::posix_time::ptime& elem2)
{
	boost::posix_time::time_duration diff = elem2 - elem1;
	// here I had a hard time to suppress warnings !
	long interval = diff.total_milliseconds();
	long abs_interval = labs(interval);
	return static_cast<float>(abs_interval);
}
