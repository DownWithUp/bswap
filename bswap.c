/*
    Mostly taken from the sample simple Windbg extension.
    You will need to set configuration type to dynamic library (.dll)
*/
#include <Windows.h>
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

/*
    Define KDEXT_64BIT to make all wdbgexts APIs recognize 64 bit addresses
    It is recommended for extensions to use 64 bit headers from wdbgexts so the extensions could support 64 bit targets.
*/
#define KDEXT_64BIT
#include <WDBGEXTS.H>

typedef ULONG64 QWORD;

// Globals
EXT_API_VERSION         ApiVersion = { 1, 0, EXT_API_VERSION_NUMBER64, 0 };
WINDBG_EXTENSION_APIS   ExtensionApis;
ULONG SavedMajorVersion;
ULONG SavedMinorVersion;


BOOL DllMain(HANDLE hModule, DWORD dwReason, DWORD dwReserved)
{
    return (TRUE);
}

__declspec(dllexport) VOID WinDbgExtensionDllInit(PWINDBG_EXTENSION_APIS lpExtensionApis, USHORT MajorVersion, USHORT MinorVersion)
{
    ExtensionApis = *lpExtensionApis;
    SavedMajorVersion = MajorVersion;
    SavedMinorVersion = MinorVersion;
    return;
}

__declspec(dllexport) LPEXT_API_VERSION ExtensionApiVersion()
{
    /*
        ExtensionApiVersion should return EXT_API_VERSION_NUMBER64 in order for APIs
        to recognize 64 bit addresses.  KDEXT_64BIT also has to be defined before including
        wdbgexts.h to get 64 bit headers for WINDBG_EXTENSION_APIS
    */
    return &ApiVersion;
}


// Routine called by debugger after load
__declspec(dllexport) VOID CheckVersion()
{
    return;
}

__declspec(dllexport) DECLARE_API(bswap)
{
    QWORD qwValue;
    QWORD qwLittle;
    QWORD qwBig;
    BYTE b1, b2, b3, b4, b5, b6, b7, b8;
    if (GetExpressionEx(args, &qwValue, &args))
    {
        b1 = ((qwValue & 0xFF00000000000000) >> 56);
        b2 = ((qwValue & 0x00FF000000000000) >> 48);
        b3 = ((qwValue & 0x0000FF0000000000) >> 40);
        b4 = ((qwValue & 0x000000FF00000000) >> 32);
        b5 = ((qwValue & 0x00000000FF000000) >> 24);
        b6 = ((qwValue & 0x0000000000FF0000) >> 16);
        b7 = ((qwValue & 0x000000000000FF00) >> 8);
        b8 = ((qwValue & 0x00000000000000FF) >> 0);
        qwLittle = (((QWORD)b1 << 56) | ((QWORD)b2 << 48) | ((QWORD)b3 << 40) | ((QWORD)b4 << 32) | ((QWORD)b5 << 24) | ((QWORD)b6 << 16) | ((QWORD)b7 << 8) | ((QWORD)b8 << 0));
        qwBig = (((QWORD)b8 << 56) | ((QWORD)b7 << 48) | ((QWORD)b6 << 40) | ((QWORD)b5 << 32) | ((QWORD)b4 << 24) | ((QWORD)b3 << 16) | ((QWORD)b2 << 8) | ((QWORD)b1 << 0));
        dprintf("Little Endian: %I64X\n", qwLittle);
        dprintf("Big Endian: %I64X\n", qwBig);
    }
    else
    {
        dprintf("Usage for bswap.dll extension: !bswap [value]\n");
    }
}
