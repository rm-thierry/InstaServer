#include "terminal.h"
#include "menu.h"
#include "server_setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    struct Terminal term;
    char os[256] = "Unknown OS";
    int highlight = 1;
    int choice = 0;
    int key;

    // OS-Information abrufen
    system("lsb_release -d | cut -f2- > os.txt");

    FILE *file = fopen("os.txt", "r");
    if (file) {
        if (fgets(os, sizeof(os), file)) {
            os[strcspn(os, "\n")] = '\0';
        }
        fclose(file);
        system("rm os.txt");
    } else {
        fprintf(stderr, "Failed to retrieve OS information.\n");
    }

    printf("Welcome to the Linux Server Setup Tool\n");
    printf("  _____ _   _  _____ _______          _____ ______ _______      ________ _____  \n");
    printf(" |_   _| \\ | |/ ____|__   __|/\\      / ____|  ____|  __ \\ \\    / /  ____|  __ \\ \n");
    printf("   | | |  \\| | (___    | |  /  \\    | (___ | |__  | |__) \\ \\  / /| |__  | |__) |\n");
    printf("   | | | .  |\\___ \\   | | / /\\ \\    \\___ \\|  __| |  _  / \\ \\/ / |  __| |  _  / \n");
    printf("  _| |_| |\\  |____) |  | |/ ____ \\   ____) | |____| | \\ \\  \\  /  | |____| | \\ \\ \n");
    printf(" |_____|_| \\_|_____/   |_/_/    \\_\\ |_____/|______|_|  \\_\\  \\/   |______|_|  \\_\\\n");
    printf("\n");
    sleep(2);
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
