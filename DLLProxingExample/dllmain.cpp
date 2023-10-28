// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <stdio.h>
#include "push_pop_register.h"

#pragma comment(linker, "/EXPORT:function3=orgDLL_Example.function3")


extern "C" __declspec(dllexport) long function1(int a)
{
    /*
    Here we can do our modifications if we want
    */
    printf("function1 is hijacked\n");

    // define the original function signature
    typedef long(*myfunction1)(int a);

    // obtain a handle to the original DLL
    HMODULE hModule = LoadLibrary(TEXT("orgDLL_Example.dll"));

    // obtain the address of the original function
    myfunction1 ffunction1 =
        (myfunction1)(GetProcAddress(hModule, "function1"));

    // call the original function
    return ffunction1(a);
}

extern "C" __declspec(dllexport) void function2(void)
{
    // save CPU state, by saving the register content on the stack
    push_register;

    /*
    Here we can do our modifications if we want
    */
    printf("function2 is hijacked\n");

    // obtain a handle to the original DLL
    HMODULE hModule = LoadLibrary(TEXT("orgDLL_Example.dll"));

    // obtain the address of the original function
    FARPROC ffunction2 =
         GetProcAddress(hModule, "function2");

    // restore register content
    pop_register;

    // call the original function, with the orignal arguments
    ffunction2();
    return;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

