#pragma once
#include "model.h"
#include "curses.h"
#include "panel.h"

Model::Model()
{
	cur_line = cur_pos = 0;
	cur_mode = 'N';
	cur_file = "no file";
	start_pos = 2;
	max_lines = 0;
	count_syms_per_line.resize(1000);
	is_empty.resize(1000);
	display_start_line = 0;
	print_screen();
	
}

void Model::update_lines()
{
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}

	for (int i = 0; i <= max_lines; i++)
		count_syms_per_line[i] = 0;
	int i = 0;
	int line = 0;
	while (i<left_buffer.size())
	{
		if (left_buffer.c_str()[i] == '\n' && count_syms_per_line[line]!=0)
		{
			count_syms_per_line[line]++;
			line++;
		}
		else if (left_buffer.c_str()[i] == '\n' && count_syms_per_line[line] == 0)
		{
			count_syms_per_line[line]=1;
			line++;
		}
		else
		{
			count_syms_per_line[line]++;
		}
		i++;
	}
	max_lines = line ;
}
//////////   ARROWS   ///////////
void Model::left()
{

	if (cur_pos)
		cur_pos--;
	else
	{
		if (!cur_line)
			return;
		else
		{
			cur_line--;
			cur_pos = count_syms_per_line[cur_line] - 1;
			check_display_overflow();
		}
	}
}

void Model::right()
{
	//dodge '\n', that's why -1
	if (cur_pos < count_syms_per_line[cur_line] - 1 && count_syms_per_line[cur_line] != 0)
	{
		cur_pos++;
	}
	else if (count_syms_per_line[cur_line] == 0)
	{
		return;
	}
	else
	{
		if (cur_line == max_lines)
		{
			if (cur_pos == count_syms_per_line[cur_line])
				return;
			else
			{
				cur_pos++;
				return;
			}
		}
		else
		{
			cur_line++;
			cur_pos = 0;
			check_display_overflow();
		}
	}
}

void Model::up()
{
	/*         3 cases:
	1 - cur_line is a first_line
	2 - cur_pos >= len[high_line]
	3 - cur_pos < len[high_line]*/

	if (!cur_line)
		return;
	if (cur_pos >= count_syms_per_line[cur_line - 1])
	{
		cur_line--;
		cur_pos = count_syms_per_line[cur_line] - 1;
	}
	else
	{
		cur_line--;
	}
	check_display_overflow();
}

void Model::down()
{
	//			4 cases:
	//1 - cur_line is a max_line
	//2 - cur_pos > len[low_line]
	//	2.1 - len[low_line] is empty ('\n')
	//3 - cur_pos <= len[low_line]
	//4 - cur_line is a pre-max_line

	if (cur_line == max_lines)
		return;
	if (cur_pos >= count_syms_per_line[cur_line + 1])
	{
		cur_line++;
		if (cur_line == max_lines)
			cur_pos = count_syms_per_line[cur_line];
		else
		{
			if (!is_empty[cur_line])
				cur_pos = count_syms_per_line[cur_line] - 1;
			else
				cur_pos = 0;
		}
	}
	else
	{
		cur_line++;
	}

	check_display_overflow();
}

//////////////////////////////

void Model::insert(int symbol)
{
	shift_boundary_of_buffers();
	if (symbol != 8 && symbol != 262 && symbol != 358)
	{
		left_buffer.append(1, symbol);
		cur_pos++;
		count_syms_per_line[cur_line]++;
		is_empty[cur_line] = 0;
	}
	else if (symbol == 8)
		backspace();
	else if (symbol == 262)
		to_begin_of_line();
	else if (symbol == 358)
		to_end_of_line();
}

void Model::backspace()
{

	if (cur_pos == 0 && cur_line == 0)
		return;


	shift_boundary_of_buffers();
	left_buffer.erase(left_buffer.size() - 1, 1);
	if (cur_pos)
	{
		cur_pos--;
		count_syms_per_line[cur_line]--;

	}

	else
	{
		cur_line--;
		cur_pos = count_syms_per_line[cur_line] - 1;
		max_lines--;
		
	}
	update_lines();
	check_display_overflow();
}

void Model::enter()
{
	shift_boundary_of_buffers();
	left_buffer.append(1, '\n');
	int tmp_pos = cur_pos;
	cur_pos = 0;
	count_syms_per_line[cur_line]++;
	cur_line++;
	if (count_syms_per_line[cur_line] == 1)
		is_empty[cur_line] = true;
	max_lines++;
	if (right_buffer.size())
	{
		count_syms_per_line[cur_line] = count_syms_per_line[cur_line - 1] - tmp_pos - 1;
		count_syms_per_line[cur_line - 1] = tmp_pos + 1;	
	}
	check_display_overflow();
}

///////////  BUFFER OPERATIONS  ///////////

