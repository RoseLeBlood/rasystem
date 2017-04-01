#include "..\include\raMain.h"

raString System::RASYSTEMPATH;
#pragma unmanaged

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

   pData = (DWORD *) lpvData; // Cast to my data type.
   // In this example, it is only a pointer to a DWORD
   // but it can be a structure pointer to contain more complicated data.

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

//-----------------------------------------------------------------
void RAPI RWRITE(raString x)
{
	System::raWriteToLog(x);
	printf("%s", x.c_str());
}
void RAPI ROK(raString x)
{
	System::raWriteToLog("<b><font color=\"#008000\">OK:</font></b>" + x);
	printf("OK: %s\n", x.c_str());
}
void RAPI RINFO(raString x)
{
	System::raWriteToLog("<b><font color=\"#008000\">INFO:</font></b> " + x);
	printf("INFO: %s\n", x.c_str());
}
void RAPI RWARNING(raString x)
{
	System::raWriteToLog("<b><font color=\"#FF0000\">WARNUNG:</font></b>" + x);
	printf("WARNUNG: %s\n", x.c_str());
}
void RAPI RERROR(raString x)
{
	System::raWriteToLog("<b><font color=\"#FF0000\">FEHLER:</font></b>" + x);
	printf("ERROR: %s\n", x.c_str());
}
void RAPI RERROR_NULLPOINTER(raString x)
{
	System::raWriteToLog("<b><font color=\"#FF0000\">FEHLER:</font></b> <i>" + x + "</i> ist NULL!");
	printf("ERROR: %s ist NULL\n", x.c_str());
}
void RAPI RERROR_INVALIDVALUE(raString x)
{
	System::raWriteToLog("<b><font color=\"#FF0000\">FEHLER:</font></b> <i>" + x + "</i> hat einen ungültigen Wert!");
	printf("ERROR: %s invalidvalue\n", x.c_str());
}
void RAPI RERROR_OUTOFMEM()
{
	System::raWriteToLog("<b><font color=\"#FF0000\">FEHLER:</font></b> Nicht genug Speicher!");
	printf("Out Of Mem\n");
}
void RAPI RERROR_FILE(raString  f)
{
	char temp[512];
	sprintf(temp,"<tr><td><font size=\"2\"><b><font color=\"#FF0000\">FEHLER:</font></b> Die Datei <i>%s</i> konnte nicht geöffnet, gelesen, erstellt oder beschrieben werden!</font></td>", f.c_str());
	System::raWriteToLog(temp);
	printf("ERROR: %s konnte nicht geöffnet, gelesen, erstellt oder beschrieben werden", f.c_str());
}
void RAPI RERROR_DX11(raString x, HRESULT h)
{
	System::raWriteToLog("<b><font color=\"#FF8000\">FEHLER-DIRECTX:</font></b>" + x + "["+  DXGetErrorString(h) + " " + DXGetErrorDescription(h) + "]</font></td>");
	printf("ERROR-DX11: %s [ %s - %s ]\n", x.c_str(), DXGetErrorString(h), DXGetErrorDescription(h) );
}
void RAPI RERROR_INPUT(raString x, HRESULT h)
{
}
//void RAPI RERROR_FMOD(FMOD_RESULT fr)
//{
//	raSystem::raWriteToLog("<b><font color=\"#FF8000\">FEHLER-FMOD:</font></b>" + raString(FMOD_ErrorString(fr)) + "</font></td>");
//	printf("FEHLER-FMOD: %s\n", raString(FMOD_ErrorString(fr)).c_str());
//}