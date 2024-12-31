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

    const char* vimrc_file_path = "../utils/.vimrc";

    FILE *vimrc_file = fopen(vimrc_file_path, "r");
    if (!vimrc_file) {
        printf("✗ Could not open the vimrc file: %s\n", vimrc_file_path);
        return;
    }

    char vimrc_path[1024];
    size_t result = (size_t)snprintf(vimrc_path, sizeof(vimrc_path), "%s/.vimrc", home);
    if (result >= sizeof(vimrc_path)) {
        printf("✗ Path too long\n");
        fclose(vimrc_file);
        return;
    }

    FILE* fp = fopen(vimrc_path, "w");
    if (!fp) {
        printf("✗ Could not create .vimrc\n");
        fclose(vimrc_file);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), vimrc_file)) {
        fputs(buffer, fp);
    }

    fclose(vimrc_file);
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