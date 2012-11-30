#ifndef BORING_RPG_STATE_MACHINE_HPP__
#define BORING_RPG_STATE_MACHINE_HPP__

/*
#include "boring_rpg_state_machine.hpp"
 */
#include <iostream>

#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>


namespace sc = boost::statechart;

// This define a state machine using boost statechart
// cf. tutorial http://www.boost.org/doc/libs/1_48_0/libs/statechart/doc/tutorial.html

/******* Events *******/
class EvClick : public sc::event< EvClick > {};
class EvWaitTimeElapsed : public sc::event< EvWaitTimeElapsed > {};

/******* States *******/
// We need to forward-declare the states

class Ready;
class Waiting;


class BoringRPGStateMachine : public sc::state_machine< BoringRPGStateMachine, Ready > {};

class Ready : public sc::simple_state< Ready, BoringRPGStateMachine >
{
public:

	typedef sc::transition< EvClick, Waiting > reactions;

	Ready()  { std::cout << "In  Ready\n"; } // entry
	~Ready() { std::cout << "Out Ready\n"; } // exit
};

class Waiting : public sc::simple_state< Waiting, BoringRPGStateMachine >
{
public:

	typedef sc::transition< EvWaitTimeElapsed, Ready > reactions;

	Waiting()  { std::cout << "In  Waiting\n"; } // entry
	~Waiting() { std::cout << "Out Waiting\n"; } // exit
};

#endif // BORING_RPG_STATE_MACHINE_HPP__
