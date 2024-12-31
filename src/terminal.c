#include "terminal.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void terminal_setup(struct Terminal* term) {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &term->orig_termios);
    new_termios = term->orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void terminal_restore(struct Terminal* term) {
    tcsetattr(STDIN_FILENO, TCSANOW, &term->orig_termios);
}

void terminal_clear_screen(void) {
    printf("\033[2J\033[H");
}

void terminal_hide_cursor(void) {
    printf("\033[?25l");
}

void terminal_show_cursor(void) {
    printf("\033[?25h");
}

int terminal_get_key(void) {
    char buf[3];
    read(STDIN_FILENO, buf, 1);
    int c = buf[0];
    
    if (c == 27) {
        if (read(STDIN_FILENO, buf, 2) == 2) {
            if (buf[0] == 91) {
                return buf[1];
            }
        }
    }
    return c;
}