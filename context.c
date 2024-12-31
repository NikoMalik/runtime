#include "context.h"
#include "stdio.h"

int saveContext(Context *ctx) {
    __asm__ __volatile__("leaveq;\n\t"
                         "pop  %%rsi;\n\t"
                         "xorl %%eax,%%eax;\n\t"
                         "movq %%rbx,(%0);\n\t"
                         "movq %%rsp,8(%0);\n\t"
                         "pushq %%rsi;\n\t"
                         "movq %%rbp,16(%0);\n\t"
                         "movq %%r12,24(%0);\n\t"
                         "movq %%r13,32(%0);\n\t"
                         "movq %%r14,40(%0);\n\t"
                         "movq %%r15,48(%0);\n\t"
                         "movq %%rsi,56(%0);\n\t"
                         "ret;\n\t"
                         :
                         : "r"(ctx->buffer)
                         :);
    // never reach

    return 0;
};

int getContext() {
    // rdi is the first parameter
    __asm__ __volatile__("movq (%%rdi),%%rbx;\n\t"
                         "movq 8(%%rdi),%%rsp;\n\t"
                         "movq 16(%%rdi),%%rbp;\n\t"
                         "movq 24(%%rdi),%%r12;\n\t"
                         "movq 32(%%rdi),%%r13;\n\t"
                         "movq 40(%%rdi),%%r14;\n\t"
                         "movq 48(%%rdi),%%r15;\n\t"
                         "movl $1,%%eax;\n\t"
                         // pass param
                         "movq %%rdi,%%rsi;\n\t"
                         "movq 64(%%rdi),%%rdi;\n\t"
                         "jmpq *56(%%rsi);\n\t"
                         :
                         :
                         :);
    return 0;
};

int setContext(Context *ctx) {
    __asm__ __volatile__(
        "movq 0(%0), %%rbx;\n\t"  //  rbx
        "movq 8(%0), %%rsp;\n\t"  //  rsp
        "movq 16(%0), %%rbp;\n\t" //  rbp
        "movq 24(%0), %%r12;\n\t" //  r12
        "movq 32(%0), %%r13;\n\t" //  r13
        "movq 40(%0), %%r14;\n\t" //  r14
        "movq 48(%0), %%r15;\n\t" //  r15
        "movq 56(%0), %%rsi;\n\t" //  PC
        "jmp *%%rsi;\n\t"         // save PC
        :
        : "r"(ctx->buffer)
        : "memory", "rsi");

    // never reach
    return -1;
}

void swapContext(Context *from, Context *to) {
    if (!from || !to) {
        fprintf(stderr, "failed to switch contexts\n");
        return;
    }
    if (saveContext(from) == 0) {
        // If `saveContext` returns 0, we are in the saved context.
        // Now switch to the 'to' context.
        setContext(to);
    }
}

void makeContext(Context *context, void (*__func)(void), int __argc, ...) {

    context->r.pc = (uintptr)__func;

    context->r.rsp = (uintptr)((char *)context->stack.ss_sp + context->stack.ss_size);
}
