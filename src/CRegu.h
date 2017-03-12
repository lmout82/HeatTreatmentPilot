//**********************************************************************************************
//                                        CRegu.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CRegu_H
#define CRegu_H

#include "wx/wx.h"

class CSerialPort;


class CRegu
{
public:
	CRegu();
	~CRegu();

	bool Command(int, int, int, int*);
	bool Command(int, int, int);
	bool StartProgram();
	bool StopProgramReset();
	bool Level1();
	bool Reboot();
	bool Status(bool, int, bool*);

private:
	int  GetFCS(wxString);
	bool GetError(wxString);
	CSerialPort* m_pPortCom;
	bool         m_bPortOK;
};

#endif