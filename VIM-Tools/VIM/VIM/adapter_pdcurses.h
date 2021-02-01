#pragma once
#include <iostream>
#include "curses.h"
#include "panel.h"
#include "my_string.h"
//#include "MyString.h"
class AdapterPDcurses {
public:
    AdapterPDcurses();
    void endwindow();
    void print(int y, int x, const char* str, unsigned int cur_line, unsigned int cur_pos, unsigned int max_lines, unsigned int max_line_pos, unsigned int display_start_line,  MyString cmd, MyString cur_file) const;
    void print(int y, int x, const char* str) const;
    void print(int y, int x, const char* str, MyString cmd) const;
    void print_string(const char* string) const;
    void insert_preambula() const;
    void screen_color() const;
    void window_parameters() ;
    void print_symbol(char symbol) const;
    void move_cursor(int y, int x) const;
    void change_color(int color) const;
    void clear_screen() const;
    void refresh_screen() const;
    int max_y, max_x;

};
