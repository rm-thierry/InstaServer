#include "terminal.h"
#include "menu.h"
#include "server_setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    struct Terminal term;
    int highlight = 1;
    int choice = 0;
    int key;
    
    terminal_setup(&term);
    
    while (1) {
        terminal_clear_screen();
        terminal_hide_cursor();
        menu_print(highlight);
        
        key = terminal_get_key();
        
        switch (key) {
            case 65:
                highlight = (highlight == 1) ? N_CHOICES : highlight - 1;
                break;
            case 66:
                highlight = (highlight == N_CHOICES) ? 1 : highlight + 1;
                break;
            case 10:
                choice = highlight;
                break;
        }
        
        if (choice != 0) {
            terminal_show_cursor();
            
            switch (choice) {
                case 1:
                    terminal_clear_screen();
                    printf("Performing server setup...\n");
                    server_setup_perform();
                    server_setup_wait_for_enter();
                    break;
                case EXIT_CHOICE:
                    terminal_clear_screen();
                    terminal_restore(&term);
                    return 0;
            }
            
            choice = 0;
        }
    }
    
    return 0;
}