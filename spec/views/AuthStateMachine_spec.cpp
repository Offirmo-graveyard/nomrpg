#include "off_spec/off_spec.hpp"

#include "AuthStateMachine.hpp"

DESCRIBE_CLASS(AuthStateMachine)
{
/* simply cast the state machine to the expected state
 * - returns a ref on the state if actual state (ignored with void)
 * - throw a bad_cast else
 */
#define STATE_SHOULD_BE(state_machine, state) (void)state_machine.state_cast< const state & >()

	void DisplayStateConfiguration( const AuthStateMachine &sm )
	{
		char region = 'A';

		for (AuthStateMachine::state_iterator pLeafState = sm.state_begin();
			  pLeafState != sm.state_end();
			  ++pLeafState )
		{
			std::cout << "Orthogonal region " << region << " : ";
			// The following use of typeid assumes that
			// BOOST_STATECHART_USE_NATIVE_RTTI is defined
			std::cout << typeid( *pLeafState ).name() << "\n";
			++region;
		}
	}

	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		AuthStateMachine sm;
	}

	IT(starts_in_correct_state)
	{
		TRACE_CURRENT_TEST();

		AuthStateMachine sm;
		sm.initiate();

		// check if it is in the correct state at creation
		STATE_SHOULD_BE(sm, Unidentified);
	}

	IT(has_a_correct_unidentified_state)
	{
		TRACE_CURRENT_TEST();

		AuthStateMachine sm;
		sm.initiate();

		// check if it is in the correct state at creation
		STATE_SHOULD_BE(sm, Unidentified);

		// check if it responds to signal and switch to the correct state
		sm.process_event( EvIdentify() );
		STATE_SHOULD_BE(sm, Identified);

		// should be able to switch back
		sm.process_event( EvUnidentify() );
		STATE_SHOULD_BE(sm, Unidentified);

		// should be able to re-identify
		sm.process_event( EvIdentify() );
		STATE_SHOULD_BE(sm, Identified);
	}

	IT(has_a_correct_Identified_state)
	{
		TRACE_CURRENT_TEST();

		AuthStateMachine sm;
		sm.initiate();

		// switch it to correct state
		sm.process_event( EvIdentify() );
		STATE_SHOULD_BE(sm, Identified);

		// now check inner state
		STATE_SHOULD_BE(sm, Unauthentified);

		//DisplayStateConfiguration(sm);

		// send event to inner
		sm.process_event( EvAuthentify() );
		STATE_SHOULD_BE(sm, Identified);
		STATE_SHOULD_BE(sm, Authentified);

		// switch back
		sm.process_event( EvUnauthentify() );
		STATE_SHOULD_BE(sm, Identified);
		STATE_SHOULD_BE(sm, Unauthentified);

		//DisplayStateConfiguration(sm);
	}
}
