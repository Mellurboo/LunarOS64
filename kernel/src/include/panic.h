#ifndef _PANIC_H
#define _PANIC_H 1

extern void syshalt();
void panic(const char reason[]);

#endif