/* Specs expectations
 * specific to boost objects (dates, etc.)
 */
/*
#include "off_spec_boost.hpp"
 */

#ifndef OFF_SPEC_BOOST_HPP_INCLUDED__
#define OFF_SPEC_BOOST_HPP_INCLUDED__


#include "off_spec.hpp"

#include "boost/date_time/posix_time/posix_time.hpp"



template<>
float off_spec_max_distance_considered_close(const boost::posix_time::ptime& useless_param_just_for_easy_templating);

template<>
float off_spec_distance(const boost::posix_time::ptime& elem1, const boost::posix_time::ptime& elem2);




#endif // OFF_SPEC_BOOST_HPP_INCLUDED__
