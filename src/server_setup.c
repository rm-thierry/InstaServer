#include "server_setup.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static const char* loading_chars = "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏";

static void show_loading(const char* package) {
    static int i = 0;
    printf("\r\033[K%c Installing %s...", loading_chars[i], package);
    fflush(stdout);
    i = (i + 1) % strlen(loading_chars);
}

static void execute_quiet(const char* cmd) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s >/dev/null 2>&1", cmd);
    system(buffer);
}

void server_setup_perform(void) {
    const char* packages[] = {
        "vim",
        "htop",
        "net-tools",
        "curl",
        "wget",
        "git",
        "unzip",
        "zip"
    };
    int num_packages = sizeof(packages) / sizeof(packages[0]);

    printf("Updating system...\n");
    execute_quiet("apt-get update");

    for (int i = 0; i < num_packages; i++) {
        char cmd[256];
        int j = 0;
        snprintf(cmd, sizeof(cmd), "apt-get install -y %s", packages[i]);
        
        while (1) {
            show_loading(packages[i]);
            usleep(100000);
            
            char check_cmd[256];
            snprintf(check_cmd, sizeof(check_cmd), 
                    "dpkg -l %s 2>/dev/null | grep -q '^ii'", packages[i]);
            if (system(check_cmd) == 0) break;

            if (j == 0) {
                execute_quiet(cmd);
            }
            j++;
        }
        printf("\r\033[K✓ Installed %s\n", packages[i]);
    }

    printf("\nAll packages installed successfully!\n");
    custom_configs();

}


void custom_configs(void) {
    printf("Configuring VIM...\n");

    const char* home = getenv("HOME");
    if (!home) {
        printf("✗ Could not get HOME directory\n");
        return;
    }


    //VIM configuration
    //This Config is hard coded. I want to make it dynamic in future.
    const char* vimrc_content =
        "syntax on\n"
        "set number\n"
        "set autoindent\n"
        "set tabstop=4\n"
        "set shiftwidth=4\n"
        "set expandtab\n"
        "set smartindent\n"
        "set hlsearch\n"
        "set incsearch\n"
        "set ignorecase\n"
        "set smartcase\n"
        "set backspace=indent,eol,start\n"
        "set history=1000\n"
        "set undolevels=1000\n"
        "set undoreload=10000\n"
        "set laststatus=2\n"
        "set statusline=%F%m%r%h%w[%L][%{&ff}]\n"
        "set mouse=a\n"
        "set clipboard=unnamedplus\n"
        "set background=dark\n"
        "colorscheme desert\n"
        "set t_Co=256\n"
        "set encoding=utf-8\n"
        "set fileencoding=utf-8\n"
        "set termencoding=utf-8\n"
        "set listchars=tab:▸\\ ,eol:¬,trail:•,extends:>,precedes:<,nbsp:.\n"
        "set list\n";

    char vimrc_path[1024];
    size_t result = (size_t)snprintf(vimrc_path, sizeof(vimrc_path), "%s/.vimrc", home);
    if (result >= sizeof(vimrc_path)) {
        printf("✗ Path too long\n");
        return;
    }

    FILE* fp = fopen(vimrc_path, "w");
    if (!fp) {
        printf("✗ Could not create .vimrc\n");
        return;
    }

    fprintf(fp, "%s", vimrc_content);
    fclose(fp);

    if (access(vimrc_path, F_OK) == 0) {
        printf("✓ VIM configured successfully!\n");
    } else {
        printf("✗ Failed to configure VIM\n");
    }
}



void server_setup_wait_for_enter(void) {
    printf("\nPress Enter to continue...");
    getchar();
}