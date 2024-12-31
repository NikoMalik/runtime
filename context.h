#ifndef CONTEXT_H
#define CONTEXT_H
#include "types.h"
#include <stddef.h>

typedef struct Register {
    uintptr rbx, rsp, rbp, r12, r13, r14, r15, pc, rdi;
} Register;

typedef struct Stack {
    void *ss_sp;
    size_t ss_size;
    int ss_flags;
} Stack;

typedef union Context {
    uintptr buffer[9];
    Stack stack;
    Register r;
} Context;

int saveContext(Context *context);
int setContext(Context *context);
int getContext();
void makeContext(Context *context, void (*__func)(void), int __argc, ...);
void swapContext(Context *from, Context *to);

#endif // !CONTEXT_H
