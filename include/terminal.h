#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
#include <unistd.h>

struct Terminal {
    struct termios orig_termios;
};

void terminal_setup(struct Terminal* term);
void terminal_restore(struct Terminal* term);
void terminal_clear_screen(void);
void terminal_hide_cursor(void);
void terminal_show_cursor(void);
int terminal_get_key(void);

#endif