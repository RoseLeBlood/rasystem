#pragma once

	class raCamera;
	namespace Util
	{
		class RAPI raFile
		{
		public:
			// Datei Functionen
			static DWORD	raGetFileSize(raString Filename);
			static BOOL		raFileExists(raString Filename);
			static raString	raGetFilenameExtension(raString Filename);
			static raString	raRemoveDir(raString Filename);
			static bool		raRunProgramm(raString FileName, raString Parameter);
		};
	}
	// Engine Functionen
	RAPI bool raLoadEngine();
	RAPI bool raCloseEngine(void);

	// Diverses
	RAPI  raString raGetWinVersion();
	RAPI long  raGetMemory();
#ifndef WIN64
	RAPI float raGetCpuFrequency();
#endif
	// Fenster
	RAPI raVector2 raGetWinSize(HWND hWnd);

	RAPI int		raGetRandInt(int min, int max);
	RAPI float		raGetRandFloat(float min, float max);
	RAPI float		raGetRandPercent();
	RAPI void		raComputePickRay(raCamera* pCamera, float xPos, float yPos, raVector3* v1, raVector3* v2);
	RAPI raMatrix	raGetRandMatrix(float scaleX, float scaleY, float scaleZ, float offsetY);

	//Math Function
	RAPI raFloat    raSqrtf(raFloat X);
	RAPI raFloat    raSinf(raFloat X);
	RAPI raFloat    raCosf(raFloat X);
	RAPI raFloat    raTanf(raFloat X);
	RAPI raFloat    raAcosf(raFloat X);
	RAPI raFloat    raAbs(raFloat X);