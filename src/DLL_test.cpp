#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "Main.h"

typedef DWORD (__cdecl original)(DWORD ecx);
original* _original = NULL; 



DWORD  __cdecl HOOOKING(DWORD ecx) {
    
   printf("This is a Hook test \n");

   return  _original(ecx);
}


DWORD WINAPI nop(LPVOID ll) {

    void* pwntown = (void*)GetModuleHandle(L"GameAssembly.dll");
    
  _original = (original*)(H00k_CALL((BYTE * )pwntown + 0x2DEF5C , (DWORD)HOOOKING , 1) | 0x00007FFD00000000) ; 
     
     
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
        CreateThread(NULL , 0 , &nop , NULL , 0 , NULL ); 
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
     
        break; 
    }
    return TRUE;
}
