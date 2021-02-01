#pragma once
#include "view.h"
#include "curses.h"
#include "panel.h"

View::View()
{
	AdapterPDcurses();
}

View::~View()
{
	endwindow();
}

void View::panel(char cur_mode, unsigned int cur_line, unsigned int cur_pos,
	unsigned int max_lines, unsigned int max_line_pos, MyString cmd,
	unsigned int display_start_line, MyString tmp, MyString tmp_2, MyString cur_file)
{
	change_color(2);

	if (cur_mode == 'C')
	{
		if (cmd.size() > 0)
			print(max_y - 1, 10, " command:%s", cmd.c_str());
		else
			print(max_y - 1, 10, " command:");
		refresh_screen();
	}

		print(max_y - 1, 0, " ");
		switch (cur_mode)
		{
		case 'N':
			addstr("[NAV]");
			break;
		case 'I':
			addstr("[INS]");
			break;
		case 'S':
			addstr("[SEA]");
		case 'C':
			addstr("[CMD]");
			break;
		default:
			break;
		}
		if (cur_mode != 'C') 
		{
			print(max_y - 1, 6, " cursor %d,%d   max %d,%d    visible %d  %s     %s ",
				cur_line, cur_pos, max_lines, max_line_pos, display_start_line, cmd.c_str(), cur_file.c_str());
		}
		else
		{
			print(max_y - 1, 18, " %s", cmd.c_str());
		}

	change_color(1);
	logo_preline();
	move_cursor(0, 0);
}

void View::logo_preline()
{
	int i = 0;

	print(max_y - 2, 0, "*");
	while (i < max_x/2 - 6)
	{
		print_symbol('*');
		i++;
	}
	print_string("  V  I  M  ");
	i += 12;
	while (i < max_x  )
	{
		print_symbol('*');
		i++;
	}
}

void View::help_info()
{
	clear_screen();
	print(0, 0, " ");
	print(1, 0, " [Navigation and Edit mode] ");
	print(2, 0, " $ - Move cursor to end of line ");
	print(3, 0, " w - Move the cursor to the end of the word to the right of the cursor");
	print(4, 0, " b - Move the cursor to the beginning of a word to the left of the cursor");
	print(5, 0, " gg - Move to the beginning of the file");
	print(6, 0, " G - Move to the end of the file");
	print(7, 0, " NG - Move to line with number N");
	print(8, 0, " x - Delete character after cursor");
	print(9, 0, " diw - Delete the word under the cursor, including the 	space to the right");
	print(10, 0, " dd - Cut the current line");
	print(11, 0, " y - Copy current line");
	print(12, 0, " yw - Copy word under cursor");
	print(13, 0, " p - Paster after cursor");
	print(15, 0, " Press any button to continue ... ");
	logo_preline();
}
