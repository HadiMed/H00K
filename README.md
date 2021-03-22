# H00K
<b> Hooking to a near call , to trick the process to execute  code each time a specific call is reached , usefull in many cases : binary instrumentation ; want to know state of some registers , memory adresses at specific state in the program without using breakpoints  <b/>
  
  
# How it works ? 

Knowing the address of some function and the index of the call you want to hook to , the Hook function look for the CALL instruction , replace the original callee address with your code address you want to execute , and returns the original function address to jump to after your hooking code is done executing . 

# Usage 

Write your code and compile it as form of DLL and inject it with <a href="https://github.com/HadiMed/Dll_injector"> Injector </a> 
Call H00k_CALL like this : 
```c++
H00k_CALL( LPVOID Function_to_look_where_to_hook , DWORD Your_code , int Right_index_for_the_call)
/* usefull in case you have many CALLs in function 1 signify first call , 2 second call etc ...*/
```
# Example
for this example im using actually a CTF challenge game named pwntown <br/> 
```c++

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "Main.h"

typedef DWORD (__cdecl _NetworkAuthenticator)(DWORD ecx); // Definition of function that we will Hook 
_NetworkAuthenticator *  _original = NULL; 



DWORD  __cdecl Hook_test(DWORD ecx) {
    
   printf("This is a Hook test \n");

   return  _original(ecx);
}


DWORD WINAPI maain(LPVOID ll) {

    void* pwntown = (void*)GetModuleHandle(L"GameAssembly.dll");
    _original = (_NetworkAuthenticator * )(H00k_CALL((BYTE * )pwntown + 0x2DEF5C , (DWORD)Hook_test , 1) | 0x00007FFD00000000
    /* H00k_CALL returns pointer 32bit size we need to make it adequate with x64 base Address of GameAssembly.dll*/) ; 
    return 1; 

}




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole(); 
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        CreateThread(NULL , 0 , &maain , NULL , 0 , NULL ); 
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
     
        break; 
    }
    return TRUE;
}
```

Now starting the game and injecting the DLL <br/>
We see that The Hook Worked :<br/> 
<img src="Hookout.png"/> 
