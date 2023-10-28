# DLL Function Hijacking
Dynamically-Linked Libraries are essentially collection of functions that can be used by multiple executables. These so-called exported functions by a DLL can be called by multiple executables. Most applications don't check the integrity of these DLL files. Therefore, they are vulnerable to being swapped out by a malicious DLL file if the directory is writable to an attacker. This technique is known as "DLL hijacking" or "DLL proxying".

Those techniques usually proxy all DLL functions to the orignal DLL and only add code into the DLL Main function. These can be problematic as gaining persistance that way can lead to easier detections as the mallicious code it often run at startup of an application. So another idea would be to replace a function that the DLL exports and that gets called by the application at some point, thus making it harder to deterministically trigger modified code. However, this may have a detrimental effect on the functionality of the program.

By combining the two ideas, we can get both benefits at the same time, by hijacking the control flow when a function is called, but also redirecting to the original function. It's similar to how a man-in-the-middle attack is carried out, so it can be advantageous to intercept function calls, for example to instrument a closed source application for fuzzing.


## Proxy DLL functions
Our modified DLL should redirect all unmodified functions to the original DLL. This will make the program work as expected. We can do this by telling the linker where it should map the functions.

To install the requirements for ```exportedFunctionsExtractor.py``` run ```pip install -r requirements.txt```.

```
C:\>python exportedFunctionsExtractor.py "c:\orgDLL_Example.dll"
```
We pass the DLL to the Python program ```exportedFunctionsExtractor.py```. It will look in the PE file for the exported functions and print out the linker comments for us to copy into our code. For each function that we don't want to hijack we need to remove the pragma comment.
```
#pragma comment(linker, "/EXPORT:function1=orgDLL_Example.function1")
#pragma comment(linker, "/EXPORT:function2=orgDLL_Example.function2")
#pragma comment(linker, "/EXPORT:function3=orgDLL_Example.function3")
```

## Hijacking functions
Depending on whether we know the function signature of the function we want to hijack or not, we need to proceed differently. Remember that our own DLL should have the same name as the original one. This is so that the application calls our own DLL.
For example, if we want to hijack ```long function1(int a) ``` for which we know the function signature, we can do the following

```
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
```

If we do not know the signature of e.g. ```function2``` we can can also hijack it by first saving the CPU registers, i.e. the state. Then we can execute our own code and restore those previously saved registers and continue to execute original function as if nothing had happened. To save and restore these named registers we can use the define statements in ```push_pop_registers.h```.

```
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
```
