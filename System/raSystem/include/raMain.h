/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Philipp " phil175 " Schröck
	raSystem eine Grafik Engine mit DirectX 11 , FMOD
	Diese Engine steht unter der EUPL

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifdef RASYSTEM_EXPORT
#define RAPI __declspec(dllexport)
#else
#define RAPI __declspec(dllimport)
#endif

#ifndef WINVER
#define WINVER 0x0700
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0700
#endif

#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_DCOM

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma warning (disable:4018)
#pragma warning (disable:4251)
#pragma warning (disable:4244)
#pragma warning (disable:4793)
#pragma warning (disable:4835)

#define RASYSTEMVERSION            10805           // VV.RR.PP
#define RASYSTEMVERSIONSTRING     "01.0805"

#include <windows.h>
#include <Windowsx.h>
#include <D3D11.h>
#include <d3dCompiler.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include <DXErr.h>
#include <DXGI.h>
#include <DXFile.h>
#include <rmxfguid.h>
#include <shellapi.h>
#include "..\3rd-path\D3Dx11EffectFramework\d3dx11effect.h"
#include <DXFile.h>
#include <Commctrl.h>

#include <d3dx9xof.h>
#include <d3dx9mesh.h>
#include <rmxfguid.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Mmsystem.h>

#ifndef WIN64
#define RADLL (GetModuleHandle("raSystem.dll"))
#else
#define RADLL (GetModuleHandle("raSystemx64.dll"))
#endif

#define RASIZE_HACK(x, y) {if (((x) <= 0) && ( (y) <= 0)) return 0; }
#define RA_IFAKTIVE		  return raIsAktive()
#define RARENDER_HACK	  { if(raIsAktive() == false) return; }
#define RAMAX_DRAWODER	  6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <time.h>
#include <Sys\\Stat.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <conio.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <list>
#include <usp10.h>
#include <dimm.h>
#include <sstream>
#include <limits.h>
#include <ppl.h>

#include <omp.h>
//#include <Ws2tcpip.h>

typedef float			raFloat;
typedef std::string		raString;
typedef int				raInt;

void RAPI RWRITE(raString x);
void RAPI ROK(raString x);
void RAPI RINFO(raString x);
void RAPI RWARNING(raString x);
void RAPI RERROR(raString x);
void RAPI RERROR_NULLPOINTER(raString x);
void RAPI RERROR_INVALIDVALUE(raString x);
void RAPI RERROR_OUTOFMEM();
void RAPI RERROR_FILE(raString  f);
void RAPI RERROR_DX11(raString x, HRESULT h);
void RAPI RERROR_INPUT(raString x, HRESULT h);
//void RAPI RERROR_FMOD(FMOD_RESULT fr);

#define SAFE_DELETE(x)			{if((x)) {delete (x); (x) = NULL;}}			// Sicheres Löschen
#define SAFE_DELETE_ARRAY(x)	{if((x)) {delete[] (x); (x) = NULL;}}		// Sicheres Löschen eines Arrays
#define SAFE_RELEASE(x)			{if((x)) {(x)->Release(); (x) = NULL;}}		// Sicheres Releasen eines COM-Objekts
#define SAFE_MEMFREE(x)			{if((x)) {free((x)); (x) = NULL;}}		// Sicheres Freigeben von Speicher
#define RAMIN(a, b)				((a) < (b) ? (a) : (b))						// Minimum
#define RAMAX(a, b)				((a) > (b) ? (a) : (b))						// Maximum
#define PI						(3.1415926535897932384626433832795f)		// Pi
#define DEG2RAD(x)				((x) * 0.0174532925199432957692369076848f)	// Grad -> Bogenmaß
#define RAD2DEG(x)				((x) * 57.295779513082320876798154814105f)	// Bogenmaß -> Grad

#define COLOR_XRGB(r, g, b)		System::raColor(r, g, b)
#define COLOR_RGB(r, g, b)		COLOR_XRGB(r, g, b)
#define COLOR_ARGB(a, r, g, b)	System::raColor(r, g, b, a)

