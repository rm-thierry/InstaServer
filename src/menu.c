#include "menu.h"
#include <stdio.h>

static const char* choices[] = {
    "Server Setup",
    "Exit"
};

void menu_print(int highlight) {
    printf("\n\n Linux Server Setup Tool\n\n");
    for (int i = 0; i < N_CHOICES; i++) {
        if (highlight == i + 1) {
            printf(" \033[7m%s\033[0m\n", choices[i]);
        } else {
            printf(" %s\n", choices[i]);
        }
    }
}

int menu_get_number_of_choices(void) {
    return N_CHOICES;
}