#include <iostream>
#include <typeinfo>

#include "off_spec/off_spec.hpp"
#include "off_spec/off_spec_wt_model.hpp"

#ifdef USE_WT
#include "wt_spec_helpers.hpp"
#endif

#include "RealUser.hpp"
#include "VirtualWorld.hpp"
#include "VirtualWorldGeneralParticipation.hpp"
#include "VirtualWorldInstance.hpp"
#include "VirtualWorldInstanceParticipation.hpp"
#include "Avatar.hpp"




static const std::string TEST_NAME_U1 = "Kevin";
static const std::string TEST_NAME_U2 = "Leeroy Jenkins";

static const std::string TEST_NAME_VW1 = "Global Testing Grounds";
static const std::string   TEST_NAME_VW1I1 = "Beta";
static const std::string     TEST_NAME_VW1I1U1A1 = "Kevin le roxxor";
static const std::string   TEST_NAME_VW1I2 = "Gamma";
static const std::string TEST_NAME_VW2 = "Global Testing Grounds II";
static const std::string   TEST_NAME_VW2I1 = "Beta";
static const std::string   TEST_NAME_VW2I2 = "Zeta";


SUITE(VirtualWorldUseCases)
{
	IT(allows_use_case_1)
	{
		// What is described here
		// is already tested unitely in specific tests.
		// But it is easier for the mind,
		// to make a "big picture" test.

		TRACE_CURRENT_TEST();

		// first a virtual world
		VirtualWorld vw1(TEST_NAME_VW1);

		// then an instance
		VirtualWorldInstance vw1i1(vw1, TEST_NAME_VW1I1);

		// then a user
		RealUser ru1(TEST_NAME_U1);

		// now user1 connects for the 1st time
		// general participation is automatically created for this user
		VirtualWorldGeneralParticipation vwgp_u1vw1(ru1, vw1);
		// does he have a last used instance ?
		// of course not since he never played so far
		SHOULD_EQ(vwgp_u1vw1.get_last_instance_name(), "");

		// TODO test instance auto-selection

		// The user now select an instance
		vwgp_u1vw1.register_instance_selection(vw1i1);

		// is the last used instance correctly remembered ?
		SHOULD_EQ(vwgp_u1vw1.get_last_instance_name(), vw1i1.get_name());
		SHOULD_EQ(vwgp_u1vw1.get_last_instance_name(), TEST_NAME_VW1I1);

		// instance participation is automatically created for this user
		VirtualWorldInstanceParticipation vwip_u1vw1i1(ru1, vw1i1);

		// has the user a last used avatar for this instance ?
		// of course not since he never played so far
		SHOULD_EQ(vwip_u1vw1i1.get_last_avatar_name(), "");

		// the user now creates an avatar
		Avatar av_u1vw1i1(ru1, vw1i1, TEST_NAME_VW1I1U1A1);

		// the user now selects this avatar
		vwip_u1vw1i1.register_avatar_selection(av_u1vw1i1);

		// is the last used avatar correctly remembered ?
		SHOULD_EQ(vwip_u1vw1i1.get_last_avatar_name(), av_u1vw1i1.get_name());
		SHOULD_EQ(vwip_u1vw1i1.get_last_avatar_name(), TEST_NAME_VW1I1U1A1);
	}
}

