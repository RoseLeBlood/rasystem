#include "..\include\raMain.h"

namespace System
{
	bool g_IfAktive = false;

	namespace Util
	{
		raString raFile::raRemoveDir(raString Filename)
		{
			int iLastBackSlash;
			int iChar;

			// Letzten Back-Slash ("\") suchen
			iLastBackSlash = -1;
			iChar = 0;
			while(Filename[iChar] != 0)
			{
				if(Filename[iChar] == '\\') iLastBackSlash = iChar;
				iChar++;
			}

			// String ab dem letzten Back-Slash plus 1 zurückliefern
			return raString(Filename.c_str() + iLastBackSlash + 1);
		}

		raString raFile::raGetFilenameExtension(raString Filename)
		{
			int iLastDot;
			int iChar;

			// Letzten Punkt (".") im Dateinamen suchen
			iLastDot = -1;
			iChar = 0;
			while(Filename[iChar] != 0)
			{
				if(Filename[iChar] == '.') iLastDot = iChar;
				iChar++;
			}
			// String ab dem letzten Punkt plus 1 zurückliefern
			return raString(Filename.c_str() + iLastDot + 1).c_str();
		}
		BOOL raFile::raFileExists(raString Filename)
		{
			// Versuchen, die Datei zu öffnen
			FILE* pFile = fopen(Filename.c_str(), "rb");
			if(pFile) fclose(pFile);

			// pFile = NULL: Datei existiert nicht (oder Fehler), ansonsten existiert sie.
			return pFile != NULL;
		}
		DWORD raFile::raGetFileSize(raString Filename)
		{
			FILE *pFile;
			long length;

			if(!raFileExists(Filename))
			{
				return 0;
			}
			else
			{
				pFile=fopen(Filename.c_str(),"rb");
				fseek(pFile,0,SEEK_END);
				length=ftell(pFile);
				fclose(pFile);
			}

			return length;
		}
	}
	RAPI bool raCloseEngine(void)
	{
		CoUninitialize();
		System::raExitLog();

		return true;
	}
	RAPI bool raLoadEngine()
	{
		char buf[260];
		GetCurrentDirectory(260, buf);
		RASYSTEMPATH = raString(buf);

		RINFO("raSystem 1.08 Beta On " + raGetWinVersion() + "\n" );
		RINFO("Current Path: " + RASYSTEMPATH + "\n");

		System::raInitLog("./raLog/raslog.html");
		RINFO("Set Log output to: " + raString("./raLog/raslog.html"));

		raIni *conf = new raIni();

		if(conf->Open("./raConfig/config.ini"))
		{
			raConfigLoader::LoadConfig(conf);
		}
		else
		{
			raConfigLoader::LoadStandardConfig();
		}

		if(CoInitializeEx(NULL, COINIT_SPEED_OVER_MEMORY) == S_OK)
			ROK("CoInitializeEx");

		srand(timeGetTime());
		ROK("srand");

		int iCPU = omp_get_num_procs();
		omp_set_num_threads(iCPU);

		return true;
	}
	 raString raGetWinVersion()
	{
		OSVERSIONINFO info;
		OSVERSIONINFOEX ex;

		ZeroMemory( &info, sizeof( OSVERSIONINFO ) );
		ZeroMemory( &ex, sizeof( OSVERSIONINFOEX ) );
		info.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
		ex.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );

		GetVersionEx( &info );
		GetVersionEx( ( OSVERSIONINFO * ) &ex );

		if( info.dwMajorVersion == 6 && info.dwMinorVersion == 0 && ex.wProductType == VER_NT_WORKSTATION )
			return "(6.0) Windows Vista";

		if( info.dwMajorVersion == 6 && info.dwMinorVersion == 1 && ex.wProductType == VER_NT_WORKSTATION )
			return "(6.1) Windows 7";

