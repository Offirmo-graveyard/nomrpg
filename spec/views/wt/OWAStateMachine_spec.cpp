#include "off_spec/off_spec.hpp"

#include "OWAStateMachine.hpp"

DESCRIBE_CLASS(OWAStateMachine)
{

/* simply cast the state machine to the expected state
 * - returns a ref on the state if actual state (ignored with void)
 * - throw a bad_cast else
 */
#define STATE_SHOULD_BE(state_machine, state) (void)state_machine.state_cast< const state & >()

	IT(is_instantiable)
	{
		TRACE_CURRENT_TEST();

		OWAStateMachine sm;
		//CHECK(false); // fails
	}

	IT(starts_in_correct_state)
	{
		TRACE_CURRENT_TEST();

		OWAStateMachine sm;
		sm.initiate();

		// check if it is in the correct state at creation
		STATE_SHOULD_BE(sm, Landing);
		// just to test the exceptions check syntax
		CHECK_THROW((void)sm.state_cast< const Debug & >(), std::bad_cast);

	}

	IT(has_a_proper_landing_state)
	{
		TRACE_CURRENT_TEST();

		OWAStateMachine sm;
		sm.initiate();

		// check if it is in the correct state at creation
		(void)sm.state_cast< const Landing & >();

		// check if it responds to signal and switch to the correct state
		sm.process_event( EvReqDebug() );
		(void)sm.state_cast< const Debug & >();

		// should be able to switch back
		sm.process_event( EvReqHome() );
		(void)sm.state_cast< const Landing & >();

		// check if it responds to signal and switch to the correct state
		sm.process_event( EvReqApp() );
		(void)sm.state_cast< const App & >();

		// should be able to switch back
		sm.process_event( EvReqHome() );
		(void)sm.state_cast< const Landing & >();

		// check if it responds to signal and switch to the correct state
		sm.process_event( EvReqMisc() );
		(void)sm.state_cast< const Misc & >();

		// should be able to switch back
		sm.process_event( EvReqHome() );
		(void)sm.state_cast< const Landing & >();
	}

	IT(has_a_proper_app_state)
	{
		TRACE_CURRENT_TEST();

		OWAStateMachine sm;
		sm.initiate();

		// check if it is in the correct state at creation
		(void)sm.state_cast< const Landing & >();

		// switch to app state
		sm.process_event( EvReqApp() );
		(void)sm.state_cast< const App & >();

		// should be able to switch to debug
		sm.process_event( EvReqDebug() );
		(void)sm.state_cast< const Debug & >();

		// switch back to app state
		sm.process_event( EvReqHome() );
		sm.process_event( EvReqApp() );
		(void)sm.state_cast< const App & >();

		// should be able to switch to misc
		sm.process_event( EvReqMisc() );
		(void)sm.state_cast< const Misc & >();

		// switch back to app state
		sm.process_event( EvReqHome() );
		sm.process_event( EvReqApp() );
		(void)sm.state_cast< const App & >();
	}

	IT(has_a_proper_misc_state)
	{
		TRACE_CURRENT_TEST();

		OWAStateMachine sm;
		sm.initiate();

		// check if it is in the correct state at creation
		(void)sm.state_cast< const Landing & >();

		// switch to misc state
		sm.process_event( EvReqMisc() );
		(void)sm.state_cast< const Misc & >();

		// should be able to switch to debug
		sm.process_event( EvReqDebug() );
		(void)sm.state_cast< const Debug & >();

		// switch back to misc state
		sm.process_event( EvReqHome() );
		sm.process_event( EvReqMisc() );
		(void)sm.state_cast< const Misc & >();

		// should be able to switch to app
		sm.process_event( EvReqApp() );
		(void)sm.state_cast< const App & >();

		// switch back to misc state
		sm.process_event( EvReqMisc() );
		(void)sm.state_cast< const Misc & >();
	}
}
