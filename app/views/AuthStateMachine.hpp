#ifndef AUTH_STATE_MACHINE_HPP__
#define AUTH_STATE_MACHINE_HPP__

/*
#include "auth_state_machine.hpp"
 */
#include <iostream>

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>


namespace sc = boost::statechart;

// This define a state machine using boost statechart
// cf. tutorial http://www.boost.org/doc/libs/1_48_0/libs/statechart/doc/tutorial.html

/******* Events *******/
class EvIdentify : public sc::event< EvIdentify > {};
class EvUnidentify : public sc::event< EvUnidentify > {};
class EvAuthentify : public sc::event< EvAuthentify > {};
class EvUnauthentify : public sc::event< EvUnauthentify > {};

/******* States *******/
// We need to forward-declare the states

class Unidentified;
class Identified;

class Unauthentified;
class Authentified;


// Boost.Statechart makes heavy use of the curiously recurring
// template pattern. The deriving class must always be passed as
// the first parameter to all base class templates.
//
// The state machine must be informed which state it has to
// enter when the machine is initiated. That's why Greeting is
// passed as the second template parameter.
class AuthStateMachine : public sc::state_machine< AuthStateMachine, Unidentified >
{};

// For each state we need to define which state machine it
// belongs to and where it is located in the statechart. Both is
// specified with Context argument that is passed to
// simple_state<>. For a flat state machine as we have it here,
// the context is always the state machine. Consequently,
// Machine must be passed as the second template parameter to
// Greeting's base (the Context parameter is explained in more
// detail in the next example).
class Unidentified : public sc::simple_state< Unidentified, AuthStateMachine >
{
public:

	typedef sc::transition< EvIdentify, Identified > reactions;

	// Whenever the state machine enters a state, it creates an
	// object of the corresponding state class. The object is then
	// kept alive as long as the machine remains in the state.
	// Finally, the object is destroyed when the state machine
	// exits the state. Therefore, a state entry action can be
	// defined by adding a constructor and a state exit action can
	// be defined by adding a destructor.
	Unidentified()  { /*std::cout << "In  Unidentified\n";*/ } // entry
	~Unidentified() { /*std::cout << "Out Unidentified\n";*/ } // exit
};

class Identified : public sc::simple_state< Identified, AuthStateMachine, Unauthentified >
{
public:

	typedef sc::transition< EvUnidentify, Unidentified > reactions;

	Identified()  { /*std::cout << "In  Identified\n";*/ } // entry
	~Identified() { /*std::cout << "Out Identified\n";*/ } // exit
};

class Unauthentified : public sc::simple_state< Unauthentified, Identified >
{
public:

	typedef sc::transition< EvAuthentify, Authentified > reactions;

	Unauthentified()  { /*std::cout << "In  Unauthentified\n";*/ } // entry
	~Unauthentified() { /*std::cout << "Out Unauthentified\n";*/ } // exit
};

class Authentified : public sc::simple_state< Authentified, Identified >
{
public:

	typedef sc::transition< EvUnauthentify, Unauthentified > reactions;

	Authentified()  { /*std::cout << "In  Authentified\n";*/ } // entry
	~Authentified() { /*std::cout << "Out Authentified\n";*/ } // exit
};

#endif // AUTH_STATE_MACHINE_HPP__