		return "Windows";
	}
	RAPI long  raGetMemory()
	{
		MEMORYSTATUS lpBuffer;
		lpBuffer.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus (&lpBuffer);

		return lpBuffer.dwTotalPhys/1024/1024;
	}
	RAPI float raGetCpuFrequency()
	{
	#ifndef WIN64
		/* RdTSC:
		ReaD Time Stamp Counter :: Wird pro Takt um 1 erhöht
		*/
		#define RdTSC __asm _emit 0x0f __asm _emit 0x31

		__int64 cyclesStart = 0, cyclesStop = 0;
		unsigned __int64 nCtr = 0, nFreq = 0, nCtrStop = 0;

		if(!QueryPerformanceFrequency((LARGE_INTEGER *) &nFreq)) return 0;
		QueryPerformanceCounter((LARGE_INTEGER *) &nCtrStop);

		nCtrStop += nFreq;

		_asm
		{// Abrufen der Clock-Cycles für den Startwert
			RdTSC
			mov DWORD PTR cyclesStart, eax
			mov DWORD PTR [cyclesStart + 4], edx
		}

		do
		{
			QueryPerformanceCounter((LARGE_INTEGER *) &nCtr);
		}while (nCtr < nCtrStop);

		_asm
		{// Nochmals Abruf der Clock-Cycles nach einer Sekunde
			RdTSC
			mov DWORD PTR cyclesStop, eax
			mov DWORD PTR [cyclesStop + 4], edx
		}

		return ((float)cyclesStop-(float)cyclesStart) / 1024 / 1024;
	#else
		return 0.0f;
	#endif
	}
	RAPI void raSetAktive(bool Aktive)
	{
		g_IfAktive = Aktive;
	}
	RAPI bool raIsAktive(void)
	{
		return g_IfAktive;
	}
	RAPI int raGetRandInt(int min, int max)
	{
		return rand() / RAND_MAX * (max - min) + min;
	}

	RAPI float raGetRandFloat(float min, float max)
	{
		return (float)rand() / RAND_MAX * (max - min) + min;
	}
	RAPI float		raGetRandPercent()
	{
		float ret = (float)((rand() % 20000) - 10000);
		return ret / 10000.0f;
	}
	RAPI void		raComputePickRay(raCamera* pCamera, float xPos, float yPos, raVector3* v1, raVector3* v2)
	{
		raMatrix mViewProj;
		mViewProj =  pCamera->GetViewMatrix() * pCamera->GetProjMatrix();

		raMatrix Inverse;
		raMatrix invProj, invView;

		D3DXMatrixInverse((D3DXMATRIX*)&invView, NULL, (D3DXMATRIX*)&pCamera->GetViewMatrix());
		D3DXMatrixInverse((D3DXMATRIX*)&invProj, NULL, (D3DXMATRIX*)&pCamera->GetProjMatrix());
		D3DXMatrixInverse((D3DXMATRIX*)&Inverse, NULL, (D3DXMATRIX*)&mViewProj);

		raVector3 nearPoint(xPos, yPos, 0);
		raVector3 farPoint (xPos, yPos, 1);

		D3DXVec3TransformCoord((D3DXVECTOR3*)v1, (D3DXVECTOR3*)&nearPoint, (D3DXMATRIX*)&Inverse);
		D3DXVec3TransformCoord((D3DXVECTOR3*)v2, (D3DXVECTOR3*)&farPoint,  (D3DXMATRIX*)&Inverse);
	}
	RAPI raMatrix	raGetRandMatrix(float scaleX, float scaleY, float scaleZ, float offsetY)
	{
		raVector3 pos;
		pos.x = raGetRandPercent() * scaleX;
		pos.y = raGetRandPercent() * scaleY + offsetY;
		pos.z = raGetRandPercent() * scaleZ;

		float fRot = raGetRandPercent() * PI;

		raMatrix mTrans;
		raMatrix mRot;
		mRot = raMatrixRotationY(fRot); //Für rotationssymmetrische Objekte uninteressant
		mTrans = raMatrixTranslation(pos.x);

		return mRot * mTrans;
	}
	RAPI bool raRunProgramm(const  raString FileName, const  raString Parameter)
	{
		int ret = (int) ShellExecute(0, "open", FileName.c_str(), Parameter.c_str(), System::RASYSTEMPATH.c_str(), SW_SHOWNORMAL);

		if (ret <= 32)
		{
			char buf[260];

			switch(ret)
			{
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
				sprintf(buf, "Die Datei %s kann nicht gefunden werden", FileName);
				RERROR(buf);
				return false;

			case ERROR_BAD_FORMAT :
				RERROR("Die aufgerufene Anwendung ist ungültig oder beschädigt!");
				return false;

			case SE_ERR_ACCESSDENIED:
			case SE_ERR_SHARE:
				sprintf(buf, "Zugriff auf Datei %s wurde verweigert", FileName);
				RERROR(buf);
				return false;

			case  SE_ERR_OOM :
				RERROR("Out of Mem");
				return false;

			default:
				sprintf(buf, "Datei %s kann nicht geöffnet werden", FileName);
				RERROR(buf);
				return false;
			}
		}

		return true;
	}
	RAPI raFloat raSqrtf(raFloat X)
	{
		return sqrtf((float)X);
	}
	RAPI raFloat raAcosf(raFloat X)
	{
		return acosf((float)X);
	}
	RAPI raFloat raAbs(raFloat X)
	{
		return abs((float)X);
	}
	RAPI long raAbs(long X)
	{
		return abs((float)X);
	}
	RAPI double raAbs(double X)
	{
		return abs((float)X);
	}
	RAPI raFloat raSinf(raFloat X)
	{
		return sinf((float)X);
	}
	RAPI raFloat raCosf(raFloat X)
	{
		return cosf((float)X);
	}
	RAPI raFloat raTanf(raFloat X)
	{
		return tanf((float)X);
	}
};