#define VECTOR2_2F(x, y)			System::raVector2(x, y)
#define VECTOR2_1F(f)				System::raVector2(f)

#define VECTOR3_3F(x, y, z)			System::raVector3(x,y,z)
#define VECTOR3_1F(f)				System::raVector3(f)

#define VECTOR4_4F(x,y,z,w)			System::raVector4(x,y,z,w)
#define VECTOR4_1F(f)				System::raVector4(f)

#define RM_CREATED				WM_USER+3
#define RM_ENTERLOOP			WM_USER+4
#define RM_EXITLOOP				WM_USER+5
#define RM_ERRORLOOP			WM_USER+6
#define RM_CONSOLE				WM_USER+7

namespace System
{
	struct RAANIMATION_FRAME_DATA;
};
typedef void*	raHandle;
typedef std::map<LPCSTR, System::RAANIMATION_FRAME_DATA*> raFrameMap;

namespace System
{
	#include "raProperties.h"
	#include "IRAUnknown.h"
	#include "raSmartPointer.h"
	#include "IRASection.h"
	#include "raCriticalSection.h"
	#include "raMap.h"
	#include "IRASingleton.h"
	#include "IConfigFile.h"
	#include "raArray.h"
	#include "raIni.h"
	#include "IRADelegate.h"
	#include "raEvent.h"
	#include "raThread.h"
	#include "raInputMap.h"
	#include "raVector2.h"
	#include "raVector3.h"
	#include "raVector4.h"
	#include "raColor.h"
	#include "raMatrix.h"
	#include "raLight.h"
	#include "raPlane.h"
	#include "raRECT.h"
	#include "raTimer.h"
	#include "raQuaternion.h"
	#include "raConfigLoader.h"
	#include "raList.h"
	#include "raLog.h"
	#include "raVectexType.h"
	#include "raGrafikDriver.h"
	#include "raDirectX.h"
	#include "raResource.h"
	#include "raSDKmesh.h"
	#include "raMaterial.h"
	#include "raProceduralMaterial.h"
	#include "raUtility.h"
	#include "IRAMesh.h"
	#include "IRAImage.h"
	#include "raIBMP.h"
	#include "raISGI.h"
	#include "raPostID.h"
	#include "raSocket.h"
	#include "raClient.h"
	#include "raServer.h"
	#include "raRender.h"
	#include "raVisual.h"
	#include "raNode.h"
	#include "raCamera.h"
	#include "raOrthoCamera.h"
	#include "raSceneManager.h"
	#include "raTexturedMaterial.h"
	#include "raAnimation.h"
	#include "raAnimated.h"
	#include "raPhysik.h"
	#include "raCollide.h"
	#include "raEntity.h"
	#include "raModel.h"
	#include "raEntitySDKMesh.h"
	#include "raGSEntity.h"
	#include "raOctree.h"
	#include "raBasics.h"
	#include "raBillboard.h"
	#include "raHoehenfeld.h"
	#include "raSkyBox.h"
	#include "raSkyDome.h"
	#include "raParticleSystem.h"
	#include "raRenderScreen.h"
	#include "raRTTMaterial.h"
	#include "raCMaterial.h"
	#include "raMinimap.h"
	#include "raComputeShaderObject.h"
	#include "raCSRessource.h"
	#include "raCSIOBuffer.h"
	#include "raGPUParticleSystem.h"
	#include "raRFMaterial.h"
	#include "raGPUHeightmap.h"
	#include "raTesselatinObject.h"
	#include "raRain.h"
	#include "raFractal.h"
	#include "raTree.h"
	#include "raWater.h"
	#include "raGroupe.h"
	#include "raGame.h"
	#include "raGameCharakter.h"
	#include "raLocation.h"
	#include "IRARawDevice.h"
	#include "raInput.h"
	#include "raRawKeyboard.h"
	#include "raThreadManager.h"

	extern raString RASYSTEMPATH;
}