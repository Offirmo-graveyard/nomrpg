
#include "off_spec_wt_model.hpp"

#ifdef USE_WT
// WDate
template<>
float off_spec_max_distance_considered_close(const Wt::WDateTime& useless_param_just_for_easy_templating)
{
	return 1.0; // 1 sec
}

template<>
float off_spec_distance(const Wt::WDateTime& elem1, const Wt::WDateTime& elem2)
{
	int interval = elem1.secsTo(elem2);
	return static_cast<float>(abs(interval));
}

#endif // USE_WT
