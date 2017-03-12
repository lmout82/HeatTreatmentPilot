//**********************************************************************************************
//                                        CRegu.cpp
//
// Author(s): lmout82
// Version 1.x
// Licence:  MIT License
// Link: https://github.com/lmout82/HeatTreatmentPilot
// Creation date: September 2007
// Last modification date: 2007
//***********************************************************************************************

#include "Cst.h"
#include "CSerialPort.h"
#include "CRegu.h"


CRegu::CRegu()
{
	m_pPortCom = new CSerialPort();

	if(!m_pPortCom->Open(COM1))
	{
		wxLogMessage("Unable to open the serial port.");
		this->m_bPortOK = false;
		return;
	}
		
	if(!m_pPortCom->Configure(9600, 7, 2, true, EVENPARITY))
	{
		wxLogMessage("Unable to setup the serial port.");
		this->m_bPortOK = false;
		return;
	}

	if(!m_pPortCom->Timeout(5000))
	{
		wxLogMessage("Unable to setup the timeout.");
		this->m_bPortOK = false;
		return;
	}

	this->m_bPortOK = true;
}

CRegu::~CRegu()
{
	m_pPortCom->Close();
	delete m_pPortCom;
}

bool CRegu::GetError(wxString code)
{
	if(code == "00")
	{
		return true;
	}
	else if(code == "0D")
	{
		wxLogMessage("#0D: the command can not be executed");
	}
	else if(code == "10")
	{
		wxLogMessage("#10: parity error");
	}
	else if(code == "11")
	{
		wxLogMessage("#11: no bit stop");
	}
	else if(code == "13")
	{
		wxLogMessage("#13: FCS is wrong");
	}
	else if(code == "14")
	{
		wxLogMessage("#14: format error");
	}
	else if(code == "15")
	{
		wxLogMessage("#15: numerical values/codes are incorrects");
	}
	else
	{
		wxLogMessage("#??: unknown error");
	}

	return false;
}

int CRegu::GetFCS(wxString str)
{
	int rr;
	int fcs = 0;
	const char* buf = str.c_str();

	for(size_t i=0; i<str.Len(); i++)
	{
		rr   = buf[i];
		fcs ^= buf[i];
	}

	return fcs;
}

bool CRegu::Command(int type_command, int code_command, int data_in, int* data_out)
{
	if(this->m_bPortOK == false)
		return false;

	wxString buf, command;
	int fcs = 0;

	buf.Printf("@00%1.1d%2.2d%4.4d", type_command, code_command, data_in);
	fcs = this->GetFCS(buf);

	command.Printf("%s%X*\r", buf, fcs);

	wxSleep(1);

	if(!m_pPortCom->WriteString(command))
	{
		wxLogMessage("Unable to send the command : %s", command);
		return false;
	}

	if(type_command == 3 && code_command == 11)
		return true;

	wxSleep(1);

	wxString response;
	if(!m_pPortCom->ReadString(&response, 16))
	{
		wxLogMessage("Unable to read the response to the command : %s", command);
		return false;
	}

	wxString code, data_response;
	code = response.Mid(6, 2);
	
	if(data_out != NULL)
	{
		data_response = response.Mid(8,4);
		data_response.ToLong((long*)data_out);
	}

	return this->GetError(code);
}

bool CRegu::Command(int type_command, int code_command, int data_in)
{
	return this->Command(type_command, code_command, data_in, NULL);
}

bool CRegu::StartProgram()
{
	if(this->m_bPortOK == false)
		return false;

	return this->Command(3, 0, 0);
}

bool CRegu::StopProgramReset()
{
	if(this->m_bPortOK == false)
		return false;

	return this->Command(3, 0, 1);
}

bool CRegu::Level1()
{
	if(this->m_bPortOK == false)
		return false;

	return this->Command(3, 9, 0);
}

bool CRegu::Reboot()
{
	if(this->m_bPortOK == false)
		return false;

	return this->Command(3, 11, 0);
}

bool CRegu::Status(bool group, int nb_bit, bool *state)
{
	if(this->m_bPortOK == false)
		return false;

	int iDataOut = 0;

	if(!this->Command(3, 14, (int)group, &iDataOut))
		return false;
}