unsigned int Model::get_boundary()
{
	update_lines();
	int iter = 0;
	for (int i = 0; i < cur_line; ++i)
		iter += count_syms_per_line[i];
	iter += cur_pos;
	return iter;
}

void Model::shift_from_left_to_right()
{
	left_buffer.append(1, right_buffer[0]);
	right_buffer.erase(0, 1);
}

void Model::shift_from_right_to_left()
{
	right_buffer.insert(0, 1, left_buffer[left_buffer.size() - 1]);
	left_buffer.erase(left_buffer.size() - 1, 1);
}



void Model::shift_boundary_of_buffers()
{

	//Find boundary between two buffers
	unsigned int iter = get_boundary();

	while (left_buffer.size() < iter)
	{
		shift_from_left_to_right();
	}
	while (left_buffer.size() > iter)
	{
		shift_from_right_to_left();
	}
}



////////////  PRINT  //////////////////

void Model::print_screen()
{
	clear();
	view.window_parameters();
	view.panel(cur_mode, cur_line, cur_pos, max_lines, count_syms_per_line[cur_line],
		cmd, display_start_line, tmp, right_buffer, cur_file);
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}
	print_buffer();
	view.move_cursor(cur_line - display_start_line, cur_pos + start_pos);
	view.refresh_screen();
}
int Model::skip_invisible_part()
{
	int i = 0;
	int line = 0;
	while (left_buffer[i] != '\0' && display_start_line != line)
	{
		if (left_buffer[i] == '\n')
			line++;
		i++;
	}
	return i;
}
void Model::print_buffer()
{
	int iter = skip_invisible_part();
	int y = 0;
	view.insert_preambula();
	while (left_buffer[iter] != '\0' && y < view.max_y - 2)
	{
		if (left_buffer[iter] == '\n')
		{
			view.print_symbol('\n');
			view.insert_preambula();
			y++;
		}
		else
			view.print_symbol(left_buffer[iter]);
		iter++;
	}
}


void Model::clear_two_buffers()
{
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}
	left_buffer.erase(0, left_buffer.size());

	cur_pos = 0;
	cur_line = 0;
	max_lines = 0;
	update_lines();
	check_display_overflow();
}

void Model::check_display_overflow()
{
	if (cur_line >= display_start_line + view.max_y - 3)
		display_start_line = cur_line - view.max_y + 3;
	else
		if (cur_line < display_start_line)
			display_start_line = cur_line;
}
//////////////////////////////////////////

void Model::page_up()
{
	int tmp_line = cur_line;
	tmp_line -= view.max_y - 2;
	if (tmp_line <= 0)
		cur_line = 0;
	else
		cur_line = tmp_line;
	check_display_overflow();
	cur_pos = 0;
}

void Model::page_down()
{
	cur_line += view.max_y - 2;
	if (cur_line >= max_lines)
		cur_line = max_lines;
	check_display_overflow();
	cur_pos = 0;
}

///////////////// COMMANDS /////////////

void Model::delete_after_cursor()
{
	int tmp_pos = cur_pos;
	int tmp_line = cur_line;
	right();
	if (!(tmp_pos == cur_pos && tmp_line == cur_line))
		backspace();
}

void Model::to_begin_of_line()
{
	cur_pos = 0;
}

void Model::to_end_of_line()
{
	int flag_eof = 1;
	if (cur_line == max_lines)
		flag_eof = 0;
	if (count_syms_per_line[cur_line] > 0)
		cur_pos = count_syms_per_line[cur_line] - flag_eof;

}

void Model::to_end_of_next_word()
{
	shift_boundary_of_buffers();
	int index = get_boundary()+1;
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}

	int index1 = left_buffer.find(" ",index);
	int index2 = left_buffer.find("\n", index);
	int close_index;
	if (index1 >0)
	{
		if (index2 >0)
		{
			if (index2 > index1)
				close_index = index1;
			else
				close_index = index2;
		}
		else
			close_index = index1;

	}
	else if (index1 <=0 && index2 >0)
	{
		close_index = index2;
	}
	else if (index1 <= 0 && index2 <= 0)
	{
		cur_line = max_lines;
		cur_pos = count_syms_per_line[cur_line];
		check_display_overflow();
		return;
	}
	cur_pos = 0;
	cur_line = 0;
	for (int i = 0; i < close_index; i++)
	{
		if (left_buffer[i] == '\n')
		{
			cur_line++; cur_pos = 0;
		}
		else
			cur_pos++;
	}
	check_display_overflow();
}



