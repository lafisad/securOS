#include "shell.h"
#include "../ui/wm.h"
#include "../drivers/keyboard.h"
#include "../utils/string.h"
#include "../utils/logger.h"
#include "../commands/fetch.h"

static char input_buffer[SHELL_INPUT_BUFFER_SIZE];
static size_t input_pos = 0;
static uint8_t shell_window_id = 0;

void shell_init() {
    memset(input_buffer, 0, SHELL_INPUT_BUFFER_SIZE);
    input_pos = 0;
}

void shell_prompt() {
    wm_write_to_window(shell_window_id, "securOS> ");
}

void shell_process_command() {
    input_buffer[input_pos] = '\0';
    
    if (strcmp(input_buffer, "help") == 0) {
        wm_write_to_window(shell_window_id, "\nAvailable commands:\n");
        wm_write_to_window(shell_window_id, "  help  - Show this help\n");
        wm_write_to_window(shell_window_id, "  clear - Clear screen\n");
        wm_write_to_window(shell_window_id, "  echo  - Echo text\n");
        log_info("User requested help");
    }
    else if (strcmp(input_buffer, "clear") == 0) {
        wm_clear_window(shell_window_id);
        log_info("Shell cleared");
    }
    else if (strlen(input_buffer) >= 5 && input_buffer[0] == 'e' && 
             input_buffer[1] == 'c' && input_buffer[2] == 'h' && 
             input_buffer[3] == 'o' && input_buffer[4] == ' ') {
        wm_write_to_window(shell_window_id, "\n");
        wm_write_to_window(shell_window_id, &input_buffer[5]);
        wm_write_to_window(shell_window_id, "\n");
    }
    else if (strcmp(input_buffer, "fetch") == 0 || strcmp(input_buffer, "pfetch") == 0 || strcmp(input_buffer, "neofetch") == 0) {
        fetch_command(shell_window_id);
        log_info("User ran fetch");
    }
    else if (strlen(input_buffer) > 0) {
        wm_write_to_window(shell_window_id, "\nUnknown command: ");
        wm_write_to_window(shell_window_id, input_buffer);
        wm_write_to_window(shell_window_id, "\n");
    }
    
    input_pos = 0;
    memset(input_buffer, 0, SHELL_INPUT_BUFFER_SIZE);
}

void shell_process_input(char c) {
    if (c == '\n') {
        wm_write_char_to_window(shell_window_id, '\n');
        shell_process_command();
        shell_prompt();
    }
    else if (c == '\b') {
        if (input_pos > 0) {
            input_pos--;
            input_buffer[input_pos] = '\0';
            wm_write_char_to_window(shell_window_id, '\b');
        }
    }
    else if (c >= 32 && c <= 126) { // Printable ASCII
        if (input_pos < SHELL_INPUT_BUFFER_SIZE - 1) {
            input_buffer[input_pos++] = c;
            wm_write_char_to_window(shell_window_id, c);
        }
    }
}

void shell_run() {
    shell_window_id = 0; // Left window
    shell_prompt();
    
    while (1) {
        char c = keyboard_get_char();
        if (c != 0) {
            shell_process_input(c);
        }
    }
}
