#pragma once
#define VIM_CONTROLLER_H
#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "mode_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "curses.h"
#include "panel.h"
class Controller
{
public:
	Model model;
	ModeManager* mode;
	Controller();
	bool exit = false;
	bool file_modified = false;
	void arrows(int symbol);
	void pages(int symbol);
	MyString cur_file;
	void open_file(const char* file_name);
	void save_to_cur_file();
	void save_to_new_file(const char* file_name);
	friend class ModeManager;
};