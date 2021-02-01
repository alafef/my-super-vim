#pragma once
#include "model.h"

class Controller;

class ModeManager
{
	
public:
	ModeManager(Controller* controller)
		:controller(controller){};
	Controller* controller;

	void NAV_mode();
	void NAV_parse();

	void CMD_mode();
	void CMD_parse();

	void INS_mode();

	void SEA_mode();
	void SEA_parse(int set);
};