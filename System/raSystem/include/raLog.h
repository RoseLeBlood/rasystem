#pragma once

	RAPI bool raInitLog( raString File);
	RAPI bool raExitLog();
	RAPI bool raWriteToLogDirect(raString Format);
	RAPI bool raWriteToLog(raString Format);
	RAPI bool raWriteVector2ToLog(raVector2& v);
	RAPI bool raWriteVector3ToLog(raVector3& v);
	RAPI bool raWriteMatrixToLog(raMatrix& m);
	RAPI bool raWritePlaneToLog(raPlane& p);
	RAPI bool raWriteColorToLog(raColor& c);