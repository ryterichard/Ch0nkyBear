#include <windows.h>
#include <stdio.h>


typedef int (__stdcall *fmyMain)();

int main(){
    auto handle = LoadLibraryA("maliciousdll.dll");
    if (!handle){
        printf("Failed!\n");
    return 0;
    }
    fmyMain f = (fmyMain) GetProcAddress(handle, "myMain");
    f();
    Sleep(100000);
    printf("Done!");
    
}