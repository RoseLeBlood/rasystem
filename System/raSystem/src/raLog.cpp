#include "..\include\raMain.h"

namespace System
{
// ******************************************************************
// Globale Variablen
FILE* ra_g_pLogFile = NULL;	// Handle der Logbuchdatei

// ******************************************************************
// Initialisieren der Logbuchdatei
RAPI bool raInitLog( raString File)
{
	// Logbuchdatei öffnen
	ra_g_pLogFile = fopen(File.c_str(), "wt");
	if(!ra_g_pLogFile) return false;

	return true;
}

// ******************************************************************
// Schließen der Logbuchdatei
RAPI bool raExitLog()
{
	// Datei schließen
	if(ra_g_pLogFile)
	{
		fflush(ra_g_pLogFile);
		fclose(ra_g_pLogFile);
		ra_g_pLogFile = NULL;
	}

	return true;
}

// ******************************************************************
// Einen Text unverändert in die Logbuchdatei schreiben
RAPI bool raWriteToLogDirect( raString pcFormat)
{
	// String ins Logbuch schreiben
	if(ra_g_pLogFile)
	{
		fprintf(ra_g_pLogFile, "%s<br>", pcFormat.c_str());
#ifdef _DEBUG
		fflush(ra_g_pLogFile);
#endif
	}

	return true;
}

// ******************************************************************
// Entfernt HTML-Tags aus einem String
bool RemoveHTMLTags( raString pcIn,  raString pcOut, DWORD dwOutLength)
{
#ifndef WIN64
	DWORD dwInLength = strlen(pcIn);
#else
	size_t dwInLength = strlen(pcIn.c_str());
#endif
	DWORD dwOut = 0;

	ZeroMemory((void*)pcOut.c_str(), dwOutLength);
	for(DWORD dwIn = 0; dwIn < dwInLength; dwIn++)
	{
		if(pcIn[dwIn] != '<') {pcOut[dwOut] = pcIn[dwIn]; dwOut++;}
		else
		{
			while(pcIn[dwIn] != '>') dwIn++;
		}
	}

	return true;
}

// ******************************************************************
// Einen Text an HTML anpassen und in die Logbuchdatei schreiben
RAPI bool raWriteToLog( raString pcFormat)
{
	if(ra_g_pLogFile)
	{
		fprintf(ra_g_pLogFile, "<tr><td><font size=\"2\" color=\"#000080\">%s</font></td></tr><br>", pcFormat.c_str());
#ifdef _DEBUG
		fflush(ra_g_pLogFile);
#endif
	}
	OutputDebugString(pcFormat.c_str());

	return true;
}

// ******************************************************************
// Einen 2D-Vektor in die Logbuchdatei schreiben
RAPI bool raWriteVector2ToLog(raVector2& v)
{
	char temp[MAX_PATH];
	sprintf(temp, "<tr><td><font size=\"2\"><b><font color=\"#000080\">2D-Vektor:</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, Länge = <i>%.3f</i></td></tr>",
		                      v.x, v.y, raVector2Lenght(v));
	return raWriteToLogDirect(temp);
}

// ******************************************************************
// Einen 3D-Vektor in die Logbuchdatei schreiben
RAPI bool raWriteVector3ToLog(raVector3& v)
{
	// 3D-Vektor in die Logbuchdatei schreiben
	char temp[MAX_PATH];
	sprintf(temp, "<tr><td><font size=\"2\"><b><font color=\"#000080\">3D-Vektor:</font></b> x = <i>%.3f</i>, y = <i>%.3f</i>, z = <i>%.3f</i>, Länge = <i>%.3f</i></td></tr>",
		                      v.x, v.y, v.z, raVector3Lenght(v));
	return raWriteToLogDirect(temp);
}

// ******************************************************************
// Eine Matrix in die Logbuchdatei schreiben
RAPI bool raWriteMatrixToLog(raMatrix& m)
{
	// Matrix in die Logbuchdatei schreiben
	raWriteToLogDirect("<tr><td><font size=\"2\"><b><font color=\"#000080\">Matrix:</font></b><table>");
	for(int iRow = 0; iRow < 4; iRow++)
	{
		raWriteToLogDirect("<tr>");

		char buf[25];
		for(int iColumn = 0; iColumn < 4; iColumn++)
		{
			raWriteToLogDirect("<td><font size=\"1\">");
			sprintf(buf, "<i>%.3f</i>", m.m[iRow][iColumn]);
			raWriteToLogDirect(buf);
			raWriteToLogDirect("</td>");
		}

		raWriteToLogDirect("</tr>");
	}

	return raWriteToLogDirect("</table>");
}

// ******************************************************************
// Eine Ebene in die Logbuchdatei schreiben
RAPI bool raWritePlaneToLog(raPlane& p)
{
	// Ebene in die Logbuchdatei schreiben
	char temp[MAX_PATH];
	sprintf(temp, "<tr><td><font size=\"2\"><b><font color=\"#000080\">Ebene:</font></b> a = <i>%.3f</i>, b = <i>%.3f</i>, c = <i>%.3f</i>, d = <i>%.3f</i>, Normalenvektorlänge = <i>%.3f</i></td></tr>",
                              p.a, p.b, p.c, p.d, raVector4Lenght(p.n));
	return raWriteToLogDirect(temp);
}

// ******************************************************************
// Eine Farbe in die Logbuchdatei schreiben
RAPI bool raWriteColorToLog(raColor& c)
{
	char acHexColor[9];

	sprintf(acHexColor, "%x", (DWORD)(c) << 8);
	acHexColor[6] = 0;

	// Farbe in die Logbuchdatei schreiben
	char temp[MAX_PATH];
	sprintf(temp, "<tr><td><font size=\"2\"><b><font color=\"#000080\">Farbe:</font></b> a = <i>%.3f</i>, r = <i>%.3f</i>, g = <i>%.3f</i>, b = <i>%.3f</i>, Hexadezimal: <i>0x%x</i>, <font color=\"#%s\"><i>Probetext</i></font></td></tr>",
                              c.a, c.r, c.g, c.b, (DWORD)(c), acHexColor);
	return raWriteToLogDirect(temp);
}
};