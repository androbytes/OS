#pragma once
#include <stddef.h>
#include <stdint.h>
#include <limine.h>

void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
volatile void* memset(volatile void* s, int c, size_t n);
void* memmove(void* dest, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

size_t GetMemorySize(limine_memmap_response*);