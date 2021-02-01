#pragma once
#include "adapter_pdcurses.h"
#include "my_string.h"
//#include "MyString.h"


AdapterPDcurses::AdapterPDcurses() 
{
    initscr();
    noecho();
    keypad(stdscr, true);
    use_default_colors();
    screen_color();
}

void AdapterPDcurses::endwindow()
{
    endwin();
}

void AdapterPDcurses::move_cursor(int y, int x) const
{
    move(y, x);
}

void AdapterPDcurses::print_symbol(char symbol) const 
{
    addch(symbol);
}

void AdapterPDcurses::insert_preambula() const
{
    addstr("~ ");
}

void AdapterPDcurses::window_parameters()
{
    getmaxyx(stdscr, max_y, max_x);
}

void AdapterPDcurses::print(int y, int x, const char* str) const 
{
    mvprintw(y, x, str);
}

void AdapterPDcurses::print(int y, int x, const char* str, MyString cmd) const
{
    mvprintw(y, x, str, cmd.c_str());
}

void AdapterPDcurses::print_string(const char* string) const 
{
    addstr(string);
}


void AdapterPDcurses::print(int y, int x, const char* str, unsigned int cur_line,
   unsigned int cur_pos, unsigned int max_lines, unsigned int max_line_pos, 
   unsigned int display_start_line,  MyString cmd, MyString cur_file) const 
{
    mvprintw(y, x, str, cur_line,cur_pos, max_lines, max_line_pos, display_start_line,
      cmd.c_str(), cur_file.c_str());
}

void AdapterPDcurses::change_color(int color) const
{
    attron(COLOR_PAIR(color));
}

void AdapterPDcurses::clear_screen() const
{
    clear();
}

void AdapterPDcurses::screen_color() const 
{
    start_color();
    init_pair(1, COLOR_YELLOW, -1);
    init_pair(2, COLOR_CYAN, -1);
    attron(COLOR_PAIR(1));
}

void AdapterPDcurses::refresh_screen() const
{
    refresh();
}