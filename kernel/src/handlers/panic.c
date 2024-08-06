#include "src/include/stdio.h"

extern void syshalt(){
    __asm__("cli;hlt");
}

void panic(const char reason[]){
    if (reason == NULL){
        kprintf("%s LunarOS has become unstable and has decided to halt, sorry!\n", REDB);
    }else{
        kprintf("%s%s", REDB, reason);
    }

    syshalt();
}