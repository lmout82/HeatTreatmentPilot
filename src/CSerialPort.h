//**********************************************************************************************
//                                        CSerialPort.h
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#ifndef CSerialPort_H
#define CSerialPort_H

#include "wx/wx.h"
#include "windows.h"


class CSerialPort
{
public:
	CSerialPort();
	~CSerialPort();

	bool Open(int);
	bool Configure(int, int, int, bool, int);
	bool WriteString(wxString);
	bool ReadString(wxString*, int);
	bool Timeout(int);
	bool Close();

private:
	HANDLE m_hPort;
	DCB    m_dcb;
	COMMTIMEOUTS m_timeout;
};

#endif