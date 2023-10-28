#if _M_X64 || __x86_64__
#define push_register    \
{                        \
    __asm push rax       \
    __asm push rbx       \
    __asm push rcx       \
    __asm push rdx       \
    __asm push rsi       \
    __asm push rdi       \
    __asm push rbp       \
    __asm push rsp       \
    __asm push r8        \
    __asm push r9        \
    __asm push r10       \
    __asm push r11       \
    __asm push r12       \
    __asm push r13       \
    __asm push r14       \
    __asm push r15       \
}
#define pop_register    \
{                        \
    __asm pop r15        \
    __asm pop r14        \
    __asm pop r13        \
    __asm pop r12        \
    __asm pop r11        \
    __asm pop r10        \
    __asm pop r9         \
    __asm pop r8         \
    __asm pop rsp        \
    __asm pop rbp        \
    __asm pop rdi        \
    __asm pop rsi        \
    __asm pop rdx        \
    __asm pop rcx        \
    __asm pop rbx        \
    __asm pop rax        \
}
#endif

#if _M_IX86 || __i386__
#define push_register    \
{                        \
    __asm pushad         \
}
#define pop_register     \
{                        \
    __asm popad          \
}
#endif