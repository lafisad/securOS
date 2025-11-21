#ifndef STRING_H
#define STRING_H

#include "types.h"

size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);
char* strcpy(char* dest, const char* src);
void* memset(void* s, int c, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void reverse(char* s);
void itoa(int n, char* str);
void hex_to_string(uint32_t n, char* str);

#endif
