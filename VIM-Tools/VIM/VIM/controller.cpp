#pragma once
#include "controller.h"
Controller::Controller()
{
	mode = new ModeManager(this);
}


void Controller::arrows(int symbol)
{
	switch (symbol)
	{
	case KEY_LEFT:
		model.left();
		break;
	case KEY_RIGHT:
		model.right();
		break;
	case KEY_DOWN:
		model.down();
		break;
	case KEY_UP:
		model.up();
		break;
	default:
		break;
	}
}


void Controller::pages(int symbol)
{
	switch (symbol)
	{
	case KEY_PPAGE:
		model.page_up();
		break;
	case KEY_NPAGE:
		model.page_down();
		break;
	default:
		break;
	}
}


void ModeManager::NAV_mode()
{
	int symbol;
	while (true)
	{
		symbol = getch();
		switch (symbol)
		{
		case 'i':
			INS_mode(); break;
		case '0': case '^': case KEY_HOME:
			controller->model.to_begin_of_line(); break;
		case 'I':
			controller->model.to_begin_of_line();
			INS_mode(); break;
		case '$': case KEY_END:
			controller->model.to_end_of_line(); break;
		case 'A':
			controller->model.to_end_of_line();
			INS_mode(); break;
		case 'S':
			controller->model.delete_line();
			INS_mode(); break;
		case 'w':
			controller->model.to_end_of_next_word(); break;
		case 'G':
			controller->model.cursor_total_end(); break;
		case 'b':
			controller->model.to_begin_of_last_word(); break;
		
		case 'x':
			controller->model.delete_after_cursor(); break;
		case 'p':
			controller->model.paste(); controller->file_modified = true; break;
		case 'r':
			symbol = getch();
			controller->model.delete_and_insert_symbol(symbol);
			controller->file_modified = true; break;
		case ':':
			CMD_mode(); break;
		case '/':
			controller->model.substring.append(1, '/');
			controller->model.cmd.append(1, '/');
			controller->model.substring_buffer.append(1, '/');
			SEA_mode(); break;
		case '?':
			controller->model.substring.append(1, '?');
			controller->model.cmd.append(1, '?');
			controller->model.substring_buffer.append(1, '?');
			SEA_mode(); break;
		case 'n':
			SEA_parse(1);
			break;
		case 'N':
			SEA_parse(2);
			break;
		case ERR:
			break;
		case KEY_UP: case KEY_DOWN: case KEY_RIGHT: case KEY_LEFT:
			controller->arrows(symbol); break;
		case KEY_PPAGE: case KEY_NPAGE:
			controller->pages(symbol); break;
		case 'd': case 'y': case 'g': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
			controller->model.cmd.append(1, symbol);
			NAV_parse();
		default:
			break;
		}
		controller->model.cmd.clear();
		controller->model.print_screen();
		if (controller->exit)
			return;
	}
}


void ModeManager::NAV_parse()
{
	int symbol;
	//dd, yw, gg, diw, 100G, y
	MyString dd = "dd";
	MyString diw = "diw";
	MyString gg = "gg";
	MyString yw = "yw";
	MyString y = "y\n";
	if (controller->model.cmd[0] == '/' || controller->model.cmd[0] == '?')
		return;
	while (controller->model.cmd.size() <= 4)
	{
		if ((controller->model.cmd[0] >= '1' && controller->model.cmd[0] <= '9')
			|| controller->model.cmd[0] == 'd' || controller->model.cmd[0] == 'y' || controller->model.cmd[0] == 'g')
		{
			if (controller->model.cmd == dd)
			{
				controller->model.cut();
				break;
			}
			if (controller->model.cmd == diw)
			{
				controller->model.delete_word_with_space();
				break;
			}
			if (controller->model.cmd == gg)
			{
				controller->model.cursor_total_begin();
				break;
			}
			if (controller->model.cmd == yw)
			{
				controller->model.copy_word_under_cursor();
				break;
			}
			if (controller->model.cmd == y)
			{
				controller->model.copy();
				break;
			}
			if (controller->model.cmd[0] >= '1' && controller->model.cmd[0] <= '9')
			{
				if (controller->model.cmd[controller->model.cmd.size() - 1] == 'G')
				{
					int num = 0;
					for (int i = 0; i < controller->model.cmd.size() - 1; ++i)
					{

						num += int(controller->model.cmd[i] - 48);
						num *= 10;
					}
					num /= 10;
					controller->model.go_to_N_line(num);

					break;
				}
				else if (controller->model.cmd[controller->model.cmd.size() - 1] != 'G'
					&& (controller->model.cmd[controller->model.cmd.size() - 1] < '0'
						|| controller->model.cmd[controller->model.cmd.size() - 1] > '9')) break;
			}
			if (controller->model.cmd[controller->model.cmd.size() - 1] == 27)
				break;
			controller->model.print_screen();
			symbol = getch();
			controller->model.cmd.append(1, symbol);
		}
		else
		{
			break;
		}
	}
	controller->model.cmd.clear();
	return;
}

