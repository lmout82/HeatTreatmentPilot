//**********************************************************************************************
//                                        CBalance.cpp
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
#include "CBalance.h"


CBalance::CBalance()
{
	m_pPortCom = new CSerialPort();

	if(!m_pPortCom->Open(COM2))
	{
		wxLogMessage("Unable to open the serial port.");
		this->m_bPortOK = false;
		return;
	}
		
	if(!m_pPortCom->Configure(9600, 8, 0, false, NOPARITY))
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

	m_bPortOK = true;
}

CBalance::~CBalance()
{
	m_pPortCom->Close();
	delete m_pPortCom;
}

bool CBalance::GetMass(double* mass)
{
	if(this->m_bPortOK == false)
		return false;

	wxString command;
	command = "w\r\n";

	if(!m_pPortCom->WriteString(command))
	{
		wxLogMessage("Failed to send the command: %s", command);
		return false;
	}

	wxSleep(1);

	wxString response;
	if(!m_pPortCom->ReadString(&response, 18))
	{
		wxLogMessage("Unable to read the response to the command : %s", command);
		return false;
	}

	const char* buf = response.c_str();
	int beginning = -1, end = -1;
	for(int i=0;i<18; i++)
	{
		if((buf[i]>=48 && buf[i]<=57) || buf[i] == 46)
		{
			beginning = i;
			break;
		}
	}

	if(beginning == -1)
	{
		wxLogMessage("Nothing to extract from the string.");
		*mass = 0;
		return false;
	}

	for(int i=beginning; i<18; i++)
	{
		if((buf[i]>=48 && buf[i]<=57) || buf[i] == 46)
		{}
		else
		{
			end = i;
			break;
		}
	}

	if(end == -1)
	{
		wxLogMessage("The number in the string is too big.");
		*mass = 0;
		return false;
	}

	wxString strMass;
	strMass = response.Mid(beginning, end-beginning);
	strMass.ToDouble(mass);

	// signe "-"
	if(buf[beginning-1] == 45)
	{
		*mass = -1*(*mass);
	}

	return true;
}