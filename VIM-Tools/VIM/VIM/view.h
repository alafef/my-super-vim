#pragma once
#define VIM_VIEW_H
#include <iostream>
#include "my_string.h"
//#include "MyString.h"
#include "adapter_pdcurses.h"

class View : public AdapterPDcurses
{
public:
	View();
	~View();

	void logo_preline();
	void help_info();
	void panel(char cur_mode, unsigned int cur_line, unsigned int cur_pos,
		unsigned int max_lines, unsigned int max_line_pos, MyString cmd,
		unsigned int display_start_line, MyString tmp, MyString tmp_2, MyString cur_file);

};