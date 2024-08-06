#include "../include/panic.h"
//everything is handled by doing absoloutly nothing!!

__attribute__((noreturn))
void exception_handler(void);
void exception_handler(){
    panic("An exception was thrown!");
}