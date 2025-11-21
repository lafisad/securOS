#ifndef SHELL_H
#define SHELL_H

#include "../utils/types.h"

#define SHELL_INPUT_BUFFER_SIZE 256

void shell_init();
void shell_run();
void shell_process_input(char c);

#endif