void Model::to_begin_of_last_word()
{
	shift_boundary_of_buffers();
	int index = get_boundary() ;
	MyString reverse;
	for (int i = left_buffer.size() - 1; i >= 0; i--)
	{
		reverse.append(1, left_buffer.c_str()[i]);
	}
	int index1 = reverse.find(" ", 0);
	if (index1==index)
		int index1 = reverse.find(" ", 1);
	int index2 = reverse.find("\n", 0);
	if (index2 == index)
		int index2 = reverse.find("\n", 1);
	int close_index;
	if (index1 > 0)
	{
		if (index2 > 0)
		{
			if (index2 > index1)
				close_index = index1;
			else
				close_index = index2;
		}
		else
			close_index = index1;

	}
	else if (index1 <= 0 && index2 > 0)
	{
		close_index = index2;
	}
	else if (index1 <= 0 && index2 <= 0)
	{
		cur_line = 0;
		cur_pos = 0;
		return;
	}
	close_index = left_buffer.size() - close_index -1;
	cur_pos = 0;
	cur_line = 0;
	for (int i = 0; i < close_index; i++)
	{
		if (left_buffer[i] == '\n')
		{
			cur_line++; cur_pos = 0;
		}
		else
			cur_pos++;
	}
	check_display_overflow();

}

void Model::cursor_total_end()
{
	cur_line = max_lines;
	cur_pos = count_syms_per_line[cur_line];
	check_display_overflow();
}


void Model::cursor_total_begin()
{
	cur_pos = 0;
	cur_line = 0;
	check_display_overflow();
}


void Model::copy()
{
	tmp.clear(); 
	int tmp_pos = cur_pos;
	to_begin_of_line();
	int index = get_boundary();
	for (int i = 0; i < count_syms_per_line[cur_line]; i++)
		tmp.append(1, left_buffer[index + i]);
	cur_pos = tmp_pos;

}

void Model::delete_line()
{
	to_end_of_line();
	while (count_syms_per_line[cur_line] > 0 && cur_pos != 0)
		backspace();
}

void Model::cut()
{
	copy();
	delete_line();
	backspace();
}

void Model::paste()
{
	if (!tmp.size())
		return;

	for (int i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] != '\n')
			insert(tmp[i]);
		else
			enter();
	}
}



void Model::delete_and_insert_symbol(int symbol)
{
	delete_after_cursor();
	insert(symbol);
	left();
}

void Model::copy_word_under_cursor()
{
	tmp.clear();
	int tmp_pos = cur_pos;
	shift_boundary_of_buffers();
	int index = get_boundary()-1;
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}
	while (left_buffer[index] != ' ' && left_buffer[index] != '\n' && cur_pos >= 1)
	{
		index--; 
		left();
	}
	index++;
	while (left_buffer[index] != ' ' && left_buffer[index] != '\n' && cur_pos < count_syms_per_line[cur_line])
	{
		tmp.append(1, left_buffer[index]);
		index++;
		right();
	}
	cur_pos = tmp_pos;
}


void Model::delete_word_with_space()
{
	shift_boundary_of_buffers();
	int index = get_boundary() - 1;
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}
	while (left_buffer[index] != ' ' && left_buffer[index] != '\n' && cur_pos >= 1)
	{
		index--;
		left();
	}
	index++;
	while (left_buffer[index] != ' ' && left_buffer[index] != '\n' && index != left_buffer.size())
	{
		left_buffer.erase(index,1);
	}
	backspace();
	update_lines();

}

void Model::go_to_N_line(int N)
{
	if (N > max_lines) return;
	cur_line = N;
	cur_pos = 0;
	check_display_overflow();
}


////////////// SEARCH ///////////////////////

void Model::search()
{

	int index = get_boundary();
	while (right_buffer.size())
	{
		shift_from_left_to_right();
	}
	int iter = -1;
	iter = left_buffer.find(substring.c_str(), index);
	if (iter != -1)
	{
		for (int i = index; i < iter; i++)
		{
			if (left_buffer[i] == '\n')
			{
				cur_line++; cur_pos = 0;
			}
			else
				cur_pos++;
		}
	}
	check_display_overflow();

}

void Model::search_reverse()
{
	shift_boundary_of_buffers();
	int index = get_boundary()-1;
	MyString reverse, substring_reverse;
	for (int i = index; i >= 0; i--)
	{
		reverse.append(1, left_buffer.c_str()[i]);
	}
	for (int i = substring.size() - 1; i >= 0; i--)
	{
		substring_reverse.append(1, substring.c_str()[i]);
	}
	int iter = -1;
	iter = reverse.find(substring_reverse.c_str(), 0);
	if (iter != -1)
	{
		iter = index - iter - substring_reverse.size() + 1;
		cur_pos = 0;
		cur_line = 0;
		for (int i = 0; i < iter; i++)
		{
			if (left_buffer[i] == '\n')
			{
				cur_line++; cur_pos = 0;
			}
			else
				cur_pos++;
		}
	}
	check_display_overflow();
}


