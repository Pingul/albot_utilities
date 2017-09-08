#include <string>
#include "battleships.hh"

battleship::Logic logic; // We store some state, so it needs to be 

std::string playMove(const std::string& state)
{
	logic.parseBoardState(state);
	auto action = logic.nextAction();
	return action;
}

void initSettings()
{
	logic.reset();
}