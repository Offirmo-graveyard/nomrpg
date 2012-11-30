#ifndef OFFIRMO_WEB_APP_STATE_MACHINE_HPP__
#define OFFIRMO_WEB_APP_STATE_MACHINE_HPP__

/*
#include "OWAStateMachine.hpp"
 */
#include <iostream>

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
namespace sc = boost::statechart;

#include <boost/mpl/list.hpp>
namespace mpl = boost::mpl;


// This define a state machine using boost statechart
// cf. tutorial http://www.boost.org/doc/libs/1_48_0/libs/statechart/doc/tutorial.html


/******* Events *******/
// names : EvReqHome = Event of Requesting the Home page (most likely by user clicking a link but not always)
class EvReqHome  : public sc::event< EvReqHome > {};
class EvReqDebug : public sc::event< EvReqDebug > {};
class EvReqApp   : public sc::event< EvReqApp > {};
class EvReqMisc  : public sc::event< EvReqMisc > {};


/******* States *******/

// We need to forward-declare the states
class Landing;
class Debug;
class App;
class Misc;


// Boost.Statechart makes heavy use of the curiously recurring
// template pattern. The deriving class must always be passed as
// the first parameter to all base class templates.
//
// The state machine must be informed which state it has to
// enter when the machine is initiated. That's why Greeting is
// passed as the second template parameter.
class OWAStateMachine : public sc::state_machine< OWAStateMachine, Landing >
{};

// For each state we need to define which state machine it
// belongs to and where it is located in the statechart. Both is
// specified with Context argument that is passed to
// simple_state<>. For a flat state machine as we have it here,
// the context is always the state machine. Consequently,
// Machine must be passed as the second template parameter to
// Greeting's base (the Context parameter is explained in more
// detail in the next example).
class Landing : public sc::simple_state< Landing, OWAStateMachine >
{
public:

	typedef mpl::list<
			sc::transition< EvReqDebug, Debug >,
			sc::transition< EvReqApp,   App   >,
			sc::transition< EvReqMisc,  Misc  >
		> reactions;


	// Whenever the state machine enters a state, it creates an
	// object of the corresponding state class. The object is then
	// kept alive as long as the machine remains in the state.
	// Finally, the object is destroyed when the state machine
	// exits the state. Therefore, a state entry action can be
	// defined by adding a constructor and a state exit action can
	// be defined by adding a destructor.
	Landing()  { /*std::cout << "In  Landing\n";*/ } // entry
	~Landing() { /*std::cout << "Out Landing\n";*/ } // exit
};

//
class Debug : public sc::simple_state< Debug, OWAStateMachine >
{
public:

	// this is a special page, only able to go back to home
	typedef mpl::list<
			sc::transition< EvReqHome, Landing >
		> reactions;

	Debug()  { /*std::cout << "In  Debug\n";*/ } // entry
	~Debug() { /*std::cout << "Out Debug\n";*/ } // exit
};

class App : public sc::simple_state< App, OWAStateMachine >
{
public:

	typedef mpl::list<
			sc::transition< EvReqHome,  Landing >,
			sc::transition< EvReqDebug, Debug >,
			sc::transition< EvReqMisc,  Misc  >
		> reactions;

	App()  { /*std::cout << "In  App\n";*/ } // entry
	~App() { /*std::cout << "Out App\n";*/ } // exit
};

class Misc : public sc::simple_state< Misc, OWAStateMachine >
{
public:

	typedef mpl::list<
			sc::transition< EvReqHome,  Landing >,
			sc::transition< EvReqDebug, Debug >,
			sc::transition< EvReqApp,   App   >
		> reactions;

	Misc()  { /*std::cout << "In  Misc\n";*/ } // entry
	~Misc() { /*std::cout << "Out Misc\n";*/ } // exit
};

#endif // OFFIRMO_WEB_APP_STATE_MACHINE_HPP__
