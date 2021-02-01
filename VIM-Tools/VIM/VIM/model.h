#pragma once
#define VIM_MODEL_H
#include "my_string.h"
//#include "MyString.h"
#include "view.h"
#include <vector>
using namespace std;
class Model
{
public:
	View view;
	unsigned int cur_line;
	unsigned int cur_pos;
	char cur_mode;
	unsigned int start_pos;
	unsigned int max_lines;
	MyString cur_file = "";
	MyString left_buffer = "";
	MyString right_buffer = "";
	MyString cmd = "";
	MyString substring = "";
	MyString substring_buffer = "";
	MyString tmp = "";
	
	vector<bool> is_empty;
	vector<unsigned int> count_syms_per_line;

	Model();
	void update_lines();
	void insert(int symbol);
	void backspace();
	void left();
	void right();
	void enter();
	void up();
	void down();
	void page_up();
	void page_down();
	void delete_line();
	unsigned int get_boundary();
	void shift_boundary_of_buffers();
	void shift_from_left_to_right();
	void shift_from_right_to_left();
	void clear_two_buffers();
	void to_end_of_line();
	void to_begin_of_line();
	void to_begin_of_last_word();
	void to_end_of_next_word();
	void cursor_total_end();
	void delete_after_cursor();
	void cursor_total_begin();
	void delete_and_insert_symbol(int symbol);
	void go_to_N_line(int N);
	void cut();
	void copy();
	void paste();
	void delete_word_with_space();
	void copy_word_under_cursor();
	int skip_invisible_part();
	void print_screen();
	void print_buffer();
	unsigned int display_start_line;
	void check_display_overflow();
	void search_reverse();
	void search();
	
};