void ModeManager::SEA_mode()
{
	controller->model.print_screen();
	controller->model.cur_mode = 'S';

	int symbol;
	int running = 1;
	while (running)
	{
		symbol = getch();
		switch (symbol)
		{
		case KEY_BACKSPACE:
			controller->model.substring.erase(controller->model.substring.size() - 1, 1);
			controller->model.cmd.erase(controller->model.substring.size() - 1, 1);
			break;
		case 10:
			SEA_parse(0);
			controller->model.cmd.clear();
			controller->model.substring.clear();
			controller->model.cur_mode = 'N';
			running = 0;
			break;
		case 27:
			controller->model.substring.clear();
			controller->model.cmd.clear();
			controller->model.cur_mode = 'N';
			running = 0;
		default:
			controller->model.substring.append(1, symbol);
			controller->model.cmd.append(1, symbol);
			break;
		}
		controller->model.print_screen();
	}

}
void ModeManager::SEA_parse(int set)
{
	if (set == 0)
	{
		int flag = 0;
		if (controller->model.substring.c_str()[0] == '/')
			flag = 0;
		else
			flag = 1;
		controller->model.substring_buffer = controller->model.substring;
		controller->model.substring.erase(0, 1);
		if (!flag) controller->model.search();
		else controller->model.search_reverse();
	}
	if (set == 1)
	{
		controller->model.substring = controller->model.substring_buffer;
		int flag = 0;
		if (controller->model.substring.c_str()[0] == '/')
			flag = 0;
		else
			flag = 1;
		controller->model.substring.erase(0, 1);
		if (!flag) controller->model.search();
		else controller->model.search_reverse();
	}
	if (set == 2)
	{
		controller->model.substring = controller->model.substring_buffer;
		int flag = 0;
		if (controller->model.substring.c_str()[0] == '/')
			flag = 1;
		else
			flag = 0;
		controller->model.substring.erase(0, 1);
		if (!flag) controller->model.search();
		else controller->model.search_reverse();
	}

}


void ModeManager::CMD_mode()
{
	controller->model.cur_mode = 'C';
	int running = 1;
	while (running)
	{
		controller->model.print_screen();
		int symbol = getch();
		switch (symbol)
		{
		case KEY_BACKSPACE:
			if (controller->model.cmd.size())
				controller->model.cmd.erase(controller->model.cmd.size() - 1, 1);
			else
				running = 0;
			break;
		case 10:
			CMD_parse();
			running = 0;
			break;
		case 27:
			running = 0;
			break;
		default:
			controller->model.cmd.append(1, symbol);
			break;
		}
	}
	controller->model.cmd.clear();
	controller->model.cur_mode = 'N';
	controller->model.view.move_cursor(controller->model.cur_line, controller->model.cur_pos + controller->model.start_pos);
}

void ModeManager::CMD_parse()
{
	if (controller->model.cmd[0] >= '0' && controller->model.cmd[0] <= '9')
	{
		int num = 0;
		for (int i = 0; i < controller->model.cmd.size(); ++i)
		{

			num += int(controller->model.cmd[i] - 48);
			num *= 10;
		}
		num /= 10;
		controller->model.go_to_N_line(num);
	}

	MyString q = "q";
	if (controller->model.cmd == (q + "!")
		|| controller->model.cmd == q && !controller->file_modified)
	{
		controller->exit = true;
	}

	if (controller->model.cmd[0] == 'o')
	{
		controller->model.cmd.erase(0, 2);
		controller->model.cur_file = controller->model.cmd;
		controller->open_file(controller->model.cmd.c_str());
	}

	MyString wq = "wq!";
	MyString x = "x";
	if (controller->model.cmd == x 	|| controller->model.cmd == wq)
	{
		controller->save_to_cur_file();
		controller->exit = true;
	}

	MyString w = "w";
	if (controller->model.cmd >= w)
	{
		if (controller->model.cmd == w)
		{
			controller->save_to_cur_file();	
		}
		else
		{
			controller->model.cmd.erase(0, 2);
			controller->save_to_new_file(controller->model.cmd.c_str());
		}
			
	}

	MyString h = "h";
	if (controller->model.cmd == h)
	{
		controller->model.view.help_info();
		getch();
	}
}

void ModeManager::INS_mode()
{
	controller->model.cur_mode = 'I';
	controller->model.print_screen();
	int symbol;
	while (true)
	{
		symbol = getch();
		switch (symbol)
		{
		case KEY_LEFT:
			controller->model.left(); break;
		case KEY_RIGHT:
			controller->model.right(); break;
		case KEY_DOWN:
			controller->model.down(); break;
		case KEY_UP:
			controller->model.up(); break;
		case KEY_PPAGE:
			controller->model.page_up(); break;
		case KEY_NPAGE:
			controller->model.page_down(); break;
		case KEY_DC:
			controller->model.backspace(); break;
		case 10:
			controller->model.enter(); controller->file_modified = true; break;
		case ERR:
			break;
		case 27: 
			controller->model.cur_mode = 'N';
			controller->model.print_screen();
			return;
		default:
			controller->model.insert(symbol); controller->file_modified = true; break;
		}
		controller->model.print_screen();
	}
}


void Controller::open_file(const char* file_name)
{
	model.clear_two_buffers();
	ifstream f;
	f.open(file_name);
	if (!f.is_open())
		return;
	char symbol;
	model.cur_file = file_name;
	while (!f.eof())
	{
		f.get(symbol);

		if (symbol == '\n')
			model.enter();
		else
			model.insert(symbol);
	}
	model.cursor_total_begin();
	model.cmd.clear();
	
	model.print_screen();
	
}
void Controller::save_to_cur_file()
{
	ofstream f;
	f.open(cur_file.c_str(), ios_base::out | ios_base::trunc);
	f << model.left_buffer.c_str();
	f << model.right_buffer.c_str();
}

void Controller::save_to_new_file(const char* file_name)
{
	ofstream f;
	f.open(file_name, ios_base::out | ios_base::trunc);
	if (!f.is_open())
		return;
	f << model.left_buffer.c_str();
	f << model.right_buffer.c_str();
}

