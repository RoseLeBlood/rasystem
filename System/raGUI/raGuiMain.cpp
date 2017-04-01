#include <Windows.h>

static DWORD dwTlsIndex;
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	LPVOID lpvData;
    BOOL fIgnore;

    switch (fdwReason)
    {
        // The DLL is loading due to process
        // initialization or a call to LoadLibrary.
        case DLL_PROCESS_ATTACH:

            // Allocate a TLS index.
            if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
                return FALSE;
        // No break: Initialize the index for first thread.
        // The attached process creates a new thread.

        case DLL_THREAD_ATTACH:
            // Initialize the TLS index for this thread.
            lpvData = (LPVOID) LocalAlloc(LPTR, 256);
            if (lpvData != NULL)
                fIgnore = TlsSetValue(dwTlsIndex, lpvData);

            break;

        // The thread of the attached process terminates.
        case DLL_THREAD_DETACH:

            // Release the allocated memory for this thread.
            lpvData = TlsGetValue(dwTlsIndex);
            if (lpvData != NULL)
                LocalFree((HLOCAL) lpvData);

            break;

        // DLL unload due to process termination or FreeLibrary.
        case DLL_PROCESS_DETACH:

            // Release the allocated memory for this thread.
            lpvData = TlsGetValue(dwTlsIndex);
            if (lpvData != NULL)
                LocalFree((HLOCAL) lpvData);

            // Release the TLS index.
            TlsFree(dwTlsIndex);
            break;

        default:
            break;
    }
    return TRUE;
    UNREFERENCED_PARAMETER(hinstDLL);
    UNREFERENCED_PARAMETER(lpReserved);
}
#ifdef __cplusplus    // If used by C++ code,
extern "C" {          // we need to export the C interface
#endif

__declspec(dllexport)
BOOL WINAPI StoreData(DWORD dw)
{
   LPVOID lpvData;
   DWORD * pData;  // The stored memory pointer

   lpvData = TlsGetValue(dwTlsIndex);
   if (lpvData == NULL)
   {
      lpvData = (LPVOID) LocalAlloc(LPTR, 256);
      if (lpvData == NULL)
         return FALSE;
      if (!TlsSetValue(dwTlsIndex, lpvData))
         return FALSE;
   }

   pData = (DWORD *) lpvData;

   (*pData) = dw;
   return TRUE;
}

__declspec(dllexport)
BOOL WINAPI GetData(DWORD *pdw)
{
   LPVOID lpvData;
   DWORD * pData;  // The stored memory pointer

   lpvData = TlsGetValue(dwTlsIndex);
   if (lpvData == NULL)
      return FALSE;

   pData = (DWORD *) lpvData;
   (*pdw) = (*pData);
   return TRUE;
}
#ifdef __cplusplus
}
#endif